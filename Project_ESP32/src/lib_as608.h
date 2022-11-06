#ifndef LIB_AS608_h
#define LIB_AS608_h
#include "define.h"
#include <EEPROM.h>
#include <LIB/Adafruit_Fingerprint.h>
extern uint8_t arr_finger[];

void EEPROM_Init(void);
uint8_t EEPROM_Write(uint8_t *arr);
uint8_t EEPROM_Read(uint8_t *arr);

void AS608_Init(void);
uint8_t AS608_Delete_all(void);
uint8_t AS608_Delete_Id(void);
uint8_t AS608_Add_New(uint8_t uid);
uint8_t AS608_Get_ID(void);
uint8_t AS608_Find_ADD_New(uint8_t *arr);
#endif

