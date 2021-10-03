#ifndef __HTTP_SERVER_APP_H
#define __HTTP_SERVER_APP_H


#include <sys/param.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "nvs.h"


#include <esp_http_server.h>

typedef void (*post_data_t)(char *data, uint16_t len);

void set_wifi_inf_callback( void *cb );

void start_webserver(void);
void stop_webserver(void);


#endif





