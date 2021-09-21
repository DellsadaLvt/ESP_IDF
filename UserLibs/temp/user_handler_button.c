// #include "user_handler_button.h"

// Button_Typedef Button[16];

// /* initial parameter */
// void Button_Begin(gpio_num_t gpio_num){
//    Button[gpio_num].Accumulator= 0u;
//    Button[gpio_num].Count= 20u;
//    //Button[gpio_num].Time= 0u;
//    Button[gpio_num].Status= (BUTTON_STATUS)( READ_BUTTON(gpio_num) + 1 );
//    Button[gpio_num].LastStatus= BUTTON_STATUS_NONE;
// }

// void Button_Process(gpio_num_t gpio_num){
//     if( Button[gpio_num].Count--)
//         Button[gpio_num].Accumulator += (READ_BUTTON(gpio_num) + 1U);
//     else{
//         Button[gpio_num].Count= 19u;
//         Button[gpio_num].Accumulator /= 15u;
//         if( Button[gpio_num].Accumulator != Button[gpio_num].Status ){
//             Button[gpio_num].Status = (BUTTON_STATUS)Button[gpio_num].Accumulator;
//             Button[gpio_num].LastStatus = (BUTTON_STATUS)Button[gpio_num].Status;
//             /* if press down then reset the time */
//             //if( Button[gpio_num].Accumulator == BUTTON_STATUS_FALL )
//                 //Button[gpio_num].Time= 0u;
//         }
//         // else{
//         //     if( Button[gpio_num].Accumulator == 1u ){
//         //          if( Button[gpio_num].Time < 254 ){
//         //              Button[gpio_num].Time++;
//         //          }
//         //      }
//         // }
//         Button[gpio_num].Accumulator = READ_BUTTON(gpio_num) + 1U;
//     }
// }



// unsigned char Button_Check(gpio_num_t gpio_num, BUTTON_STATUS Status, unsigned char Clear){
//     if( Button[gpio_num].LastStatus == Status ){
//         if( Clear ){
//             Button[gpio_num].LastStatus = BUTTON_STATUS_NONE;
//         }
//         return 1;
//     }
//     return 0;
// }



















