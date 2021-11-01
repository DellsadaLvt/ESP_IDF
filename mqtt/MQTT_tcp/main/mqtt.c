#include "mqtt.h"

func_callback_t mqtt_client_notify =  NULL;
get_client_mqtt_callback_t get_client_mqtt_callback= NULL;
get_payload_t get_payload_callback = NULL;

static const char *TAG = "MQTT.c";


static esp_err_t mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
    switch (event->event_id) {
        case MQTT_EVENT_CONNECTED:
            //ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
           /* send client to main.c */
            get_client_mqtt_callback(event->client);
            break;
        case MQTT_EVENT_DISCONNECTED:
            //ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
            mqtt_client_notify( disconnected );
            break;
        case MQTT_EVENT_SUBSCRIBED:
            //ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            //ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_PUBLISHED:
            //ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
            break;
        case MQTT_EVENT_DATA:
            //ESP_LOGI(TAG, "MQTT_EVENT_DATA");
            get_payload_callback(event->topic, event->topic_len, event->data, event->data_len);
            break;
        case MQTT_EVENT_ERROR:
            //ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
            mqtt_client_notify( err );
            break;
        default:
            //ESP_LOGI(TAG, "Other event id:%d", event->event_id);
            break;
    }
    return ESP_OK;
}

static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data) {
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    mqtt_event_handler_cb(event_data);
}


/* Call functions */
void mqtt_start(esp_mqtt_client_config_t *mqtt_cfg)
{
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
}

void mqtt_client_publish(esp_mqtt_client_handle_t client, char *topic, char *data, uint16_t u16_data_len, uint8_t u8_qos, uint8_t u8_retain )
{
    esp_mqtt_client_publish(client, topic, data, u16_data_len, u8_qos, u8_retain);
}

void mqtt_client_subscribe(esp_mqtt_client_handle_t client, char *topic, uint8_t u8_qos)
{
    esp_mqtt_client_subscribe(client, topic, u8_qos);
}


/* Set callback func */
void vset_get_client_mqtt_callback ( void *cb )
{
    get_client_mqtt_callback = cb;
}

void vset_get_payload_callback ( void *cb )
{
    get_payload_callback = cb;
}

void vset_mqtt_notify_callback( void *cb )
{
    mqtt_client_notify = cb;
}



/*
    msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
    ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);

    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
    printf("DATA=%.*s\r\n", event->data_len, event->data);
*/






