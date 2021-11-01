#ifndef __CONNECT_H

#define __CONNECT_H


#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

#include "lwip/err.h"
#include "lwip/sys.h"


typedef enum {
    wifi_switch_normal,
    wifi_switch_reset
} wifi_switch_mode_t;


#define EXAMPLE_ESP_WIFI_SSID      "Le Tu_2.4G"
#define EXAMPLE_ESP_WIFI_PASS      "17012005"
#define EXAMPLE_ESP_MAXIMUM_RETRY  (3u)

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1



/* Func prototypes */
void wifi_init_sta(void);


#endif

