#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_spi_flash.h"


#include "nvs_flash.h"
#include "nvs.h"

#include "user_button.h"
#include "gpio.h"

volatile char xx= 10u;
TimerHandle_t xTimer1 = NULL;

static void v_task_1( void *arg );
static void timer_callback_handler( TimerHandle_t xTimer );


void app_main(){
    config_led_pin();
    config_button();
    Button_Begin();

    xTimer1 = xTimerCreate("Timer_1", pdMS_TO_TICKS(100u), pdTRUE, (void *const)0u, timer_callback_handler);       // after use, timer will delete
    xTimerStart(xTimer1, portMAX_DELAY);

    xTaskCreate(v_task_1, "sntp_task", 2048u, NULL, 8u, NULL);
    
}


static void v_task_1( void *arg )
{
    static char temp = 20u;
    while(1){
        if(temp != xx ){
            temp = xx;
            if( temp == 2 ){
                printf("button rise\n");
            }
            else if( temp == 3 ){
                printf("button falling\n");
            }
        }
        vTaskDelay(pdMS_TO_TICKS(100u));
    }
}


static void timer_callback_handler( TimerHandle_t xTimer )
{
    if( xTimer == xTimer1 ){
        Button_Process();
        if (Button_Check(BUTTON_STATUS_RISE, 0))
        {
            xx = 2;
        }
        else if (Button_Check(BUTTON_STATUS_FALL, 0))
        {
            xx = 3;
        }
    }
}





