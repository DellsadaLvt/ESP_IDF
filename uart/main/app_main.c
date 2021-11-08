#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

/* User include */
#include "component\uart\user_uart.h"
#include "component\print\test.h"

#define RD_BUF_SIZE (1024)

static const char *TAG = "app_main";

static void uart_event_task(void *pvParameters);

void app_main(void)
{
    esp_log_level_set(TAG, ESP_LOG_INFO);

    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_APB,
    };
    uart_init(uart_config);

    //Create a task to handler UART event from ISR
    xTaskCreate(uart_event_task, "uart_event_task", 2048, NULL, 12, NULL);

}


static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    uint8_t* dtmp = (uint8_t*) calloc(1, RD_BUF_SIZE );
    for(;;) {
        print_something();
        // UART event.
        uart_event_handler(&event, 0u, dtmp, RD_BUF_SIZE);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    free(dtmp);
    dtmp = NULL;
    vTaskDelete(NULL);
}










