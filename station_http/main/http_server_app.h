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


typedef void (*http_post_callback_t) (char *data, uint16_t len);
typedef void (*http_get_callback_t) (void);
typedef void (*http_get_data_callback_t) (char *data, uint16_t len);
typedef void (*http_post_cmd_callback_t) (void);

void http_set_callback_switch(void *cb);
void http_set_calback_dht11( void *cb );
void http_set_callback_slider( void *cb );
void http_set_callback_wifi_inf( void *cb );
void http_set_rgb_callback( void *cb);
void http_set_wifi_reset_callback( void *cb );

void http_set_flag_enable( void );
void http_set_flag_disable( void );
void dht11_response( char *data, uint16_t len);
void start_webserver(void);
void stop_webserver(void);


#endif





