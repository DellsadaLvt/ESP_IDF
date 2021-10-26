#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"
#include "freertos/event_groups.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

/* User include */
#include "connect.h"
#include "mqtt.h"

/* Define for event group */
#define TOTAL_MSG_STORE     14U
#define TOPIC_ALL_ACK       15U
#define TOPIC_TEMP_ACK    ( 1U << 0u )
#define TOPIC_HUMID_ACK   ( 1U << 1u )
#define TOPIC_DEMO1_ACK   ( 1U << 2u )
#define TOPIC_DEMO2_ACK   ( 1U << 3u )
#define ENA_TOPIC_TEMP    ( 1U << 4u )
#define ENA_TOPIC_HUMID   ( 1U << 5u)
#define ENA_TOPIC_DEMO1   ( 1U << 6u)
#define ENA_TOPIC_DEMO2   ( 1U << 7u)

static const char *TAG = "MQTT_EXAMPLE";
/* Declare a variable to hold the created event group. */
EventGroupHandle_t xEvent_flag;
/* Init queue */
QueueHandle_t xQueue_send_mqttClient = NULL;    
QueueHandle_t xQueue_mqtt_data = NULL;

/* Prototype functions */
static void init_log_level_set ( void );
static void init_flash( void );
static void delay( uint16_t u16_time);
static void vsend_data_processor_task(char *data, char *token);
static void handler_payload( char *data_in, uint16_t u16_len_datain, char *data_out );
/* Callback func */
static void vget_payload_callback(char *topic, uint16_t u16_topic_len, char *data, uint16_t u16_data_len);
static void vget_client_mqtt_callback( esp_mqtt_client_handle_t client );
static void v_mqtt_notify_callback( mqtt_event_status_t status );
/* Task hanlder */
static void v_mqtt_task_handler ( void *arg );
static void v_processor_task( void *arg );


void app_main(void)
{
    /* Init log set level */
    init_log_level_set();

    /* Init Flash */
    init_flash();

    /* Create queue */
    xQueue_send_mqttClient = xQueueCreate(1u, 4u);
    xQueue_mqtt_data = xQueueCreate(20u, 8u);

    /* Attempt to create the event group. */
    xEvent_flag = xEventGroupCreate();
    xEventGroupClearBits(xEvent_flag, TOPIC_TEMP_ACK|TOPIC_HUMID_ACK|TOPIC_DEMO2_ACK|TOPIC_DEMO1_ACK);
   
    /* Init */
    vset_get_client_mqtt_callback( vget_client_mqtt_callback );
    vset_get_payload_callback( vget_payload_callback );
    vset_mqtt_notify_callback( v_mqtt_notify_callback );

    /* connect to ap */
    wifi_init_sta();

    /* Start MQTT */
    /* Config mqtt client */
    esp_mqtt_client_config_t mqtt_cfg = {
        //.uri = "mqtt://broker.emqx.io:1883/",
        .host = "192.168.1.9",
        .port = 1883u,
        // .username = "admin",
        // .password = "123456",
        .client_id = "esp_node_temp",
        .lwt_topic = "status/esp",
        .lwt_msg = "0", 
        .lwt_msg_len = 0u,
        .lwt_qos = 1u,
        .lwt_retain = true,
        .keepalive = 3u,
        .disable_clean_session = true,
    };
    mqtt_start(&mqtt_cfg);

    /* Create a task handler */
    xTaskCreate(v_mqtt_task_handler, "task-1", 2048u, NULL, 10u, NULL);
    xTaskCreate(v_processor_task, "task_processor", 2048u, NULL, 10u, NULL );
}



/*=================== SUBROUNTINE FUNCTION ====================*/
static void v_mqtt_task_handler ( void *arg )
{
    esp_mqtt_client_handle_t client = NULL;
    uint8_t u8_enable_pulish = 0u, count= 0;
    char buff[4u];
    while(1){
        /* receive connected ack */
        if(pdFAIL !=  xQueueReceive(xQueue_send_mqttClient, &client, 0u) ){
            printf("Connect server Done\n");
            mqtt_client_publish(client, "status/esp", "1", 0u, 1u, 1u);
            mqtt_client_subscribe(client, "esp/demo1", 1u);
            mqtt_client_subscribe(client, "esp/demo2", 1u);
            mqtt_client_subscribe(client, "esp/temp", 1u);
            mqtt_client_subscribe(client, "esp/humid", 1u);
            /* enable publish */
            u8_enable_pulish = 1u;
        }
        
        if( u8_enable_pulish ){
            count++;
            sprintf(buff, "%d", count);
            mqtt_client_publish(client, "esp/demo", buff, 0u, 1u, 1u);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000u));
    }
}

static void v_processor_task( void *arg )
{
    char *token= NULL, key, buff[10u];
    float temp, humid, demo1;
    while(1){
        /* Receive data handler */
        if( pdFAIL != xQueueReceive(xQueue_mqtt_data, buff, 0u)){
            // printf("The data receive =-----> : %s, sizeof data: %d\n", buff, strlen(buff));
            key = *(buff+strlen(buff)-1);
            if( key == 't' ){
                token = strtok(buff, "t");
                temp = atof(token);
                printf("Temp in float-----> %f, temp in string: %s\n", temp, token);
            }
            else if( key == 'h' ){
                token = strtok(buff, "h");
                humid = atof(token);
                printf("Humid is ---->: %f\n", humid);
            }
            else if( key == 'a' ){
                token = strtok(buff, "a");
                demo1 = atof(token);
                printf("Demo1 is:-----> %f\n", demo1);
            }
            else{
                printf("no need read----> \n");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(500u));
    }
}


static void init_log_level_set ( void )
{
    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("MQTT_EXAMPLE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_TCP", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_SSL", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);
}

static void init_flash( void )
{
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

static void vget_client_mqtt_callback( esp_mqtt_client_handle_t client )
{
    /* Send client inf */
    xQueueSendToFront(xQueue_send_mqttClient, &client, 0u);
}

static void vget_payload_callback(char *topic, uint16_t u16_topic_len, char *data, uint16_t u16_data_len)
{
    static char temp[8u], humid[8u], demo1[8u], demo2[8u], flag = 0u, count = 0;

    if( count <= TOTAL_MSG_STORE ){
        count++;
        printf("The count: %d, the flag: %d\n", count, flag);
    }

    if( (0u == strncmp(topic, "esp/temp", 8u)) && ( !((flag&0xF0)&ENA_TOPIC_TEMP) || ((flag&0x0F) == TOPIC_ALL_ACK) ) ){
        flag |= TOPIC_TEMP_ACK;
        if( count > TOTAL_MSG_STORE ){
            flag |= ENA_TOPIC_TEMP;
        }
        handler_payload(data, u16_data_len, temp);
        vsend_data_processor_task(temp, "t");
        // printf("The temp: %s\n", temp);
    }
    else if( (0u == strncmp(topic, "esp/humid", 9u)) && ( !((flag&0xF0)&ENA_TOPIC_HUMID) || ( (flag&0x0F) == TOPIC_ALL_ACK)) ){
        flag |= TOPIC_HUMID_ACK;
        if( count > TOTAL_MSG_STORE ){
            flag |= ENA_TOPIC_HUMID;
        }
        handler_payload(data, u16_data_len, humid);
        vsend_data_processor_task(humid, "h");
        // printf("The humid: %s\n", humid);
    }
    else if( (0u == strncmp(topic, "esp/demo1", 9u)) && ( !((flag&0xF0)&ENA_TOPIC_DEMO1) || ( (flag&0x0F) == TOPIC_ALL_ACK)) ){
        flag |= TOPIC_DEMO1_ACK;
        if( count > TOTAL_MSG_STORE ){
            flag |= ENA_TOPIC_DEMO1;
        }
        handler_payload(data, u16_data_len, demo1);
        vsend_data_processor_task(demo1, "a");
        // printf("The demo1: %s\n", demo1);
    }
    else if( (0u == strncmp(topic, "esp/demo2", 9u)) && ( !((flag&0xF0)&ENA_TOPIC_DEMO2) || ( (flag&0x0F) == TOPIC_ALL_ACK))  ){
        flag |= TOPIC_DEMO2_ACK;
        if( count > TOTAL_MSG_STORE ){
            flag |= ENA_TOPIC_DEMO2;
        }
        handler_payload(data, u16_data_len, demo2);
        vsend_data_processor_task(demo2, "b");
        // printf("The demo2: %s\n", demo2);
    }
    
}

static void v_mqtt_notify_callback( mqtt_event_status_t status )
{
    if( status == disconnected ){
        printf("mqtt disconnected\n");
    }
    else if( status == err ){
        printf("mqtt err\n");
    }
}

static void handler_payload( char *data_in, uint16_t u16_len_datain, char *data_out )
{
    memset(data_out, '\0', u16_len_datain +1u);
    strncpy(data_out, data_in, u16_len_datain);
    // printf("Data in callback: %s\n", data_out);
}

static void vsend_data_processor_task(char *data, char *token)
{
    printf("-----------> Data received: %s\n", data);
    strcat(data, token);
    xQueueSendToBack(xQueue_mqtt_data, data, 0u);
}

static void delay( uint16_t u16_time)
{
    uint32_t u32_tick_set = pdMS_TO_TICKS(u16_time);
    uint32_t u32_tick_curr = xTaskGetTickCount();
    while(xTaskGetTickCount() < (u32_tick_curr + u32_tick_set));
}



