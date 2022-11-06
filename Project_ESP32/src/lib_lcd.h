#ifndef LIB_LCD_h
#define LIB_LCD_h
#include <LIB/LiquidCrystal_I2C.h>

extern uint8_t lcd_display_infor_user_flag;

void LCD_Init(void);
void LCD_Update_Date_Time(uint8_t row, uint8_t column,char *arr);
void LCD_Mode_Setting_Init(void);
void LCD_Mode_Setting_Run(uint8_t mode);

void LCD_Mode_Add_New_Init(void);
void LCD_Mode_Add_New_Run(uint8_t mode, uint8_t uid);

void LCD_Mode_Delete_All_Init(void);
void LCD_Mode_Delete_All_Run(uint8_t mode);

void LCD_Mode_Delete_Uid_Init(void);
void LCD_Mode_Delete_Uid_Run(void);

void LCD_Display_Infor_User(uint8_t mode,uint8_t uid,  char *arr_rfid, char *arr_name);

void LCD_Mode_Back(void);
#endif

