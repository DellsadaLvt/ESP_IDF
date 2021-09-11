#include "driver/userGpio.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

/* typedef function pointer */
call_back_func_t call_func= NULL;
static const char *TAG = "GPIO_FUNC";
/*=============================== INTERRUPTS FUNC  ==================================*/
static void gpio_isr_handler(void *arg){
    gpio_num_t gpio_num = (uint32_t) arg;
    call_func( gpio_num );
}

/*=============================== SUBROUNTINE ==================================*/
void vConfig_gpio( gpio_num_t gpio_num, gpio_type_mode_t xMode){
    gpio_config_t io_conf;
    /* reset all value to zero  */
    memset(&io_conf, 0u, sizeof(io_conf));
    /* config pin */
    switch( xMode ){
        case input: 
            /* config gpio input */
            io_conf.intr_type       = GPIO_INTR_DISABLE;
            io_conf.mode            = GPIO_MODE_INPUT;
            io_conf.pin_bit_mask    = (1ULL << gpio_num);
            io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en      = GPIO_PULLUP_ENABLE;
            gpio_config(&io_conf);
            break;
        case output:
            //disable interrupt
            io_conf.intr_type = GPIO_INTR_DISABLE;
            //set as output mode
            io_conf.mode = GPIO_MODE_OUTPUT;
            //bit mask of the pins that you want to set,e.g.GPIO4
            io_conf.pin_bit_mask =(1ULL << gpio_num);;
            //disable pull-down mode
            io_conf.pull_down_en = 0;
            //disable pull-up mode
            io_conf.pull_up_en = 0;
            //configure GPIO with the given settings
            gpio_config(&io_conf);
            break;
        case input_interrupt:
            /* config gpio input */
            io_conf.intr_type       = GPIO_INTR_NEGEDGE;
            io_conf.mode            = GPIO_MODE_INPUT;
            io_conf.pin_bit_mask    = (1ULL << gpio_num);;
            io_conf.pull_down_en    = GPIO_PULLDOWN_DISABLE;
            io_conf.pull_up_en      = GPIO_PULLUP_ENABLE;
            gpio_config(&io_conf);
             //install gpio isr service
            gpio_install_isr_service(0);
            //hook isr handler for specific gpio pin
            gpio_isr_handler_add(gpio_num, gpio_isr_handler, (void *) gpio_num);
            break;
        default:
            ESP_LOGI(TAG, "vConfig_gpio ERROR\n");
            while(1);
    }
}

void vSet_call_back_func( void *cb  ){
    call_func= cb;
}









