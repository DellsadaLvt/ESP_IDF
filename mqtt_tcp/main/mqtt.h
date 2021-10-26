#ifndef _MQTT_H_
#define _MQTT_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "esp_log.h"
#include "mqtt_client.h"

typedef enum {
    err,
    disconnected,
    connected,
} mqtt_event_status_t;

typedef void (*get_client_mqtt_callback_t) (esp_mqtt_client_handle_t client);
typedef void(*func_callback_t) ( mqtt_event_status_t status );
typedef void(*get_payload_t)(char *topic, uint16_t u16_topic_len, char *data, uint16_t u16_data_len);


void mqtt_start(esp_mqtt_client_config_t *mqtt_cfg);
void mqtt_client_publish(esp_mqtt_client_handle_t client, char *topic, char *data, uint16_t u16_data_len, uint8_t u8_qos, uint8_t u8_retain );
void mqtt_client_subscribe(esp_mqtt_client_handle_t client, char *topic, uint8_t u8_qos);
void vset_get_client_mqtt_callback ( void *cb );
void vset_get_payload_callback ( void *cb );
void vset_mqtt_notify_callback( void *cb );

#ifdef __cplusplus
}
#endif //__cplusplus

#endif // _MQTT_H_


