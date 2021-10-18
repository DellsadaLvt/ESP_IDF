#include "user_button.h"


Button_TypeDef Button;
static BUTTON_STATUS GetStatus()
{
//  unsigned char count = 0;
//  for (unsigned char i = 0; i < 5; i++)
//  {
//    if (Button.Value & (1 << i))
//      count++;
//  }
//  if (count >= 4)
//    return BUTTON_STATUS_RISE;
//  return BUTTON_STATUS_FALL;
  
//  static const BUTTON_STATUS code[] = {};
//  return code[Button.Value & 0x1F];
  
  return Button.Count >= 4 ? BUTTON_STATUS_RISE : BUTTON_STATUS_FALL;
  
}

void Button_Begin()
{
  Button.Value = 0;
  Button.Count = 0;
  for (unsigned char i = 0; i < 5; i++)
  {
    unsigned char read = READ_BUTTON();
    Button.Value <<= 1;
    Button.Value |= read;
    Button.Count += read;
  }
  Button.Time = 0;
  Button.Status = GetStatus();
  Button.LastStatus = BUTTON_STATUS_NONE;
}
void Button_Process()
{
  unsigned char read = READ_BUTTON();
  Button.Value <<= 1;
  Button.Value |= read;
  Button.Count -= !!(Button.Value & (1 << 5));
  Button.Count += read;
  
  BUTTON_STATUS status = GetStatus();
  if (status != Button.Status)
  {
    Button.Status = (BUTTON_STATUS)status;
    Button.LastStatus = Button.Status;
    if (status == BUTTON_STATUS_FALL)
      Button.Time = 0;
  }
  else
  {
    if (status == 1)
    {
      if (Button.Time < 254)
        Button.Time++;
    }
  }
}

//void Button_Begin()
//{
//  Button.Accumulator = 0;
//  Button.Count = 5;
//  Button.Time = 0;
//  Button.Status = (BUTTON_STATUS)(READ_BUTTON() + 1);
//  Button.LastStatus = BUTTON_STATUS_NONE;
//}
//void Button_Process()
//{
//  if (Button.Count--)
//    Button.Accumulator += READ_BUTTON() + 1;
//  else
//  {
//    Button.Count = 4;
//    Button.Accumulator /= 4;
//    if (Button.Accumulator != Button.Status)
//    {
//      Button.Status = (BUTTON_STATUS)Button.Accumulator;
//      Button.LastStatus = Button.Status;
//      if (Button.Accumulator == BUTTON_STATUS_FALL)
//        Button.Time = 0;
//    }
//    else
//    {
//      if (Button.Accumulator == 1)
//      {
//        if (Button.Time < 254)
//          Button.Time++;
//      }
//    }
//    Button.Accumulator = READ_BUTTON() + 1;
//  }
//}
unsigned char Button_Check(BUTTON_STATUS Status, unsigned char Clear)
{
  if (Button.LastStatus == Status)
  {
    if (Clear)
      Button.LastStatus = BUTTON_STATUS_NONE;
    return 1;
  }
  return 0;
}
unsigned char Button_Press()
{
  if (Button.LastStatus == BUTTON_STATUS_RISE && Button.Time < 10)
  {
    Button.LastStatus = BUTTON_STATUS_NONE;
    return 1;
  }
  return 0;
}
unsigned char Button_Hold(unsigned char Time, unsigned char Once)
{
  if (Button.LastStatus == BUTTON_STATUS_FALL && Button.Time > Time)
  {
    if (Once)
      Button.Time = 255;
    else
      Button.Time = 10;
    return 1;
  }
  return 0;
}
unsigned char Button_HoldAccel(ButtonAccel_TypeDef* ButtonAccel)
{
  if (Button.LastStatus == BUTTON_STATUS_RISE)
    ButtonAccel->HoldTime = ButtonAccel->MaxHoldTime;
  else
  {
    if (Button.Time > ButtonAccel->HoldTime)
    {
      if (ButtonAccel->HoldTime == ButtonAccel->MaxHoldTime)
        ButtonAccel->HoldTime = ButtonAccel->MaxHoldTime + 1 + 10;
      else if (ButtonAccel->HoldTime > (ButtonAccel->MinHoldTime + ButtonAccel->SubStep))
        ButtonAccel->HoldTime -= ButtonAccel->SubStep;
      Button.Time = 10;
      return 1;
    }
  }
  return 0;
}
void Button_Set(BUTTON_STATUS Status, unsigned char Time)
{
//  Button.Status = Status;
//  Button.Time = Time;
//  Button.Count = 5;
//  Button.Accumulator = 0;
}