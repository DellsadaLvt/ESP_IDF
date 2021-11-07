#include <string.h>
#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "protocol_examples_common.h"
#include "nvs.h"
#include "nvs_flash.h"

/* user include */
#include "user_udp.h"

static const char *TAG = "app_main";

static void udp_client_task(void *pvParameters);

void app_main()
{
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    ESP_ERROR_CHECK(example_connect());

    xTaskCreate(udp_client_task, "udp_client", 4096, NULL, 5, NULL);
}

static void udp_client_task(void *pvParameters){
    char buff[50u];
    char data[50];
    uint8_t u8_count=0u;
    user_udp_init("192.168.1.9", 3333u);
    while (1) {
        if( u8_count < 11){
            memset(data, '\0', 50u);
            sprintf(data, "[%d] The message from Esp8266\n", u8_count);
            u8_count++;
            udp_send_data(data, strlen(data), MSG_DONTWAIT);
            udp_recv_data(buff, 50u, MSG_DONTWAIT);
        }
        else{
            udp_shutdown();
        }
        vTaskDelay(pdMS_TO_TICKS(1000u));
    }
    vTaskDelete(NULL);
}









