#ifndef _BUTTON_H_
#define _BUTTON_H_

typedef enum
{
  BUTTON_STATUS_NONE    = 0,
  BUTTON_STATUS_FALL    = 1,
  BUTTON_STATUS_RISE    = 2,
} BUTTON_STATUS;

//typedef struct
//{
//  unsigned char Accumulator;
//  unsigned char Count;
//  unsigned char Time;
//  BUTTON_STATUS Status;
//  BUTTON_STATUS LastStatus;
//} Button_TypeDef;

typedef struct
{
  unsigned char Value;
  unsigned char Count;
  unsigned char Time;
  BUTTON_STATUS Status;
  BUTTON_STATUS LastStatus;
  
  
} Button_TypeDef;

typedef struct
{
  unsigned char HoldTime;
  unsigned char MaxHoldTime;
  unsigned char MinHoldTime;
  unsigned char SubStep;
} ButtonAccel_TypeDef;

void Button_Begin();
void Button_Process();

unsigned char Button_Check(BUTTON_STATUS Status, unsigned char Clear);
unsigned char Button_Press();
unsigned char Button_Hold(unsigned char Time, unsigned char Once);
unsigned char Button_HoldAccel(ButtonAccel_TypeDef* ButtonAccel);
void Button_Set(BUTTON_STATUS Status, unsigned char Time);

#endif