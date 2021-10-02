#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"

#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

/* User include */
#include "gpio.h"
#include "connect.h"
#include "http_server_app.h"


/* struct and enum */
typedef enum {
    FUNC_OK,
    FUNC_ERR
} state_func_t;

typedef struct rgb {
    uint8_t r, g, b;
    state_func_t state;
} rgb_color_t;


/* TAG message */
static const char *TAG = "------> main.c: ";

/* Prototypes function */
static void switch_data_callback( char *data, uint16_t len);
static void dht11_data_callback( void );
static void pwm_callback( char* data, uint16_t len);
static void rgb_value_handler(char *data, uint16_t len);
static void wifi_inf( char* data, uint16_t len );
static rgb_color_t x_seperate_color( char *rgb_color );
static uint16_t u16_rgb_to_ledc_value( uint8_t inp );


void app_main(){
    /* Init flash */
    ESP_ERROR_CHECK(nvs_flash_init());
    config_gpio_output();
    v_ledc_init();
    //config_pwm();
    /* set callback func */
    http_set_calback_dht11(dht11_data_callback);
    http_set_callback_switch(switch_data_callback);
    http_set_callback_slider(pwm_callback);
    http_set_callback_wifi_inf(wifi_inf);
    http_set_rgb_callback(rgb_value_handler);
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    start_webserver();
}


static void switch_data_callback( char *data, uint16_t len){
    if( *data == '1'){
        gpio_set_level(GPIO_NUM_2, 0u);
    }else{
        gpio_set_level(GPIO_NUM_2, 1u);
    }
}

static void dht11_data_callback( void ){
    char resp[100u];
    static uint8_t u8_test_value = 0u;
    u8_test_value += 20;
    sprintf(resp, "{\"temperature\": \"%d\", \"humidity\": \"%d\"}", u8_test_value, u8_test_value);
    dht11_response( resp, strlen(resp) );
}

static void pwm_callback( char* data, uint16_t len ){
    printf("The value slider: %s\n", data);
    // /* convert duty */
    // int duty_set = PWM_PERIOD/100u*atoi(data);
    // /* get current duty */
    // uint32_t duties[3];
    // uint32_t duty_current;
    // pwm_get_duty(0, &duty_current);
    // /* set duty */
    // if( duty_current > duty_set ){
    //     while(duty_current > duty_set){
    //         duties[0] = duties[1] = duties[2] = duty_current - (PWM_PERIOD/100u);    //duty_c*PWM_PERIOD/100u;
    //         /* begin change duty */
    //         pwm_stop(0);
    //         pwm_set_duties(duties);
    //         pwm_start();
    //         /* update current duty */
    //         pwm_get_duty(0, &duty_current);
    //         /* delay */
    //         vTaskDelay(pdMS_TO_TICKS(10u));
    //     }
    // }
    // else{
    //     while(duty_current < duty_set){
    //         duties[0] = duties[1] = duties[2] = duty_current + (PWM_PERIOD/100u);    //duty_c*PWM_PERIOD/100u;
    //         /* begin change duty */
    //         pwm_stop(0);
    //         pwm_set_duties(duties);
    //         pwm_start();
    //         /* update current duty */
    //         pwm_get_duty(0, &duty_current);
    //         /* delay */
    //         vTaskDelay(pdMS_TO_TICKS(10u));
    //     }
    // }
    
}

static void wifi_inf( char* data, uint16_t len ){
    char ssid[32]= "", pass[64]= "";
    char *token = strtok(data, "!@@!$^");
    if(token)
        sprintf(ssid,"%s", token);
    token = strtok(NULL, "!@@!$^");
    if(token)
        sprintf(pass, "%s", token);

    reconnect_wifi(ssid, pass);
}

static void rgb_value_handler(char *data, uint16_t len){
    rgb_color_t color = {0};
    color = x_seperate_color(data);
    ESP_LOGI(TAG, "The rgb --> %d, %d, %d\n", color.r, color.g, color.b);
    v_ledc_fade_set_duty(0, u16_rgb_to_ledc_value(color.r), 1000u);
    v_ledc_fade_set_duty(1, u16_rgb_to_ledc_value(color.g), 1000u);
    v_ledc_fade_set_duty(2, u16_rgb_to_ledc_value(color.b), 1000u);
}

static rgb_color_t x_seperate_color( char *rgb_color ){
    rgb_color_t color= {0};
    char buff[6u];
    sprintf(buff, "%s", rgb_color);
    
    /* convert str_hex to int */
    for(uint8_t i= 0u; i < 6u; i++){
        if( buff[i]>= 48 && buff[i] <= 57 ){
            if( i < 2){
                color.r <<= 4u;
                color.r |= ((uint8_t)buff[i] - 48);
            }
            else if ( i < 4){
                color.g <<= 4u;
                color.g |= ((uint8_t)buff[i] - 48);
            }
            else if ( i < 6){
                color.b <<= 4u;
                color.b |= ((uint8_t)buff[i] - 48);
            }
        }
        else if( buff[i]>= 65 && buff[i] <= 70 ){
            if( i < 2){
                color.r <<= 4u;
                color.r |= ((uint8_t)buff[i] - 65 + 10);
            }
            else if ( i < 4){
                color.g <<= 4u;
                color.g |= ((uint8_t)buff[i] - 65 + 10);
            }
            else if ( i < 6){
                color.b <<= 4u;
                color.b |= ((uint8_t)buff[i] - 65 + 10);
            }
        }
        else{
            color.state = FUNC_ERR;
            return color;
        }
    }
    color.state = FUNC_OK;
    return color;
}

static uint16_t u16_rgb_to_ledc_value( uint8_t inp ){
    return ((uint16_t)(inp*8000u/255u));
}












