#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"

#include "esp_tls.h"
#include "esp_crt_bundle.h"
// User include
#include "component/wifi_connection/user_wifi_connection.h"
#include "component/user_https_request/user_https_request.h"
#include "component/user_json/user_json.h"

static const char *TAG = "https_request";

static esp_err_t init_flash( void );
static void https_get_task(void *pvParameters);
static void https_post_task(void *pvParameters);

void app_main(void)
{
    ESP_ERROR_CHECK(init_flash());
    user_wifi_init_sta("Le Tu_2.4G", "17012005");
    xTaskCreate(&https_get_task, "https_get_task", 8192, NULL, 5, NULL);
    xTaskCreate(&https_post_task, "https_get_task", 8192, NULL, 5, NULL);
}


static esp_err_t init_flash( void ){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    return ESP_OK;
}

static void https_get_task(void *pvParameters)
{
    char *url = "https://api.thingspeak.com";
    char *request = "GET https://api.thingspeak.com/channels/1513929/fields/1.json?api_key=PYOHF0Z3CKGXDNVR&results=2\n\n";
    char buff[512u];
    char temp[200u];
   while(1){
        ESP_LOGW(TAG, "\nGet Task----------------------> \n");
        http_request(url, request, buff, 512u);
        printf("--->\n%s\n", buff);
        printf("---------Parsing json\n");
        parse_json_multi_obj(buff, "feeds", "field1", temp);
        printf("The token parse: %s\n", temp);
        vTaskDelay(pdMS_TO_TICKS(5000u));
   }
     
}

static void https_post_task(void *pvParameters){
    char *url = "https://api.thingspeak.com";
    char request[512u];
    char count= 10u;
   while(1){    
       ESP_LOGW(TAG, "\nPost Task----------------------> \n");
       sprintf(request, "POST /update HTTP/1.1\n\
Host: api.thingspeak.com\n\
Content-Type: application/x-www-form-urlencoded\n\
Content-Length: 34\n\n\
api_key=RDGA8L7G5R6NJIQC&field1=%d\n", count);
        count+=10u;
        if(count>80) count=10u;
        printf("The request: %s\n", request);
        http_request(url, request, NULL, 0);
        vTaskDelay(pdMS_TO_TICKS(15000u));
   }
}






