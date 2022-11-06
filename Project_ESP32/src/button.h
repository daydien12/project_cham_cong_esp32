#ifndef LIB_BUTTON_h
#define LIB_BUTTON_h

#define MAXINPUT	(1000)

typedef struct{
	unsigned char	vruc_DataOld;
	unsigned int 	vruc_CountAccess;
	unsigned char 	vruc_FlagChange;
    unsigned char   vruc_hold;
}	TS_TypeInput;

extern TS_TypeInput	vrts_Button1, vrts_Button2, vrts_Button3;
void Fn_INPUT_ReadInput (unsigned char _vruc_Input, TS_TypeInput *_vrts_DataInput);
void Button_Init(void);
void Button_Readall(void);
#endif

