#ifndef _USER_BUTTON_CONFIG_H_
#define _USER_BUTTON_CONFIG_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "driver/gpio.h"

/* config for button */
#define READ_BUTTON()       (gpio_get_level(GPIO_NUM_13))



#ifdef __cplusplus
}
#endif

#endif /* _USER_BUTTON_CONFIG_H_ */