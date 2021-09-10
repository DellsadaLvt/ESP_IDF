#ifndef INPUT_IO_H

#define INPUT_IO_H

#include "esp_err.h"
//#include "hal/gpio_types.h"  using for stm32

typedef enum{
    LO_TO_HI= 1U,
    HI_TO_LO= 2U,
    ANY_EDGE= 3U
}interrupt_type_edge_t;

//typedef (*input_callback_t) (int);

//void input_io_create( gpio_num_t gpio_num, interrupt_type_edge_t level );
int input_io_get_level( gpio_num_t gpio_num);
//void input_set_callback( void * cb );


#endif



