#ifndef __USER_UDP_SERVER_H_
#define __USER_UDP_SERVER_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

esp_err_t user_udp_init(uint16_t port);
esp_err_t user_upd_shutdown( void );
esp_err_t user_udp_send(char *buffer, uint16_t u16_len_buffer, uint8_t flag);
esp_err_t user_udp_recv(char *buffer, uint16_t u16_len_buffer, uint8_t flag);


#ifdef __cplusplus
}
#endif

#endif  /* __USER_UDP_SERVER_H_ */

