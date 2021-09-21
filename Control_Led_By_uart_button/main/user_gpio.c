#include "user_gpio.h"

gpio_callback_t gpio_callback= NULL;
static void gpio_isr_handler( void *gpio_num_isr );

void v_gpio_config(user_gpio_mode_t gpio_mode, gpio_config_t *io_conf, gpio_num_t gpio_num_isr){
    // init gpio
    gpio_config(io_conf);
    if( gpio_mode == gpio_input_isr ){
        //install gpio isr service
        gpio_install_isr_service(0);
        //hook isr handler for specific gpio pin
        gpio_isr_handler_add(gpio_num_isr, gpio_isr_handler, (void *) gpio_num_isr);
    }
}



static void gpio_isr_handler( void *gpio_num_isr ){
    gpio_callback( (gpio_num_t)gpio_num_isr );
}


void v_set_gpio_callback( void *cb ){
    gpio_callback= cb;
}


void v_gpio_conf_input(void){
    gpio_config_t io_conf;
    /* reset all value to zero  */
    memset(&io_conf, 0u, sizeof(io_conf));
    /* config pin */
    /* config gpio input */
    io_conf.intr_type       = GPIO_INTR_DISABLE;
    io_conf.mode            = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask    = (1ULL << GPIO_NUM_5);
    io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en      = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
}








