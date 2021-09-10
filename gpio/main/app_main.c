/* gpio example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"


static const char *TAG = "GPIO_FUNC";


/* variables for queue */
QueueHandle_t xInputQueue=  NULL;

/* define pin number */
#define GPIO_OUTPUT_IO_2    2
#define GPIO_OUTPUT_PIN_SEL  (1ULL<<GPIO_OUTPUT_IO_2)
#define GPIO_INPUT_IO_5     5
#define GPIO_INPUT_PIN_SEL   (1ULL<<GPIO_INPUT_IO_5)

/* pin mode enum */
typedef enum{
    input = 0u,
    output = 1u, 
    input_interrupt= 3u
} gpio_type_mode_t;

/* typedef function pointer */
typedef void (*call_back_func_t)(gpio_num_t);
call_back_func_t call_func= NULL;

/*=============================== FUNC PROTOTYPE ==================================*/
void vConfig_gpio( uint32_t pin_bit_mask, gpio_type_mode_t xMode);
void vGpio_call_back( gpio_num_t xGpio_num);
void vSet_call_back_func( void *cb  );
/*=============================== INTERRUPTS FUNC  ==================================*/
static void gpio_isr_handler(void *arg){
    gpio_num_t gpio_num = (uint32_t) arg;
    call_func( gpio_num );
}


/*================================ APP MAIN =====================================*/
void app_main(void)
{

    /* create a queue using for interrupts */
    xInputQueue= xQueueCreate(1u, sizeof(unsigned char));

    /* config output */
    vConfig_gpio(GPIO_OUTPUT_PIN_SEL, output);
    /* config input */
    vConfig_gpio(GPIO_INPUT_PIN_SEL, input_interrupt);
    //install gpio isr service
    gpio_install_isr_service(0);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_5, gpio_isr_handler, (void *) GPIO_INPUT_IO_5);
    /* set call back function */
    vSet_call_back_func(vGpio_call_back);
    uint8_t u8Data;
    while (1) {
        
        if( xQueueReceive(xInputQueue, (void *)&u8Data, portMAX_DELAY) ){
            ESP_LOGI(TAG, "GPIO interrup %d\n", u8Data);
        }
        //vTaskDelay(pdMS_TO_TICKS(1000u));
    }
}





/*=============================== SUBROUNTINE ==================================*/
void vConfig_gpio( uint32_t pin_bit_mask, gpio_type_mode_t xMode){
    gpio_config_t io_conf;
    /* reset all value to zero  */
    memset(&io_conf, 0u, sizeof(io_conf));
    /* config pin */
    switch( xMode ){
        case input:
            /* config gpio input */
            io_conf.intr_type       = GPIO_INTR_DISABLE;
            io_conf.mode            = GPIO_MODE_INPUT;
            io_conf.pin_bit_mask    = pin_bit_mask;
            io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en      = GPIO_PULLUP_ENABLE;
            gpio_config(&io_conf);
            break;
        case output:
            //disable interrupt
            io_conf.intr_type = GPIO_INTR_DISABLE;
            //set as output mode
            io_conf.mode = GPIO_MODE_OUTPUT;
            //bit mask of the pins that you want to set,e.g.GPIO4
            io_conf.pin_bit_mask = pin_bit_mask;
            //disable pull-down mode
            io_conf.pull_down_en = 0;
            //disable pull-up mode
            io_conf.pull_up_en = 0;
            //configure GPIO with the given settings
            gpio_config(&io_conf);
            break;
        case input_interrupt:
            /* config gpio input */
            io_conf.intr_type       = GPIO_INTR_NEGEDGE;
            io_conf.mode            = GPIO_MODE_INPUT;
            io_conf.pin_bit_mask    = pin_bit_mask;
            io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en      = GPIO_PULLUP_ENABLE;
            gpio_config(&io_conf);
            break;
        default:
            ESP_LOGI(TAG, "vConfig_gpio ERROR\n");
            while(1);
    }
}

void vGpio_call_back( gpio_num_t xGpio_num){
    if( xGpio_num == GPIO_NUM_5 ) {
        static uint8_t u8Temp= 0u;
        BaseType_t pxHigherPriorityTaskWoken= pdFAIL;
        u8Temp++;
        gpio_set_level( GPIO_NUM_2, u8Temp%2 );
        xQueueSendFromISR(xInputQueue,(void*)&u8Temp, &pxHigherPriorityTaskWoken);
        if( pdPASS == pxHigherPriorityTaskWoken ){
            taskYIELD();
        }
    }
}

void vSet_call_back_func( void *cb  ){
    call_func= cb;
}


































