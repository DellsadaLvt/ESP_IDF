#ifndef __USER_GPIO_H
#define __USER_GPIO_H


#include "driver/pwm.h"
#include "driver/gpio.h"

/* config for pwm */
// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD    (1000u)


/* Prototypes function */
void config_pwm( void );
void config_gpio(void);


#endif




