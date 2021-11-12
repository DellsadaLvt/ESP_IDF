#ifndef __USER_HTTPS_REQUEST_H_

#define __USER_HTTPS_REQUEST_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_system.h"
#include "esp_netif.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "lwip/netdb.h"
#include "lwip/dns.h"

#include "esp_tls.h"
#include "esp_crt_bundle.h"

/*"GET https://api.thingspeak.com/channels/1513929/fields/1.json?api_key=PYOHF0Z3CKGXDNVR&results=2\n\n"*/
void http_request(char *url, char *request, char *buf, uint16_t size_buff);
#ifdef __cplusplus
}
#endif

#endif  /* __USER_HTTPS_REQUEST_H_ */

