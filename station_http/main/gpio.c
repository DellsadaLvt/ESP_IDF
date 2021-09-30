#include "gpio.h"

// pwm pin number
static const uint32_t pin_num[3] = {
    GPIO_NUM_12, 
    GPIO_NUM_13,
    GPIO_NUM_14
};

// duties table, real_duty = duties[x]/PERIOD
static uint32_t duties[3] = { 
    500, 500, 500
};

// phase table, delay = (phase[x]/360)*PERIOD
static float phase[3] = {
    0.0, 0.0, 0.0
};

void config_gpio(void){
    /* config gpio input gpio2: d4 */
    gpio_config_t io_conf;
    io_conf.intr_type= GPIO_INTR_DISABLE;
    io_conf.mode= GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask= (GPIO_Pin_2);
    io_conf.pull_down_en= GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en= GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    /* init first state */
    gpio_set_level(GPIO_NUM_2, 0u);
}


void config_pwm( void ){
    pwm_init(PWM_PERIOD, duties, 3, pin_num);
    pwm_set_phases(phase);
    pwm_start();
}












