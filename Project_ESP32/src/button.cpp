#include "button.h"
#include "Arduino.h"

// Button BT1 = Button(34, LOW);
// Button BT2 = Button(32, LOW);
// Button BT3 = Button(33, LOW);
#define BT1_READ digitalRead(34)
#define BT2_READ digitalRead(32)
#define BT3_READ digitalRead(33)
TS_TypeInput    vrts_Button1, vrts_Button2, vrts_Button3;

void Button_Init(void)
{
    pinMode(32, INPUT_PULLUP);
    pinMode(33, INPUT_PULLUP);
    pinMode(34, INPUT_PULLUP);
    vrts_Button1.vruc_FlagChange = 0;
    vrts_Button2.vruc_FlagChange = 0;
    vrts_Button3.vruc_FlagChange = 0;
}

void Button_Readall(void)
{
  Fn_INPUT_ReadInput (BT1_READ, (TS_TypeInput*)(&vrts_Button1));
  Fn_INPUT_ReadInput (BT2_READ, (TS_TypeInput*)(&vrts_Button2));
  Fn_INPUT_ReadInput (BT3_READ, (TS_TypeInput*)(&vrts_Button3));
}

void Fn_INPUT_ReadInput (unsigned char _vruc_Input, TS_TypeInput *_vrts_DataInput){
   _vrts_DataInput->vruc_DataOld = _vruc_Input;
  if(!_vruc_Input){
    if(_vrts_DataInput->vruc_DataOld == _vruc_Input){
      if(_vrts_DataInput->vruc_CountAccess <= MAXINPUT){
        _vrts_DataInput->vruc_CountAccess++;
        if(_vrts_DataInput->vruc_CountAccess == 20){
          _vrts_DataInput->vruc_FlagChange = 1;
        }
      }
    }
    else{
      _vrts_DataInput->vruc_CountAccess = 0;
    }
  }
  else{
    _vrts_DataInput->vruc_CountAccess = 0;
  }
}