#ifndef __USER_GPIO_H
#define __USER_GPIO_H


#include "esp_log.h"
#include "driver/pwm.h"
#include "driver/gpio.h"
#include "driver/ledc.h"

/* config for pwm */
// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD             (1000u)
#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE

/* Prototypes function */
void config_pwm( void );
void config_gpio_output(void);
void v_ledc_init( void );
void v_ledc_fade_set_duty(uint8_t channel, uint16_t duty, uint16_t time_du );

#endif




