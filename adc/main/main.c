
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"

static const char *TAG = "ADC: ";

static void adc_task()
{
    int x;
    uint16_t adc_data[10];

    while (1) {
        if (ESP_OK == adc_read(&adc_data[0])) {
            ESP_LOGI(TAG, "adc read: %d\r\n", adc_data[0]);
        }

        ESP_LOGI(TAG, "adc read fast:\r\n");

        if (ESP_OK == adc_read_fast(adc_data, 10)) {
            for (x = 0; x < 10; x++) {
                printf("%d\n", adc_data[x]);
            }
        }

        vTaskDelay(1000 / portTICK_RATE_MS);
    }
}

void app_main()
{
    // 1. init adc
    adc_config_t adc_config;

    // Depend on menuconfig->Component config->PHY->vdd33_const value
    // When measuring system voltage(ADC_READ_VDD_MODE), vdd33_const must be set to 255.
    adc_config.mode = ADC_READ_TOUT_MODE;       // adc 0 pin
    adc_config.clk_div = 8;                     // ADC sample collection clock = 80MHz/clk_div = 10MHz
    ESP_ERROR_CHECK(adc_init(&adc_config));

    // 2. Create a adc task to read adc value
    xTaskCreate(adc_task, "adc_task", 1024, NULL, 5, NULL);
}
