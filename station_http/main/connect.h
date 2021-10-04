#ifndef __CONNECT_H

#define __CONNECT_H

#include "esp_system.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "freertos/event_groups.h"
#include <string.h>

/* Inf wifi ap */
#define ESP_WIFI_SSID      "ESP8266"
#define ESP_WIFI_PASS      "0012345678"
#define MAX_STA_CONN       4

/* Inf wifi station */
#define EXAMPLE_ESP_WIFI_SSID      "Le Tu_2.4G" //"MyASUS"    //"Le Tu_2.4G"
#define EXAMPLE_ESP_WIFI_PASS      "17012005"   //"00011101"   //"17012005"
#define EXAMPLE_ESP_MAXIMUM_RETRY  (10)

/* State connect */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

/* Led status */
#define LED_STATUS (GPIO_NUM_2)

/* Wifi status */
typedef enum{
    wifi_non_connected,
    wifi_connected,
    wifi_disconnected
} wifi_notify_state_t;

typedef void (*wifi_notify_callback_t)( wifi_notify_state_t wifi_state);

/* Func prototypes */
void wifi_init_sta(void);
void reconnect_wifi(char *ssid, char *pass);
void wifi_init_softap( void );
void wifi_switch_station( char *ssid, char *pass );
void set_wifi_state_callback( void *cb );
void wifi_switch_ap( void );

#endif

