#ifndef _USER_GPIO_H
#define _USER_GPIO_H

#pragma once
/*#include <stdint.h>*/
#include "driver/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

/* pin mode enum */
typedef enum{
    input = 0u,
    output = 1u, 
    input_interrupt= 3u
} gpio_type_mode_t;

/* use for call back func */
typedef void (*call_back_func_t)(gpio_num_t);


/*=============================== FUNC PROTOTYPE ==================================*/
void vConfig_gpio( gpio_num_t gpio_num, gpio_type_mode_t xMode);
void vSet_call_back_func( void *cb  );

#ifdef __cplusplus
}
#endif 



#endif /* _USER_GPIO_H */

