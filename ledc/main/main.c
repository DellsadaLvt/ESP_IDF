#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"

#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE

static const char* LEDC_TAG = "main.c: ";

void app_main(){
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty: 0 --> 8191
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER            // timer index
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel[] = {
        {
            .channel    = LEDC_CHANNEL_0,
            .duty       = 0,
            .gpio_num   = GPIO_NUM_12,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
        {
            .channel    = LEDC_CHANNEL_1,
            .duty       = 0,
            .gpio_num   = GPIO_NUM_13,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
        {
            .channel    = LEDC_CHANNEL_2,
            .duty       = 0,
            .gpio_num   = GPIO_NUM_14,
            .speed_mode = LEDC_HS_MODE,
            .hpoint     = 0,
            .timer_sel  = LEDC_HS_TIMER
        },
    };

    int ch;

    // Set LED Controller with previously prepared configuration
    for (ch = 0; ch < 3; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    ledc_fade_func_install(0);

    while (1) {
        ESP_LOGE(LEDC_TAG, "led up\n");
        for (ch = 0; ch < 3u; ch++) {
            //ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0);
            //ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
            ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 8000u, 1000u);
            ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
        }
        vTaskDelay(pdMS_TO_TICKS(5000u));

        ESP_LOGE(LEDC_TAG, "led down\n");
        for (ch = 0; ch < 3u; ch++) {
            //ledc_set_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0);
            //ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);
            ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 2000u, 1000u);
            ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
        }
        vTaskDelay(pdMS_TO_TICKS(5000u));
    }
}