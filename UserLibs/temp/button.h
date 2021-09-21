#ifndef _USER_BUTTON_H_
#define _USER_BUTTON_H_


#ifdef __cplusplus
extern "C"{
#endif

#include "driver/gpio.h"

typedef enum{
    BUTTON_STATUS_NONE = 0U,
    BUTTON_STATUS_FALL = 1U,
    BUTTON_STATUS_RISE = 2U,
}BUTTON_STATUS;

// typedef struct{
//     unsigned char Accumulator;
//     unsigned char Count;
//     unsigned char Time;
//     BUTTON_STATUS Status;
//     BUTTON_STATUS  LastStatus;
// }Button_Typedef;

typedef struct{
    unsigned char Value;
    unsigned char Count;
    unsigned char Time;
    BUTTON_STATUS Status;
    BUTTON_STATUS  LastStatus;
}Button_Typedef;


typedef struct{
    unsigned char HoldTime;
    unsigned char MaxHoldTime;
    unsigned char MinHoldTime;
    unsigned char SubStep;
} ButtonAccel_TypeDef;

void Button_Begin(); // reinitial parameter
void Button_Process(); // 4s one time
unsigned char Button_Check(BUTTON_STATUS Status, unsigned char Clear); // check status button
unsigned char Button_Press();
unsigned char Button_Hold(unsigned char Time, unsigned char Once);
unsigned char Button_HoldAccel(ButtonAccel_TypeDef* ButtonAccel);
void Button_Set( BUTTON_STATUS Status, unsigned char Time);

#ifdef __cplusplus
}
#endif

#endif








