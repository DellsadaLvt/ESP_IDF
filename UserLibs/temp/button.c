#include "button.h"
#include "button_config.h"

Button_Typedef Button;

// reinitial parameter
// void Button_Begin(){
//    Button.Accumulator= 0u;
//    Button.Count= 5u;
//    Button.Time= 0u;
//    Button.Status= (BUTTON_STATUS)( READ_BUTTON() + 1 );
//    Button.LastStatus= BUTTON_STATUS_NONE;
// }

// void Button_Process(){
//     if( Button.Count--)
//         Button.Accumulator += READ_BUTTON() + 1U;
//     else{
//         Button.Count= 4u;
//         Button.Accumulator /= 4u;
//         if( Button.Accumulator != Button.Status ){
//             Button.Status = (BUTTON_STATUS)Button.Accumulator;
//             Button.LastStatus = (BUTTON_STATUS)Button.Status;
//             /* if press down then reset the time */
//             if( Button.Accumulator == BUTTON_STATUS_FALL )
//                 Button.Time= 0u;
//         }
//         else{
//              if( Button.Accumulator == 1u ){
//                  if( Button.Time < 254 ){
//                      Button.Time++;
//                  }
//              }
//         }
//         Button.Accumulator = READ_BUTTON() + 1U;
//     }
// }

static BUTTON_STATUS GetStatus(){
    // unsigned char count= 0u;
    // for( unsigned char i= 0u; i< 5u; i++){
    //     if( Button.Value&(1u<<i))
    //         count++;
    // }
  
    // static const BUTTON_STATUS code[]= {};
    // return code[Button.Value & 0x1F];

    return Button.Count >= 4 ? BUTTON_STATUS_RISE : BUTTON_STATUS_FALL;
}


void Button_Begin(){
    Button.Value = 0u;
    Button.Count = 0u;
    for( char i= 0u; i< 5u; i++){
        unsigned char read = READ_BUTTON();
        Button.Value <<= 1u;
        Button.Value  |= read;
        Button.Count  += read;
    }
    Button.Time= 0u;
    Button.Status= GetStatus();
    Button.LastStatus = BUTTON_STATUS_NONE;
}


void Button_Process(){
    unsigned char read = READ_BUTTON();
    Button.Value <<= 1u;
    Button.Value  |= read;
    Button.Count -= !!(Button.Value & (1 << 5u));
    Button.Count  += read;

    BUTTON_STATUS status = GetStatus();
    if( status != Button.Status ){
        Button.Status = (BUTTON_STATUS)status;
        Button.LastStatus = (BUTTON_STATUS)Button.Status;
        /* if press down then reset the time */
        if( status == BUTTON_STATUS_FALL )
            Button.Time= 0u;
    }
    else{
            if( status == 1u ){
                if( Button.Time < 254 ){
                    Button.Time++;
                }
            }
    }
}


unsigned char Button_Check(BUTTON_STATUS Status, unsigned char Clear){
    if( Button.LastStatus == Status ){
        if( Clear ){
            Button.LastStatus = BUTTON_STATUS_NONE;
        }
        return 1;
    }
    return 0;
}
unsigned char Button_Press(){
    /* after pressed button then "nha nut nhan" check if button is high then reset last status */
    /* assum that: the void button process call each 200ms, 
        if press 20ms ==> time = 10 --> accept press button  */
    if( Button.LastStatus == BUTTON_STATUS_RISE && Button.Time < 10u){
        Button.LastStatus = BUTTON_STATUS_NONE;
        return 1;
    }
    return 0; 
}
unsigned char Button_Hold(unsigned char Time, unsigned char Once){
    if( (Button.LastStatus == BUTTON_STATUS_FALL) && ( Button.Time > Time)){
        if(Once){
            Button.Time= 255;
        }
        else{
            Button.Time= 10u;   
        }
        return 1;
    }
    return 0;
}
unsigned char Button_HoldAccel(ButtonAccel_TypeDef* ButtonAccel){
    if(Button.LastStatus == BUTTON_STATUS_RISE)
        ButtonAccel->HoldTime = ButtonAccel->MaxHoldTime;
    else{
        if( Button.Time > ButtonAccel->HoldTime){
            if( ButtonAccel->HoldTime == ButtonAccel->MaxHoldTime){
                ButtonAccel->HoldTime = ButtonAccel->MaxHoldTime + 1 + 10;
            }
            else if( ButtonAccel->HoldTime > (ButtonAccel->MinHoldTime + ButtonAccel->SubStep) ){
                ButtonAccel->HoldTime -= ButtonAccel->SubStep;
            }
            Button.Time= 10u;
            return 1;
        }
    }
    return 0;
}
void Button_Set( BUTTON_STATUS Status, unsigned char Time){
    // Button.Status= Status;
    // Button.Time = Time;
    // Button.Count = 5u;
    // Button.Accumulator = 0u;
}

















