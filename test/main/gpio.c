#include "gpio.h"

//static const char *LEDC_TAG = "------> gpio.c: ";


/* Static values */
static uint8_t key_button = 1u;
input_handler_callback  input_isr_callback  = NULL;

/* Ledc config */
static ledc_channel_config_t ledc_channel[] = {
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

/*================= static functions =================*/
static void v_filter_button(void){
    /* handler button for falling edge */
    if( (gpio_get_level(BUTTON) == 0u) && (key_button)){
        key_button = 0u;
        input_isr_callback();   
    }

    /* handler button for rising edge */
    if( (gpio_get_level(BUTTON) == 1u) && (key_button)){
        key_button = 0u;
        input_isr_callback();
    }
}

static void v_gpio_isr_handler(void *arg)
{
    if(arg == (void *)BUTTON){
        v_filter_button();
    }
}



/*==================== Call function =================*/
void config_gpio_led_status(void)
{
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

void config_led_pin(void)
{
    /* config gpio input gpio2: d4 */
    gpio_config_t io_conf;
    io_conf.intr_type= GPIO_INTR_DISABLE;
    io_conf.mode= GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask= (1UL << LED);
    io_conf.pull_down_en= GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en= GPIO_PULLUP_DISABLE;
    gpio_config(&io_conf);
    /* init first state */
    gpio_set_level(LED, 0u);
}

void v_ledc_init( void )
{
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty: 0 --> 8191
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HS_MODE,           // timer mode
        .timer_num = LEDC_HS_TIMER            // timer index
    };
    // Set configuration of timer0 for high speed channels
    ledc_timer_config(&ledc_timer);

    // Set LED Controller with previously prepared configuration
    for (uint8_t ch = 0; ch < 3; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    // Initialize fade service.
    ledc_fade_func_install(0);

    /* Init first status */
    for (uint8_t ch = 0; ch < 3u; ch++) {
        ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0u, 1000u);
        ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);
    }

}

void v_ledc_fade_set_duty(uint8_t ch, uint16_t duty, uint16_t time_du )
{
    ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, duty, time_du);
    ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_NO_WAIT);   
 
}

void v_turn_on_led( void )
{
    gpio_set_level(LED, LED_ON);
}

void v_turn_off_led( void )
{
    gpio_set_level(LED, LED_OFF);
}

void config_button(void)
{
    gpio_config_t io_conf={0u};
    io_conf.intr_type= GPIO_INTR_ANYEDGE;
    io_conf.mode= GPIO_MODE_INPUT;
    io_conf.pin_bit_mask= (1UL << BUTTON);
    /* NOTE: need pull up gpio button to avoid noise */
    io_conf.pull_down_en= GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en= GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    /* config isr */
    //gpio_install_isr_service(0u);
    /* add isr gpio pin 13: D7 */
    //gpio_isr_handler_add(BUTTON, v_gpio_isr_handler, (void*)BUTTON);
    
}

void v_reset_key_button(void)
{
    key_button = 1u;
}


/* callback func*/
void set_input_isr_callback(void *cb)
{
    input_isr_callback = cb;
}


// static void delay_ms( uint32_t u32Time_ms){
// 	uint32_t u32Tick_set, u32Current_tick;
// 	/* convert ms to tick */
// 	u32Tick_set= pdMS_TO_TICKS(u32Time_ms);
// 	u32Current_tick= xTaskGetTickCountFromISR();
// 	/* block loop while */
// 	while( xTaskGetTickCountFromISR() < ( u32Current_tick + u32Tick_set ));
// }



