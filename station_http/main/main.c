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


/* TAG message */
static const char *TAG = "main wifi station";

/* Prototypes function */
void switch_data_callback( char *data, uint16_t len);
void dht11_data_callback( void );
void pwm_callback( char* data, uint16_t len);
void wifi_inf( char* data, uint16_t len );

void app_main(){
    /* Init flash */
    ESP_ERROR_CHECK(nvs_flash_init());
    config_gpio();
    config_pwm();
    /* set callback func */
    http_set_calback_dht11(dht11_data_callback);
    http_set_callback_switch(switch_data_callback);
    http_set_callback_slider(pwm_callback);
    http_set_callback_wifi_inf(wifi_inf);
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    start_webserver();
}


void switch_data_callback( char *data, uint16_t len){
    if( *data == '1'){
        gpio_set_level(GPIO_NUM_2, 0u);
    }else{
        gpio_set_level(GPIO_NUM_2, 1u);
    }
}

void dht11_data_callback( void ){
    char resp[100u];
    static uint8_t u8_test_value = 0u;
    u8_test_value += 20;
    sprintf(resp, "{\"temperature\": \"%d\", \"humidity\": \"%d\"}", u8_test_value, u8_test_value);
    dht11_response( resp, strlen(resp) );
}

void pwm_callback( char* data, uint16_t len ){
    /* convert duty */
    int duty_set = PWM_PERIOD/100u*atoi(data);
    /* get current duty */
    uint32_t duties[3];
    uint32_t duty_current;
    pwm_get_duty(0, &duty_current);
    /* set duty */
    if( duty_current > duty_set ){
        while(duty_current > duty_set){
            duties[0] = duties[1] = duties[2] = duty_current - (PWM_PERIOD/100u);    //duty_c*PWM_PERIOD/100u;
            /* begin change duty */
            pwm_stop(0);
            pwm_set_duties(duties);
            pwm_start();
            /* update current duty */
            pwm_get_duty(0, &duty_current);
            /* delay */
            vTaskDelay(pdMS_TO_TICKS(10u));
        }
    }
    else{
        while(duty_current < duty_set){
            duties[0] = duties[1] = duties[2] = duty_current + (PWM_PERIOD/100u);    //duty_c*PWM_PERIOD/100u;
            /* begin change duty */
            pwm_stop(0);
            pwm_set_duties(duties);
            pwm_start();
            /* update current duty */
            pwm_get_duty(0, &duty_current);
            /* delay */
            vTaskDelay(pdMS_TO_TICKS(10u));
        }
    }
    
}

void wifi_inf( char* data, uint16_t len ){
    char ssid[32]= "", pass[64]= "";
    char *token = strtok(data, "!@@!$^");
    if(token)
        sprintf(ssid,"%s", token);
    token = strtok(NULL, "!@@!$^");
    if(token)
        sprintf(pass, "%s", token);

    reconnect_wifi(ssid, pass);
}





