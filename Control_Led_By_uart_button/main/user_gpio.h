#ifndef _USER_GPIO_H_
#define _USER_GPIO_H_

#ifdef __cplusplus
extern "C" {
#endif

// #include <stdbool.h>
// #include "esp_err.h"
// #include "esp_log.h"
//#include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/queue.h"
#include "driver/gpio.h"
#include <string.h>

typedef enum{
    gpio_output     = 0u,
    gpio_input      = 1u,
    gpio_input_isr  = 2u,
}user_gpio_mode_t;

typedef void (*gpio_callback_t ) ( gpio_num_t );

void v_set_gpio_callback( void *cb );
void v_gpio_config(user_gpio_mode_t gpio_mode, gpio_config_t *io_conf, gpio_num_t gpio_num_isr);
void v_gpio_conf_input(void);

#ifdef __cplusplus
}
#endif

#endif // _DRIVER_GPIO_H_
