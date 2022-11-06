#include "time.h"
#include <SPI.h>
#include <WiFi.h>
#include "button.h"
#include "lib_lcd.h"
#include <WiFiUdp.h>
#include <MFRC522.h>
#include "lib_as608.h"
#include <NTPClient.h>
#include <HardwareSerial.h>

typedef struct
{
  uint8_t RFID_Size;
  uint8_t RFID_Data[10];
  char RFID_Data_char[20];

  uint16_t TIME_Count_1S;
  uint8_t TIME_Flag_1S;

  uint8_t TIME_Count_Get_UID_50ms;
  uint8_t TIME_Flag_Get_UID_50ms;

  uint8_t AS608_Count_Not_Detect;
  uint8_t AS608_UID_Data;
  uint8_t AS608_Flag_Increase_Uid;
  char LCD_Mode_Setting;
  uint8_t LCD_Select_Setting;

  uint8_t LCD_Select_Add_new;
  uint8_t LCD_Select_Delete_Uid;
  uint8_t LCD_Select_Delete_All;

  uint8_t LCD_TimeOut_Count_2S;
  uint8_t LCD_Display_Flag;

} value_str;

value_str var_sys;
volatile uint8_t mode = 0;

const char* ssid     = "P702_2G";
const char* password = "nhanma25";
const char* ntpServer = "asia.pool.ntp.org";
const long  gmtOffset_sec = 7 * 60 * 60;
const int   daylightOffset_sec = 3600;

/*----------------------|BUTTON|----------------------*/
// Button BT1 = Button(34, LOW);
// Button BT2 = Button(32, LOW);
// Button BT3 = Button(33, LOW);


/*----------------------|RFID|----------------------*/
MFRC522 rfid(RC_522_SS_PIN, RC_522_RST_PIN);
/*----------------------|TIMER|----------------------*/
hw_timer_t *My_timer = NULL;


void RFID_Init(void);
void TIMER_Init(void);
void Value_Init(void);
void DateandTime_Init(void);
uint8_t RFID_GetId(uint8_t *arr_uid);

void IRAM_ATTR onTimer()
{
  if (var_sys.TIME_Count_1S >= 1000)
  {
    var_sys.TIME_Count_1S = 0;
    var_sys.TIME_Flag_1S = 1;
    if(var_sys.LCD_TimeOut_Count_2S > 0)
    {
      var_sys.LCD_TimeOut_Count_2S--;
    }
  }
  else
  {
    var_sys.TIME_Count_1S++;
  }

  if (var_sys.TIME_Count_Get_UID_50ms >= 50)
  {
    var_sys.TIME_Count_Get_UID_50ms = 0;
    var_sys.TIME_Flag_Get_UID_50ms = 1;
  }
  else
  {
    var_sys.TIME_Count_Get_UID_50ms++;
  }
  Button_Readall();
}

void setup()
{
  Value_Init();
  TIMER_Init();
  RFID_Init();
  AS608_Init();
  EEPROM_Init();
  DateandTime_Init();

  Serial.println(" ");

  EEPROM_Read(arr_finger);
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    Serial.print(arr_finger[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
  LCD_Init();
  Button_Init();
}

void loop()                     // run over and over again
{
  static uint8_t temp_id;
  uint8_t Get_UID = 0;

  if (vrts_Button1.vruc_FlagChange == 1)
  {
    vrts_Button1.vruc_FlagChange = 0;
    if (var_sys.LCD_Select_Setting == 1)
    {
      if (var_sys.LCD_Mode_Setting >= 4)
      {
        var_sys.LCD_Mode_Setting = 1;
      }
      else
      {
        var_sys.LCD_Mode_Setting ++;
      }
      LCD_Mode_Setting_Run(var_sys.LCD_Mode_Setting);
    }
    else if (var_sys.LCD_Select_Setting > 1)
    {
      var_sys.LCD_Mode_Setting = 4;
      var_sys.LCD_Select_Setting = 1;
      LCD_Mode_Setting_Run(var_sys.LCD_Mode_Setting);
    }
  }
  else if (vrts_Button2.vruc_FlagChange == 1)
  {
    vrts_Button2.vruc_FlagChange = 0;
    if (var_sys.LCD_Select_Setting == 1)
    {
      if (var_sys.LCD_Mode_Setting <= 1)
      {
        var_sys.LCD_Mode_Setting = 4;
      }
      else
      {
        var_sys.LCD_Mode_Setting --;
      }
      LCD_Mode_Setting_Run(var_sys.LCD_Mode_Setting);
    }
    else if (var_sys.LCD_Select_Setting > 1)
    {
      var_sys.LCD_Mode_Setting = 4;
      var_sys.LCD_Select_Setting = 1;
      LCD_Mode_Setting_Run(var_sys.LCD_Mode_Setting);
    }
  }
  else if (vrts_Button3.vruc_FlagChange == 1)
  {
    vrts_Button3.vruc_FlagChange = 0;
    if (var_sys.LCD_Select_Setting == 2)
    {
      var_sys.LCD_Mode_Setting = 4;
    }

    if (var_sys.LCD_Mode_Setting == 0)
    {
      LCD_Mode_Setting_Init();
      var_sys.LCD_Select_Setting = 1;
    }
    else if (var_sys.LCD_Mode_Setting == 1)
    {
      LCD_Mode_Add_New_Init();
      if (var_sys.AS608_Flag_Increase_Uid == 0)
      {
        var_sys.AS608_Flag_Increase_Uid = 1;
        temp_id = AS608_Find_ADD_New(arr_finger);
        arr_finger[temp_id] = 1;
      }
      var_sys.LCD_Select_Add_new = 1;
      var_sys.LCD_Select_Setting = 2;
    }
    else if (var_sys.LCD_Mode_Setting == 2)
    {
      LCD_Mode_Delete_Uid_Init();
      var_sys.LCD_Select_Setting = 2;
      var_sys.LCD_Select_Delete_Uid = 1;
    }
    else if (var_sys.LCD_Mode_Setting == 3)
    {
      LCD_Mode_Delete_All_Init();
      var_sys.LCD_Select_Setting = 2;
      var_sys.LCD_Select_Delete_All = 1;
    }
    else if (var_sys.LCD_Mode_Setting == 4)
    {
      var_sys.LCD_Mode_Setting = 0;
      var_sys.LCD_Select_Setting = 0;
      var_sys.LCD_Select_Add_new = 0;
      var_sys.LCD_Select_Delete_All = 0;
      var_sys.LCD_Select_Delete_Uid = 0;
      LCD_Mode_Back();
    }
  }

  if (var_sys.LCD_Select_Setting > 0 && var_sys.LCD_Mode_Setting == 1)
  {
    if (var_sys.LCD_Select_Add_new == 1)
    {
      if (temp_id != FINGER_ADD_NEW_FULL)
      {
        AS608_Add_New(temp_id);
      }
      var_sys.LCD_Select_Add_new = 0;
      LCD_Mode_Add_New_Run(2, temp_id);
    }
    else
    {
      Get_UID = AS608_Get_ID();
      delay(100);
      if (Get_UID <= 128)
      {
        var_sys.AS608_Count_Not_Detect = 0;
        Serial.print("Ten: ");
        Serial.println(Get_UID);
        if (var_sys.AS608_Flag_Increase_Uid && Get_UID == temp_id)
        {
          var_sys.AS608_Flag_Increase_Uid = 0;
          LCD_Mode_Add_New_Run(3, temp_id);
          EEPROM_Write(arr_finger);
          EEPROM_Read(arr_finger);
          Serial.println("Stored!");
        }
        else
        {
          Serial.println("Loi");
          LCD_Mode_Add_New_Run(4, temp_id);
        }
      }
      else if (Get_UID == DIT_NOT_FIND_A_MATCH)
      {
        if (var_sys.AS608_Count_Not_Detect >= 5)
        {
          Serial.println("Loi");
          LCD_Mode_Add_New_Run(4, temp_id);
          var_sys.AS608_Count_Not_Detect = 0;
        }
        else
        {
          var_sys.AS608_Count_Not_Detect++;
        }
      }
    }
  }
  else if (var_sys.LCD_Select_Setting > 0 && var_sys.LCD_Mode_Setting == 2)
  {
    if (var_sys.LCD_Select_Delete_Uid == 1)
    {
      if (AS608_Delete_Id() == FINGER_DELETE_DONE)
      {
        temp_id = 0;
        LCD_Mode_Delete_Uid_Run();
        var_sys.LCD_Select_Delete_Uid = 0;
        Serial.println("Delete Done!");
        EEPROM_Write(arr_finger);
        EEPROM_Read(arr_finger);
      }
    }
  }
  else if (var_sys.LCD_Select_Setting > 0 && var_sys.LCD_Mode_Setting == 3)
  {
    if (var_sys.LCD_Select_Delete_All == 1)
    {
      var_sys.LCD_Select_Delete_All = 0;
      if (AS608_Delete_all())
      {
        LCD_Mode_Delete_All_Run(1);
      }
      else
      {
        LCD_Mode_Delete_All_Run(0);
      }
    }
  }
  else
  {
    char i;
    char arr[10];
    var_sys.RFID_Size =  RFID_GetId(var_sys.RFID_Data);
    if (var_sys.RFID_Size != 0)
    {
      strcpy(var_sys.RFID_Data_char, "");
      for (i = 0; i < var_sys.RFID_Size; i++)
      {
        Serial.print(var_sys.RFID_Data[i], HEX);
        sprintf(arr, "%02X", var_sys.RFID_Data[i]);
        strcat(var_sys.RFID_Data_char, arr); 
      }
      Serial.println();
      var_sys.RFID_Size = 0;
      var_sys.LCD_TimeOut_Count_2S = 3;
      var_sys.LCD_Display_Flag = 1;
    }

    if ( var_sys.TIME_Flag_Get_UID_50ms)
    {
      var_sys.TIME_Flag_Get_UID_50ms = 0;
      Get_UID = AS608_Get_ID();
      if (Get_UID <= 128)
      {
        var_sys.AS608_UID_Data = Get_UID;
        var_sys.AS608_Count_Not_Detect = 0;
        Serial.print("Ten: ");
        Serial.println(var_sys.AS608_UID_Data);
        var_sys.LCD_TimeOut_Count_2S = 3;
        var_sys.LCD_Display_Flag = 2;
      }
      else if (Get_UID == DIT_NOT_FIND_A_MATCH)
      {
        if (var_sys.AS608_Count_Not_Detect >= 5)
        {
          Serial.println("Loi");
          var_sys.AS608_Count_Not_Detect = 0;
          var_sys.LCD_TimeOut_Count_2S = 3;
          var_sys.LCD_Display_Flag = 3;
        }
        else
        {
          var_sys.AS608_Count_Not_Detect++;
        }
      }
    }

    if(var_sys.LCD_Display_Flag == 1)
    {
      LCD_Display_Infor_User(1, 0, var_sys.RFID_Data_char, "Nguyen Quang Bien");
      var_sys.LCD_Display_Flag = 0;
    }
    else if(var_sys.LCD_Display_Flag == 2)
    {
      LCD_Display_Infor_User(2, var_sys.AS608_UID_Data, "", "Nguyen Quang Bien");
      var_sys.LCD_Display_Flag = 0;
    }
    else if(var_sys.LCD_Display_Flag == 3)
    {
      LCD_Display_Infor_User(4, 0, "", "");
      var_sys.LCD_Display_Flag = 0;
    }

    if(var_sys.LCD_TimeOut_Count_2S == 0 && lcd_display_infor_user_flag == 1)
    {
      lcd_display_infor_user_flag = 0;
    }
  }


  if (var_sys.TIME_Flag_1S)
  {
    if (var_sys.LCD_Select_Setting == 0 && lcd_display_infor_user_flag == 0)
    {
      struct tm timeinfo;
      char timeStringBuff[25];
      if (!getLocalTime(&timeinfo))
      {
        LCD_Update_Date_Time(5, 1, "Loi mang!");
      }
      else
      {
        strftime(timeStringBuff, sizeof(timeStringBuff), "%d/%m/%Y-%H:%M:%S", &timeinfo);
        LCD_Update_Date_Time(0, 1, timeStringBuff);
      }
    }
    var_sys.TIME_Flag_1S = 0;
  }
}


void DateandTime_Init(void)
{
  struct tm timeinfo;
  char timeStringBuff[25]; //50 chars should be enough

  Serial.println();
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(timeStringBuff, sizeof(timeStringBuff), "%d/%m/%Y - %H:%M:%S", &timeinfo);
  //print like "const char*"
  Serial.println(timeStringBuff);
  //disconnect WiFi as it's no longer needed
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void Value_Init(void)
{
  var_sys.RFID_Size = 0;
  var_sys.TIME_Flag_1S = 0;
  var_sys.TIME_Count_1S = 0;
  var_sys.AS608_UID_Data = 0;
  var_sys.AS608_Count_Not_Detect = 0;
  var_sys.TIME_Flag_Get_UID_50ms = 0;
  var_sys.TIME_Count_Get_UID_50ms = 0;
  var_sys.AS608_Flag_Increase_Uid = 0;
  var_sys.LCD_Mode_Setting = 0;
  var_sys.LCD_Select_Setting = 0;
}

void RFID_Init(void)
{
  SPI.begin();
  rfid.PCD_Init();
}

void TIMER_Init(void)
{
  My_timer = timerBegin(0, 80, true);
  timerAttachInterrupt(My_timer, &onTimer, true);
  timerAlarmWrite(My_timer, 1000, true);
  timerAlarmEnable(My_timer);
}

uint8_t RFID_GetId(uint8_t *arr_uid)
{
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      for (int i = 0; i < rfid.uid.size; i++)
      {
        arr_uid[i] = rfid.uid.uidByte[i];
      }
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
      return rfid.uid.size;
    }
  }
  return 0;
}

