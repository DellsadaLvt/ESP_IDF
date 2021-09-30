#ifndef __CONNECT_H

#define __CONNECT_H

#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include <string.h>

/* Infor wifi */
#define EXAMPLE_ESP_WIFI_SSID      "Le Tu_2.4G" //"MyASUS"    //"Le Tu_2.4G"
#define EXAMPLE_ESP_WIFI_PASS      "17012005"   //"00011101"   //"17012005"
#define EXAMPLE_ESP_MAXIMUM_RETRY  (10)

/* State connect */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

/* Func prototypes */
void wifi_init_sta(void);
void reconnect_wifi(char *ssid, char *pass);

#endif

