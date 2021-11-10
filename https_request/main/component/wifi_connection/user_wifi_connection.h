#ifndef __USER_WIFI_CONNECTION_H_
#define __USER_WIFI_CONNECTION_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

void user_wifi_init_sta(char *ssid, char *pass);


#ifdef __cplusplus
}
#endif

#endif  /* __USER_WIFI_CONNECTION_H_ */

