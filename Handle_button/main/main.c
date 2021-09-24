#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "esp_log.h"
#include "driver/gpio.h"


#define RESET_TIMER_COUNT	    ( 1 << 0 )
#define BLINK_LED_HIGH_SPEED	( 1 << 2 )
#define BLINK_LED_LOW_SPEED	    ( 1 << 3 )
#define TIME_OUT	            ( 1 << 4 )
#define DETERMINED_MODE	        ( 1 << 5 )


/* declare tag */
static const char *TAG = "Control Led";
/* declare a variable timer */
TimerHandle_t Timer=NULL;
/* Declare a variable to hold the created event group. */
EventGroupHandle_t x_event_group= NULL;

/* func prototype */
void delay( uint32_t u32_time_ms);
void v_gpio_isr_handler( void *arg );
void v_filter_button(void);
void timer_callback_func(  TimerHandle_t xTimer );
void config_gpio(void);
void v_task_handler_timer_count( void *arg);


void app_main(){
    /* create soft timer */
    do{
        Timer= xTimerCreate("timer_1", pdMS_TO_TICKS(100u), pdTRUE, (void*)1, timer_callback_func);
    }
    while(Timer == NULL);
    /* Attempt to create the event group. */
    do{
        x_event_group = xEventGroupCreate();
    }
    while(x_event_group== NULL);
    /* create a task handler timer count */
    xTaskCreate(v_task_handler_timer_count, "task_handler_button_count", configMINIMAL_STACK_SIZE, NULL, 10u, NULL);
    /* gpio config */
    config_gpio();
}


void v_gpio_isr_handler( void *arg ){
    /* handler gpio 13 */
    if(arg == (void*)GPIO_NUM_13){
        v_filter_button();
    }
}

void v_filter_button(void){
    float fl_tick_current;
    static float static_fl_tick_previous_falling= 0u, static_fl_tick_previous_rising= 0u;
    static char c_flag= 0u;
    BaseType_t pxHigherPriorityTaskWoken= pdFAIL;

    /* create a time distance to debounce */
    // update all value 
    fl_tick_current= xTaskGetTickCountFromISR();
    if( static_fl_tick_previous_falling == 0u )
        static_fl_tick_previous_falling= fl_tick_current;
    if( static_fl_tick_previous_rising == 0u )
        static_fl_tick_previous_rising= fl_tick_current;
    /* handler button for falling edge */
    if( (gpio_get_level(GPIO_NUM_13) == 0u) && (fl_tick_current - static_fl_tick_previous_falling > 3u) && (c_flag == 0u)){
        static_fl_tick_previous_falling = 0u;
        c_flag= 1u;
        // set bit to reset timer count
        xEventGroupSetBitsFromISR(x_event_group, RESET_TIMER_COUNT, &pxHigherPriorityTaskWoken);
        // start timer to count the time press button
        xTimerStartFromISR(Timer, &pxHigherPriorityTaskWoken);
    }
    /* handler button for rising edge */
    if( (gpio_get_level(GPIO_NUM_13) == 1u) && (fl_tick_current - static_fl_tick_previous_rising > 3u) && (c_flag == 1u) ){
        static_fl_tick_previous_rising = 0u;
        c_flag= 0u;
        // Set bit to determine mode
        xEventGroupSetBitsFromISR(x_event_group, DETERMINED_MODE, &pxHigherPriorityTaskWoken);
        // stop timer count time press button
        //xTimerStopFromISR(Timer, &pxHigherPriorityTaskWoken);
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
        static uint8_t u8_state_led= 0,_u8_time_count= 0u, timer_count_buffer= 0u;
        EventBits_t uxBits;
        /* Wait a maximum of 100ms for either bit 0 or bit 4 to be set within
        the event group.  Clear the bits before exiting. */
        uxBits = xEventGroupWaitBits(
                    x_event_group,   /* The event group being tested. */
                    RESET_TIMER_COUNT | DETERMINED_MODE, /* The bits within the event group to wait for. */
                    pdTRUE,        /* BIT_0 & BIT_4 should be cleared before returning. */
                    pdFALSE,       /* Don't wait for both bits, either bit will do. */
                    0u );/* no wait. */
        /* reset all variables */
        if( uxBits &  RESET_TIMER_COUNT){
            timer_count_buffer= 0u;
            _u8_time_count= 0u;
            //temp= 0u;
        }
        else if( uxBits & DETERMINED_MODE){
            if( _u8_time_count < 10u)
                xEventGroupSetBits(x_event_group, BLINK_LED_HIGH_SPEED);
            else if( _u8_time_count < 50)
                xEventGroupSetBits(x_event_group, BLINK_LED_LOW_SPEED);
            else
                xEventGroupSetBits(x_event_group, TIME_OUT);
            // stop timer count time press button
            xTimerStop(Timer, 0u);
        }
        /* create timer count */
        _u8_time_count++;
        /* handler time count */
        if(_u8_time_count < 10u){
            /* blink led 100ms */
            u8_state_led ^= 1;
            gpio_set_level(GPIO_NUM_2, u8_state_led);
            /* set event group to change mode 1 */
            // if( ((temp>>1u) & 1u) == 0u ){
            //     temp |= (1u<<1u);
            //     xEventGroupSetBits(x_event_group, BLINK_LED_HIGH_SPEED);
            // }
        }
        else if(_u8_time_count < 50u){
            if( _u8_time_count - timer_count_buffer >= 3u){
                timer_count_buffer= _u8_time_count;
                u8_state_led ^= 1;
                gpio_set_level(GPIO_NUM_2, u8_state_led);
                /* set event group to change mode 1 */
                // if( ((temp>>2u) & 1u) == 0u ){
                //     temp |= (1u<<2u);
                //     xEventGroupSetBits(x_event_group, BLINK_LED_LOW_SPEED);
                // }
            }
        } 
        else{
            gpio_set_level(GPIO_NUM_2, 0U);
            /* set event group to change mode 1 */
            // if( ((temp>>3u) & 1u) == 0u ){
            //     temp |= (1u<<3u);
            //     xEventGroupSetBits(x_event_group, TIME_OUT);
            // }
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

void v_task_handler_timer_count( void *arg ){
    EventBits_t x_event_bit= 0u;
    while(1){
        /* wait for receive bit */
        x_event_bit= xEventGroupWaitBits(x_event_group, BLINK_LED_HIGH_SPEED|BLINK_LED_LOW_SPEED|TIME_OUT, pdTRUE, pdFALSE, portMAX_DELAY );
        if(x_event_bit & BLINK_LED_HIGH_SPEED ){
            ESP_LOGI(TAG,"blink high speed\n");
        }
        else if(x_event_bit & BLINK_LED_LOW_SPEED ){
            ESP_LOGI(TAG,"blink low speed\n");
        }
        else if(x_event_bit & TIME_OUT ){
            ESP_LOGI(TAG,"time out\n");
        }
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
}




