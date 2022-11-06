#include "lib_lcd.h"

/*----------------------|LCD|----------------------*/
LiquidCrystal_I2C lcd(0x3F, 20, 4);
uint8_t lcd_display_infor_user_flag, flag_update_time = 0;
void LCD_Init(void)
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("Thiet bi cham cong");
}

void LCD_Update_Date_Time(uint8_t row, uint8_t column,char *arr)
{
  if(flag_update_time == 1 && lcd_display_infor_user_flag == 0)
  {
    lcd.clear();
    lcd.setCursor(1,0);
    lcd.print("Thiet bi cham cong");
    flag_update_time = 0;
  }
  lcd.setCursor(row,column);
  lcd.print(arr);
}

void LCD_Mode_Setting_Init(void)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(" Them van tay moi");
    lcd.setCursor(0, 1);
    lcd.print(" Xoa 1 Van tay");
    lcd.setCursor(0, 2);
    lcd.print(" Xoa toan bo van tay");
    lcd.setCursor(0, 3);
    lcd.print(" Back");
}

void LCD_Mode_Setting_Run(uint8_t mode)
{
    if(mode == 1)
    {
        lcd.setCursor(0, 0);
        lcd.print(">Them van tay moi");
        lcd.setCursor(0, 1);
        lcd.print(" Xoa 1 Van tay");
        lcd.setCursor(0, 2);
        lcd.print(" Xoa toan bo van tay");
        lcd.setCursor(0, 3);
        lcd.print(" Back");
    }
    else if(mode == 2)
    {
        lcd.setCursor(0, 0);
        lcd.print(" Them van tay moi");
        lcd.setCursor(0, 1);
        lcd.print(">Xoa 1 Van tay");
        lcd.setCursor(0, 2);
        lcd.print(" Xoa toan bo van tay");
        lcd.setCursor(0, 3);
        lcd.print(" Back");
    }
    else if(mode == 3)
    {
        lcd.setCursor(0, 0);
        lcd.print(" Them van tay moi");
        lcd.setCursor(0, 1);
        lcd.print(" Xoa 1 Van tay");
        lcd.setCursor(0, 2);
        lcd.print(">Xoa toan bo van tay");
        lcd.setCursor(0, 3);
        lcd.print(" Back");
    }
    else if(mode == 4)
    {
        lcd.setCursor(0, 0);
        lcd.print(" Them van tay moi");
        lcd.setCursor(0, 1);
        lcd.print(" Xoa 1 Van tay");
        lcd.setCursor(0, 2);
        lcd.print(" Xoa toan bo van tay");
        lcd.setCursor(0, 3);
        lcd.print(">Back");
    }
}

void LCD_Mode_Back(void)
{
    lcd.clear();
    lcd.print("Thiet bi cham cong");
}

void LCD_Mode_Add_New_Init(void)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dat tay len cam bien");
    lcd.setCursor(0, 1);
    lcd.print("Giu tay tai vi tri");
}

void LCD_Mode_Add_New_Run(uint8_t mode, uint8_t uid)
{
   if(mode == 2)
    {
        lcd.setCursor(0, 0);
        lcd.print("Dat tay len cam bien");
        lcd.setCursor(0, 1);
        lcd.print("Giu tay tai vi tri");
        lcd.setCursor(0, 2);
        lcd.print("Luu van tay...");
    }
    else if(mode == 3)
    {
        char temp_arr[20];
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Da them van tay moi");
        sprintf(temp_arr, "UID: %d", uid);
        lcd.setCursor(0, 1);
        lcd.print(temp_arr);
    }
    else if(mode == 4)
    {
        char temp_arr[20];
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Them van tay loi!");
    }
}

void LCD_Mode_Delete_All_Init(void)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Xoa toan bo cam bien");
}

void LCD_Mode_Delete_All_Run(uint8_t mode)
{
    if(mode == 1)
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Xoa toan bo cam bien");
        lcd.setCursor(5, 1);
        lcd.print("Thanh cong!");
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Xoa toan bo cam bien");
        lcd.setCursor(5, 1);
        lcd.print("That bai!");
    }
}

void LCD_Mode_Delete_Uid_Init(void)
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Dat tay len cam bien");
    lcd.setCursor(0, 1);
    lcd.print("Giu tay tai vi tri");
}
void LCD_Mode_Delete_Uid_Run(void)
{
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("Xoa van tay");
    lcd.setCursor(5, 1);
    lcd.print("Thanh cong!");
}


void LCD_Display_Infor_User(uint8_t mode,uint8_t uid,  char *arr_rfid, char *arr_name)
{
    lcd_display_infor_user_flag = 1;
    flag_update_time = 1;
    if(mode == 1)
    {
        char temp_arr[20];
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("The RFID");
        sprintf(temp_arr, "ma the: %s", arr_rfid);
        lcd.setCursor(1, 1);
        lcd.print(temp_arr);
        lcd.setCursor(1, 2);
        lcd.print(arr_name);
    }
    else if(mode == 2)
    {
       char temp_arr[20];
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Van tay");
        sprintf(temp_arr, "ma van tay: %d", uid);
        lcd.setCursor(1, 1);
        lcd.print(temp_arr);
        lcd.setCursor(1, 2);
        lcd.print(arr_name);
    }
    else if(mode == 3)
    {
       char temp_arr[20];
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Van tay");
        sprintf(temp_arr, "ma van tay: %d", uid);
        lcd.setCursor(1, 1);
        lcd.print(temp_arr);
        lcd.setCursor(1, 2);
        lcd.print(arr_name);
    }
    else if(mode == 4)
    {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("Van tay loi!");
    }
}