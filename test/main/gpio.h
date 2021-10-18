#ifndef __GPIO_H
#define __GPIO_H


#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/pwm.h"
#include "driver/gpio.h"
#include "driver/ledc.h"



// #define button    D1         // D1 is gpio 5
// #define triac     D2          // D2 is gpio 4
#define BUTTON       (GPIO_NUM_5)
#define LED          (GPIO_NUM_4)
#define LED_ON          (1)
#define LED_OFF         (0)

/* For lec modul */
#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE

/* Callback function */
typedef void (*input_handler_callback) (void);
typedef void (*test_isr_callback)( uint32_t a, uint32_t b, uint32_t c);

void config_gpio_led_status(void);
void config_led_pin(void);
void v_turn_off_led( void );
void v_turn_on_led( void );
void v_ledc_init( void );
void v_ledc_fade_set_duty(uint8_t ch, uint16_t duty, uint16_t time_du );
void config_button(void);
void set_input_isr_callback(void *cb);
void v_reset_key_button(void);

/* test */
void set_test_function_callback( void *cb);

#endif

