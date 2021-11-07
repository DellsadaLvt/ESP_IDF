#ifndef __USER_UDP_H_
#define __USER_UDP_H_

#ifdef __cplusplus
extern "C"{
#endif

#include <stdio.h>
#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include "esp_log.h"
#include <lwip/netdb.h>

void user_udp_init( char *host, uint16_t port);
void udp_send_data(char *payload, uint16_t u16_len_payload, uint8_t flag);
void udp_recv_data( char *buff, uint16_t u16_len_buff, uint8_t flag);
void udp_shutdown( void );

#ifdef __cplusplus
}
#endif

#endif  /* __USER_UDP_H_ */
