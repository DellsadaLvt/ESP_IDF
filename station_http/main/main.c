#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "driver/gpio.h"
#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"
#include "driver/pwm.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "http_server_app.h"

/* Infor wifi */
#define EXAMPLE_ESP_WIFI_SSID      "Le Tu_2.4G"
#define EXAMPLE_ESP_WIFI_PASS      "17012005"
#define EXAMPLE_ESP_MAXIMUM_RETRY  (10)


/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* State connect */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

/* config for pwm */
// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD    (1000u)


// pwm pin number
const uint32_t pin_num[3] = {
    GPIO_NUM_12, 
    GPIO_NUM_13,
    GPIO_NUM_14
};

// duties table, real_duty = duties[x]/PERIOD
uint32_t duties[3] = { 
    500, 500, 500
};

// phase table, delay = (phase[x]/360)*PERIOD
float phase[3] = {
    0.0, 0.0, 0.0
};


static const char *TAG = "main wifi station";



/* Func prototypes */
void wifi_init_sta(void);
static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void switch_data_callback( char *data, uint16_t len);
void dht11_data_callback( void );
void pwm_callback( char* data, uint16_t len);
void config_gpio(void);
void config_pwm( void );



void app_main(){
    /* Init flash */
    ESP_ERROR_CHECK(nvs_flash_init());
    config_gpio();
    config_pwm();
    /* set callback func */
    http_set_calback_dht11(dht11_data_callback);
    http_set_callback_switch(switch_data_callback);
    http_set_callback_slider(pwm_callback);
    ESP_LOGI(TAG, "ESP_WIFI_MODE_STA");
    wifi_init_sta();
    start_webserver();
}

static void event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data){
    static int s_retry_num = 0;
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:%s",
                 ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

void wifi_init_sta(void){
    s_wifi_event_group = xEventGroupCreate();

    tcpip_adapter_init();

    ESP_ERROR_CHECK(esp_event_loop_create_default());

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .pmf_cfg = {
                .capable= true,
                .required= false
            },
        },
    };

    /* Setting a password implies station will connect to all security modes including WEP/WPA.
        * However these modes are deprecated and not advisable to be used. Incase your Access point
        * doesn't support WPA2, these mode can be enabled by commenting below line */

    if (strlen((char *)wifi_config.sta.password)) {
        wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA) );
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    /* Waiting until either the connection is established (WIFI_CONNECTED_BIT) or connection failed for the maximum
     * number of re-tries (WIFI_FAIL_BIT). The bits are set by event_handler() (see above) */
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);

    /* xEventGroupWaitBits() returns the bits before the call returned, hence we can test which event actually
     * happened. */
    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "connected to ap SSID:%s\n",
                 EXAMPLE_ESP_WIFI_SSID);
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s\n",
                 EXAMPLE_ESP_WIFI_SSID);
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }

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

void pwm_callback( char* data, uint16_t len){
    /* convert duty */
    int duty_set = atoi(data)*PWM_PERIOD/100u;
    /* get current duty */
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
            /* update duty current */
            duty_set = atoi(data)*PWM_PERIOD/100u;
            pwm_get_duty(0, &duty_current);
            printf("The duty: %d, %d\n", duty_current, duty_set);
            /* delay */
            vTaskDelay(pdMS_TO_TICKS(100u));
        }
    }
    else{
        while(duty_current < duty_set){
            duties[0] = duties[1] = duties[2] = duty_current + (PWM_PERIOD/100u);    //duty_c*PWM_PERIOD/100u;
            /* begin change duty */
            pwm_stop(0);
            pwm_set_duties(duties);
            pwm_start();
            /* update duty current */
            duty_set = atoi(data)*PWM_PERIOD/100u;
            pwm_get_duty(0, &duty_current);
            printf("The duty: %d, %d\n", duty_current, duty_set);
            /* delay */
            vTaskDelay(pdMS_TO_TICKS(100u));
        }
    }
    
}

void config_gpio(void){
    /* config gpio input gpio2: d4 */
    gpio_config_t io_conf;
    io_conf.intr_type= GPIO_INTR_DISABLE;
    io_conf.mode= GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask= (GPIO_Pin_2);
    io_conf.pull_down_en= GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en= GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    /* init first state */
    gpio_set_level(GPIO_NUM_2, 0u);
}

void config_pwm( void ){
    pwm_init(PWM_PERIOD, duties, 3, pin_num);
    pwm_set_phases(phase);
    pwm_start();
}



