#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "user_json.h"

void app_main(void){
    char buff[50u]= "{\"one\":\"1\",\"two\":\"2\", \"three\":\"3\"}";
    printf("Json data: %s\n", buff);
    char data[10u];
    parse_json(buff, "three", data);
    printf("The data parsing: %s\n", data);
    while(1){
        vTaskDelay(pdMS_TO_TICKS(10000u));
    }
}



