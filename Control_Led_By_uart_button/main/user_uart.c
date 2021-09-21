#include "user_uart.h"


void vUart_config(uart_port_t uart_port, uart_config_t *uart_config, int BUF_SIZE, int queue_size, QueueHandle_t *uart0_queue){
    // config uart parmeter
    uart_param_config(uart_port, uart_config);
    // Install UART driver, and get the queue.
    uart_driver_install(uart_port, BUF_SIZE * 2, BUF_SIZE * 2, queue_size, uart0_queue, 0u);
}










