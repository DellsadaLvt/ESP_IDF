#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/timers.h"

#include "esp_log.h"
#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"
#include "driver/pwm.h"
#include "user_uart.h"
#include "user_gpio.h"


/* create tag */
static const char *TAG = "Control Led";
/* define uart */
#define EX_UART_NUM         (UART_NUM_0)
#define BUF_SIZE            (1024)                             // using for uart buffer
#define RD_BUF_SIZE         (BUF_SIZE)
static QueueHandle_t        uart0_queue;                       // detect uart event
/* define pwm */
#define PWM_PERIOD          ((uint32_t)1000000u)               // PWM period 1000us(1Khz), same as depth
const uint32_t pwm_pin=      12u;                              // D6
/* define for gpio pin */
#define BUTTON              (GPIO_NUM_13)                      // D7
volatile uint8_t u8_temp= 10u;
/* soft timer */
TimerHandle_t x_timer_1=     NULL;
volatile uint32_t u32_timer1_counter=   0u, u32_time_start= 0u, u32_time_stop= 0u;



/*tasks handler when uart receive data*/
static void uart_event_task(void *pvParameters);
/* GPIO callback function */
void v_gpio_callback( gpio_num_t gpio_num );
/* timer call back */
void v_timer_callback( TimerHandle_t xTimer );


/*===============  app main config function, after config, it will delete =============*/
void app_main()
{
    /* config pwm */
    uint32_t duty= 500000u;
    pwm_init(PWM_PERIOD, (uint32_t *)&duty, 1u, (uint32_t*)&pwm_pin );
    pwm_set_phase(1u,0u);
    pwm_start();

    /* config uart */
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };
    vUart_config(EX_UART_NUM, &uart_config, BUF_SIZE, 100u, &uart0_queue);
    
    /* GPIO config */
    gpio_config_t io_conf;
    memset(&io_conf, 0u, sizeof(io_conf));
    io_conf.intr_type       = GPIO_INTR_ANYEDGE;
    io_conf.mode            = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask    = (1ULL << BUTTON);;
    io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en      = GPIO_PULLUP_ENABLE;
    v_gpio_config(gpio_input_isr, &io_conf, BUTTON);
    v_set_gpio_callback(v_gpio_callback);
            

    // Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);
    /* Create a timer */
    x_timer_1= xTimerCreate("timer_1", pdMS_TO_TICKS(100u), pdTRUE, (void*)0u, v_timer_callback);
    /* Start timer 1 */
    xTimerStart(x_timer_1, portMAX_DELAY);
    
    /* TEST SECTION */
    while(1){
        ESP_LOGI(TAG, "the timer counter: %d, %d\n", u32_timer1_counter, u32_time_start);
        vTaskDelay(pdMS_TO_TICKS(500u));
    }
}

/*===================  CALLBACK FUNCTION ===================*/
void v_gpio_callback( gpio_num_t gpio_num ){
    if( gpio_num == BUTTON ){
        if( 0u == gpio_get_level(BUTTON)){
            u8_temp++;
            u32_time_start= u32_timer1_counter;
        }
        else if( 1u == gpio_get_level(BUTTON)){
            u8_temp--;
            u32_time_stop= u32_timer1_counter;
        }
    }
}


void v_timer_callback( TimerHandle_t xTimer ){
    if( x_timer_1 == xTimer ){
        u32_timer1_counter++;
    }
}

/*========================= TASK ===========================*/
static void uart_event_task(void *pvParameters){
    uart_event_t event;
    uint8_t *dtmp = (uint8_t *) malloc(RD_BUF_SIZE);
    
    for (;;) {
        // Waiting for UART event.
        if (xQueueReceive(uart0_queue, (void *)&event, (portTickType)pdMS_TO_TICKS(1000u))) {
            bzero(dtmp, RD_BUF_SIZE);

            switch (event.type) {
                // Event of UART receving data
                // We'd better handler data event fast, there would be much more data events than
                // other types of events. If we take too much time on data event, the queue might be full.
                case UART_DATA:
                    //ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
                    uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
                    //ESP_LOGI(TAG, "[DATA EVT]:");
                    uart_write_bytes(EX_UART_NUM, (const char *) dtmp, event.size);
                    break;

                // Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;

                // Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;

                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;

                // Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;

                // Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }

    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}
























