/*
	Button.h - - Button library for Wiring/Arduino - Version 0.2
	
	Original library 		(0.2) by Carlyn Maw.
	
 */

// ensure this library description is only included once
#ifndef DEFINE_h
#define DEFINE_h

#ifdef __cplusplus
extern "C"{
#endif
/* Connect
   AS608   ESP32
   VCC     3.3V
   GND     GND
   TX      16
   RX      17
*/
#define RC_522_RST_PIN  4 
#define RC_522_SS_PIN   15  


#define MODEM_RX 16
#define MODEM_TX 17
#define mySerial Serial2 // use for ESP32


#define NO_FINGER_DETECTED      200
#define DIT_NOT_FIND_A_MATCH    201

#define FINGER_DELETE_DONE      200
#define FINGER_DELETE_ERROR     201
#define FINGER_ADD_NEW_FULL     130
#define FINGER_ADD_NEW_DONE     150
#define FINGER_ADD_NEW_ERROR    200

#define EEPROM_SIZE     127
#define EEPROM_ADDRESS  0

enum add_new_finger_state
{
   getImage_1  = 0,
   image2Tz_1  = 1,
   getImage_2  = 2,
   getImage_3  = 3,
   image2Tz_2  = 4,
   createModel = 5,
   storeModel  = 6,
}; 


#ifdef __cplusplus
}
#endif
#endif

