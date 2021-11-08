#include "user_uart.h"

#define EX_UART_NUM UART_NUM_0
#define BUF_SIZE (1024)
#define QUEUE_EVEN_SIZE (20)
#define PATTERN_CHR_NUM    (2)        

static const char *TAG = "uart_events";
// this queue just stored uart event
static QueueHandle_t uart0_queue;
static size_t buffered_size;       

void uart_init( uart_config_t uart_config ){
    //Install UART driver, and get the queue.
    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, QUEUE_EVEN_SIZE, &uart0_queue, ESP_INTR_FLAG_IRAM);
    uart_param_config(EX_UART_NUM, &uart_config);
    
    //Set UART pins (using UART0 default pins ie no changes.)
    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    
    //Set uart pattern detect function.
    //hi[t1]+[t2]++[t3]hi[t1]+[t2]++[t3]hi[t1]+++
    // 9 as t2
    // post idle: 48000 as t3 if the duration less than 48000 the hi++ seem as a data
    // pre idle: 50000 as t1 if the duration less than 50000 the hi++ seem as a data
    uart_enable_pattern_det_baud_intr(EX_UART_NUM, '+', PATTERN_CHR_NUM, 9, 0, 0);  
    
    //Reset the pattern queue length to record at most 20 pattern positions.
    uart_pattern_queue_reset(EX_UART_NUM, QUEUE_EVEN_SIZE);
}


void uart_event_handler ( uart_event_t *event, uint16_t u16_time_delay, uint8_t *dtmp, uint16_t u16_rd_buffer_size){
    if(xQueueReceive(uart0_queue, (void*)event, (portTickType)pdMS_TO_TICKS(u16_time_delay))) {
        ESP_LOGI(TAG, "uart[%d] event:", EX_UART_NUM);
        memset(dtmp, '\0', u16_rd_buffer_size);
        switch(event->type) {
            //Event of UART receving data
            /*We'd better handler data event fast, there would be much more data events than
            other types of events. If we take too much time on data event, the queue might
            be full.*/
            // case UART_DATA:
            //     ESP_LOGI(TAG, "[UART DATA]: %d", event.size);
            //     uart_read_bytes(EX_UART_NUM, dtmp, event.size, portMAX_DELAY);
            //     ESP_LOGI(TAG, "[DATA EVT]:");
            //     uart_write_bytes(EX_UART_NUM, (const char*) dtmp, event.size);
            //     break;

            //Event of HW FIFO overflow detected
            case UART_FIFO_OVF:
                ESP_LOGI(TAG, "hw fifo overflow");
                // If fifo overflow happened, you should consider adding flow control for your application.
                // The ISR has already reset the rx FIFO,
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;

            //Event of UART ring buffer full
            case UART_BUFFER_FULL:
                ESP_LOGI(TAG, "ring buffer full");
                // If buffer full happened, you should consider encreasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(EX_UART_NUM);
                xQueueReset(uart0_queue);
                break;

            //Event of UART RX break detected
            case UART_BREAK:
                ESP_LOGI(TAG, "uart rx break");
                break;

            //Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGI(TAG, "uart parity error");
                break;

            //Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGI(TAG, "uart frame error");
                break;

            //UART_PATTERN_DET
            case UART_PATTERN_DET:
                // printf("uart pattern detected\n");
                uart_get_buffered_data_len(EX_UART_NUM, &buffered_size);
                int16_t pos = (int16_t)uart_pattern_pop_pos(EX_UART_NUM);
                ESP_LOGI(TAG, "[UART PATTERN DETECTED] pos: %d\n", pos);
                if (pos == -1) {
                    // There used to be a UART_PATTERN_DET event, but the pattern position queue is full so that it can not
                    // record the position. We should set a larger queue size.
                    // As an example, we directly flush the rx buffer here.
                    uart_flush_input(EX_UART_NUM);
                } else {
                    uart_read_bytes(EX_UART_NUM, dtmp, pos, 100 / portTICK_PERIOD_MS);
                    uint8_t pat[PATTERN_CHR_NUM + 1];
                    memset(pat, 0, sizeof(pat));
                    uart_read_bytes(EX_UART_NUM, pat, PATTERN_CHR_NUM, 100 / portTICK_PERIOD_MS);
                    ESP_LOGI(TAG, "read data: %s", dtmp);
                    ESP_LOGI(TAG, "read pat : %s", pat);
                }
                break;
            //Others
            default:
                ESP_LOGI(TAG, "uart event type: %d", event->type);
                break;
        }
    }
}









