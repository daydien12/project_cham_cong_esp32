#include "lib_as608.h"

/*----------------------|Finger|----------------------*/
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
uint8_t arr_finger[EEPROM_SIZE] = {0};


void EEPROM_Init(void)
{
  if (!EEPROM.begin(EEPROM_SIZE))
  {
    Serial.println("failed to initialise EEPROM"); 
  }

}

uint8_t EEPROM_Write(uint8_t *arr)
{
  uint8_t address = EEPROM_ADDRESS;
  for (uint8_t i = 0; i < EEPROM_SIZE; i++)
  {
    EEPROM.writeByte(address, arr[i]);
    EEPROM.commit();
    address += 1;
  }
  return 1;
}

uint8_t EEPROM_Read(uint8_t *arr)
{
  uint8_t address = EEPROM_ADDRESS;
  for (uint8_t i = 0; i < EEPROM_SIZE; i++)
  {
    arr[i] = EEPROM.readByte(address);
    address += 1;
  }
  return 1;
}

void AS608_Init(void)
{
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  Serial.println("\n\nAdafruit Fingerprint sensor enrollment");
  finger.begin(57600);

  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) {
      delay(1);
    }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0)
  {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else
  {
    Serial.println("Waiting for valid finger...");
    Serial.print("Sensor contains ");
    Serial.print(finger.templateCount);
    Serial.println(" templates");
  }
}

uint8_t AS608_Delete_all(void)
{
  finger.emptyDatabase();
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    arr_finger[i] = 0;
  }
  EEPROM_Write(arr_finger);
  delay(100);
  EEPROM_Read(arr_finger);
  for (int i = 0; i < EEPROM_SIZE; i++)
  {
    if( arr_finger[i] == 1)
    {
       return 0;
    }
  }
  return 1;
}

uint8_t AS608_Delete_Id(void)
{
  uint8_t p = -1;
  uint8_t Get_UID = 0;
  Serial.println("Cho tay vao ban oi!");
  Get_UID = AS608_Get_ID();
  delay(100);
  if (Get_UID <= 128)
  {
    Serial.print("Delete ID: ");
    Serial.println(Get_UID);
    p = finger.deleteModel(Get_UID);
    arr_finger[Get_UID] = 0;
    if (p == FINGERPRINT_OK)
    {
      return FINGER_DELETE_DONE;
    }
  }
  return FINGER_DELETE_ERROR;
}

uint8_t AS608_Add_New(uint8_t uid)
{
  int p = -1;
  uint8_t id = uid;
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
  }

  p = finger.image2Tz(1);
  if(FINGERPRINT_OK != p)
  {
     return FINGER_ADD_NEW_ERROR;
  }

  delay(1000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) 
  {
    p = finger.getImage();
  }

  p = -1;
  while (p != FINGERPRINT_OK) 
  {
    p = finger.getImage();
  }

  p = finger.image2Tz(2);
  if(FINGERPRINT_OK != p)
  {
     return FINGER_ADD_NEW_ERROR;
  }

  p = finger.createModel();
  if (p != FINGERPRINT_OK) 
  {
    return FINGER_ADD_NEW_ERROR;
  } 

  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK)
  {
    return FINGER_ADD_NEW_DONE;
  } 
  else 
  {
    return FINGER_ADD_NEW_ERROR;
  }
}

uint8_t AS608_Get_ID(void)
{
  uint8_t p = finger.getImage();

  if (p != FINGERPRINT_OK)
  {
    return NO_FINGER_DETECTED;
  }

  finger.image2Tz();

  p = finger.fingerSearch();

  if (p == FINGERPRINT_OK)
  {
    return finger.fingerID;
  }
  else
  {
    return DIT_NOT_FIND_A_MATCH;
  }
}

uint8_t AS608_Find_ADD_New(uint8_t *arr)
{
  for(int i=1; i<EEPROM_SIZE; i++)
  {
    if(arr[i] == 0)
    {
      return i;
    }
  }
  return FINGER_ADD_NEW_FULL;
}