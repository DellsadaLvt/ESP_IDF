#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

// User include 
#include "component/tcp_server/user_udp_server.h"
#include "component/wifi_connection/user_wifi_connection.h"

#define PORT CONFIG_EXAMPLE_PORT

static const char *TAG = "app_main";

// Prototype functions
static void init_flash( void );
static void udp_server_task(void *pvParameters);

void app_main(void)
{
    //Initialize NVS
    init_flash();

    //Connect to wifi station
    user_wifi_init_sta("Le Tu_2.4G", "17012005");

    // Create task
    xTaskCreate(udp_server_task, "udp_server", 4096, NULL, 5, NULL);
}

static void init_flash( void ){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

static void udp_server_task(void *pvParameters){
    char data[]= "Message ACK from esp32\n";
    char buff[50u];
    user_udp_init(3333u);
    printf("Server is waiting\n");
    while (1) {
        user_udp_recv(buff, 50u, 0);
        user_udp_send(data, strlen(data), 0);
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
    vTaskDelete(NULL);
}


