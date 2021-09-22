#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"


/* create tag */
static const char *TAG = "Control Led";
/* create timer */
TimerHandle_t Timer=NULL;

/* func prototype */
void delay( uint32_t u32_time_ms);
void v_gpio_isr_handler( void *arg );
void v_filter_button(void);
void timer_callback_func(  TimerHandle_t xTimer );
void config_gpio(void);

/* global variables */
 volatile uint32_t g_u32_count= 10u, g_u32_temp= 10u;
 volatile uint8_t g_u8_time_count;


void app_main(){
    /* create soft timer */
    Timer= xTimerCreate("timer_1", pdMS_TO_TICKS(100u), pdTRUE, (void*)1, timer_callback_func);
    /* gpio config */
    config_gpio();
    /* variables */
    uint32_t count= 0u;
    while(1u){
        vTaskDelay(pdMS_TO_TICKS(20u));
        if( count != g_u32_count ){
            count = g_u32_count;
            ESP_LOGI(TAG, "count with filter  %d, count not filter: %d, time press button: %d\n", g_u32_count, g_u32_temp, g_u8_time_count);
        }
    }
}


void v_gpio_isr_handler( void *arg ){
    /* handler gpio 13 */
    if(arg == (void*)GPIO_NUM_13){
        g_u32_temp++;
        v_filter_button();
    }
}

void v_filter_button(void){
    float fl_tick_current;
    static float static_fl_tick_previous_falling= 0u, static_fl_tick_previous_rising= 0u;
    static char c_flag= 0u;
    BaseType_t pxHigherPriorityTaskWoken= pdFAIL;

    /* create a time distance to debounce */
    fl_tick_current= xTaskGetTickCountFromISR();
    if( static_fl_tick_previous_falling == 0u )
        static_fl_tick_previous_falling= fl_tick_current;
    if( static_fl_tick_previous_rising == 0u )
        static_fl_tick_previous_rising= fl_tick_current;
    /* handler button for falling edge */
    if( (gpio_get_level(GPIO_NUM_13) == 0u) && (fl_tick_current - static_fl_tick_previous_falling > 3u) && (c_flag == 0u)){
        static_fl_tick_previous_falling = 0u;
        c_flag= 1u;
        /* handler button */
        g_u32_count++;   
        g_u8_time_count= 0u;     
        xTimerStartFromISR(Timer, &pxHigherPriorityTaskWoken);
    }
    /* handler button for rising edge */
    if( (gpio_get_level(GPIO_NUM_13) == 1u) && (fl_tick_current - static_fl_tick_previous_rising > 3u) && (c_flag == 1u) ){
        static_fl_tick_previous_rising = 0u;
        c_flag= 0u;
        /* handler button */
        g_u32_count--;
        xTimerStopFromISR(Timer, &pxHigherPriorityTaskWoken);
    }
    if(pxHigherPriorityTaskWoken == pdTRUE){
        taskYIELD();
    }
}

void delay( uint32_t u32_time_ms){
    /* convert ms_set to tick_set */
    uint32_t u32_tick_set= pdMS_TO_TICKS(u32_time_ms);
    /* get current tick */
    uint32_t u32_tick_current= xTaskGetTickCount();
    /* start delay */
    while( xTaskGetTickCount() < (u32_tick_current + u32_tick_set));
}

void timer_callback_func(  TimerHandle_t xTimer ){
    if( xTimer == Timer ){
        static uint8_t u8_state_led= 0, temp;

        if(g_u8_time_count == 0u)
            temp= 0u;
        g_u8_time_count++;
        if(g_u8_time_count < 10u){
            /* blink led 100ms */
            u8_state_led ^= 1;
            gpio_set_level(GPIO_NUM_2, u8_state_led);
        }
        else if(g_u8_time_count < 50u){
            if( g_u8_time_count - temp >= 3u){
                temp= g_u8_time_count;
                u8_state_led ^= 1;
                gpio_set_level(GPIO_NUM_2, u8_state_led);
            }
        } 
    }
}

void config_gpio(void){
    /* config gpio input gpio13: d7 */
    gpio_config_t io_conf={0u};
    io_conf.intr_type= GPIO_INTR_ANYEDGE;
    io_conf.mode= GPIO_MODE_INPUT;
    io_conf.pin_bit_mask= (GPIO_Pin_13);
    io_conf.pull_down_en= GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en= GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    /* config isr */
    gpio_install_isr_service(0u);
    /* add isr gpio pin 13: D7 */
    gpio_isr_handler_add(GPIO_NUM_13, v_gpio_isr_handler, (void*)GPIO_NUM_13);

    /* config gpio input gpio2: d4 */
    io_conf.intr_type= GPIO_INTR_DISABLE;
    io_conf.mode= GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask= (GPIO_Pin_2);
    io_conf.pull_down_en= GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en= GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    /* init first state */
    gpio_set_level(GPIO_NUM_2, 0u);
}





