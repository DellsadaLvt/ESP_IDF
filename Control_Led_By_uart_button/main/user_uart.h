#ifndef _USER_UART_H_
#define _USER_UART_H_

#ifdef __cplusplus
extern "C" {
#endif

// #include <stdbool.h>
// #include "esp_err.h"
// #include "esp_log.h"
#include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
#include "driver/uart.h"


/**
 * @brief Set UART data bits.
 *
 * @param uart_num Uart port number.
 * 
 *
 * @return
 *     void
 */
void vUart_config(uart_port_t uart_port, uart_config_t *uart_config, int BUF_SIZE, int queue_size, QueueHandle_t *uart0_queue);



#ifdef __cplusplus
}
#endif

#endif // _DRIVER_UART_H_
