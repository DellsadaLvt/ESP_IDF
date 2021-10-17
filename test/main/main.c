#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"


#include "nvs_flash.h"
#include "nvs.h"

#define STATE_LED       ( 1U << 0U )



static EventGroupHandle_t flag_event_group = NULL;


static void v_task_1( void *arg );
static void v_task_2( void *arg );


void app_main(){
    /* Create event group for flag */
    do{
        flag_event_group = xEventGroupCreate();
    }
    while(flag_event_group == pdFAIL);

    xTaskCreate(v_task_1, "sntp_task", 2048u, NULL, 8u, NULL);
    xTaskCreate(v_task_2, "sntp_task2", 2048u, NULL, 8u, NULL);
    
    while( 1 ){
        printf("set bit\n");
        xEventGroupSetBits(flag_event_group, STATE_LED);
        vTaskDelay(pdMS_TO_TICKS(1000u));
        printf("clear bit\n");
        xEventGroupClearBits(flag_event_group, STATE_LED);
        vTaskDelay(pdMS_TO_TICKS(1000u));
    }
}


static void v_task_1( void *arg )
{
    static EventBits_t temp_uxBits = 10u;
    EventBits_t uxBits;
    while(1){
        uxBits = xEventGroupWaitBits(
            flag_event_group,   /* The event group being tested. */
            STATE_LED, /* The bits within the event group to wait for. */
            pdFALSE,        /* BIT_0 & BIT_4 should be cleared before returning. */
            pdFALSE,       /* Don't wait for both bits, either bit will do. */
            0u );/* Wait a maximum of 100ms for either bit to be set. */

        if( temp_uxBits != uxBits ){
            temp_uxBits = uxBits;
            printf("event bit in task 1: %d\n", temp_uxBits);
        }
        vTaskDelay(pdMS_TO_TICKS(75u));
    }
}


static void v_task_2( void *arg )
{
    static EventBits_t temp_uxBits = 10u;
    EventBits_t uxBits;
    while(1){
        uxBits = xEventGroupWaitBits(
            flag_event_group,   /* The event group being tested. */
            STATE_LED, /* The bits within the event group to wait for. */
            pdFALSE,        /* BIT_0 & BIT_4 should be cleared before returning. */
            pdFALSE,       /* Don't wait for both bits, either bit will do. */
            0u );/* Wait a maximum of 100ms for either bit to be set. */

        if( temp_uxBits != uxBits ){
            temp_uxBits = uxBits;
            printf("event bit in task 2: %d\n", temp_uxBits);
        }
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
}



