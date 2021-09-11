#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_system.h"
#include "userGpio.h"

/* use for esp_log */
static const char *TAG = "GPIO_FUNC";

/* variables for queue */
QueueHandle_t xInputQueue=  NULL;

/*======================== FUNC PROTOTYPE ===============================*/
void vGpio_call_back( gpio_num_t xGpio_num);


void app_main(void){   
    /* create a queue using for interrupts */
    xInputQueue= xQueueCreate(1u, sizeof(unsigned char));

    /* config output */
    vConfig_gpio(GPIO_NUM_2, output);
    /* config input */
    vConfig_gpio(GPIO_NUM_5, input_interrupt);
    /* set call back function */
    vSet_call_back_func(vGpio_call_back);

    uint8_t u8Data;
    while (1) {
        if( xQueueReceive(xInputQueue, (void *)&u8Data, portMAX_DELAY) ){
            gpio_set_level( GPIO_NUM_2, u8Data%2 );
            ESP_LOGI(TAG, "GPIO interrup %d\n", u8Data);
        }
        //vTaskDelay(pdMS_TO_TICKS(1000u));
    }
}

void vGpio_call_back( gpio_num_t xGpio_num){
    if( xGpio_num == GPIO_NUM_5 ) {
        static uint8_t u8Temp= 0u;
        BaseType_t pxHigherPriorityTaskWoken= pdFAIL;
        u8Temp++;
        xQueueSendFromISR(xInputQueue,(void*)&u8Temp, &pxHigherPriorityTaskWoken);
        if( pdPASS == pxHigherPriorityTaskWoken ){
            taskYIELD();
        }
    }
}


































