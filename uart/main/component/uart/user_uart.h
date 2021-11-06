#ifndef __USER_UART_H_

#define __USER_UART_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "string.h"


void uart_init( uart_config_t uart_config );
void uart_event_handler ( uart_event_t *event, uint16_t u16_time_delay, uint8_t *dtmp, uint16_t u16_rd_buffer_size);


#ifdef __cplusplus
}
#endif

#endif

