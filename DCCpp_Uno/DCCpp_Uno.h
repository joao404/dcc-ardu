/**********************************************************************

DCCpp_Uno.h
COPYRIGHT (c) 2013-2016 Gregg E. Berman
              2016-2020 Harald Barth

Part of DCC++ BASE STATION for the Arduino

**********************************************************************/

#include "Config.h"

#ifndef DCCpp_Uno_h
#define DCCpp_Uno_h

/////////////////////////////////////////////////////////////////////////////////////
// RELEASE VERSION
/////////////////////////////////////////////////////////////////////////////////////

#define VERSION "11.0.9+haba"

/////////////////////////////////////////////////////////////////////////////////////
// How long to pause beteen reading current and voltage
// from the analog inputs in the main loop.
/////////////////////////////////////////////////////////////////////////////////////

#define  SAMPLE_TICKS              5000       // 1 tick is 4us so 5000 is 20ms

/////////////////////////////////////////////////////////////////////////////////////
// AUTO-SELECT ARDUINO BOARD
/////////////////////////////////////////////////////////////////////////////////////

#ifdef ARDUINO_AVR_MEGA                   // is using Mega 1280, define as Mega 2560 (pinouts and functionality are identical)
  #define ARDUINO_AVR_MEGA2560
#endif

#ifdef USE_TRIGGERPIN
#define TRIGGERPIN 4                     // Should work on UNO and MEGA
#endif

#if defined ARDUINO_AVR_UNO || defined ARDUINO_AVR_NANO 

  #define ARDUINO_TYPE    "UNO"

  #define DCC_SIGNAL_PIN_MAIN 10          // Ardunio Uno  - uses OC1B
  #define DCC_SIGNAL_PIN_PROG 5           // Arduino Uno  - uses OC0B

  #if COMM_INTERFACE != 0                 // Serial was not selected

    #error CANNOT COMPILE - DCC++ FOR THE UNO CAN ONLY USE SERIAL COMMUNICATION - PLEASE SELECT THIS IN THE CONFIG FILE

  #endif

#elif defined  ARDUINO_AVR_MEGA2560

  #define ARDUINO_TYPE    "MEGA"

  #define DCC_SIGNAL_PIN_MAIN 12          // Arduino Mega - uses OC1B
  #define DCC_SIGNAL_PIN_PROG 2           // Arduino Mega - uses OC3B

#else

  #error CANNOT COMPILE - DCC++ ONLY WORKS WITH AN ARDUINO UNO OR AN ARDUINO MEGA 1280/2560

#endif

/////////////////////////////////////////////////////////////////////////////////////
// SELECT MOTOR SHIELD
/////////////////////////////////////////////////////////////////////////////////////

#if MOTOR_SHIELD_TYPE == 0

  #define MOTOR_SHIELD_NAME "ARDUINO MOTOR SHIELD"
  #define MOTOR_SHIELD_CURRENT_LIMIT 1500 //mA  - be conservative because of bad shields

  #define SIGNAL_ENABLE_PIN_MAIN 3
  #define SIGNAL_ENABLE_PIN_PROG 11

#ifdef RAILCOM_CUTOUT
  #define BRAKE_PIN_MAIN 9
// not used yet  #define BRAKE_PIN_PROG 8
#endif

  #define CURRENT_MONITOR_PIN_MAIN A0
  #define CURRENT_MONITOR_PIN_PROG A1
  #define VOLTAGE_MONITOR_PIN_MAIN A2

  #define DIRECTION_MOTOR_CHANNEL_PIN_A 12
  #define DIRECTION_MOTOR_CHANNEL_PIN_B 13

#elif MOTOR_SHIELD_TYPE == 1

  #define MOTOR_SHIELD_NAME "POLOLU MC33926 MOTOR SHIELD"
  #define MOTOR_SHIELD_CURRENT_LIMIT 3000 //mA

  #define SIGNAL_ENABLE_PIN_MAIN 9
  #define SIGNAL_ENABLE_PIN_PROG 11

#ifdef RAILCOM_CUTOUT
  DONT KNOW BRAKE PINS FOR THIS MOTOR SHIELD
  #define BRAKE_PIN_MAIN X
// not used yet  #define BRAKE_PIN_PROG Y
#endif

  #define CURRENT_MONITOR_PIN_MAIN A0
  #define CURRENT_MONITOR_PIN_PROG A1

  #define DIRECTION_MOTOR_CHANNEL_PIN_A 7
  #define DIRECTION_MOTOR_CHANNEL_PIN_B 8

#elif MOTOR_SHIELD_TYPE == 2

  #define MOTOR_SHIELD_NAME "IBT-2 MOTOR SHIELD"
  #define MOTOR_SHIELD_CURRENT_LIMIT 1500 //mA  - be conservative because of bad shields
#ifdef RAILCOM_CUTOUT
  #define BRAKE_PIN_MAIN 9
// not used yet  #define BRAKE_PIN_PROG 8
#endif

  #define SIGNAL_ENABLE_PIN_MAIN 3
  #define SIGNAL_ENABLE_PIN_PROG 11

  #define CURRENT_MONITOR_PIN_MAIN A0
  #define CURRENT_MONITOR_PIN_PROG A1

  #define DIRECTION_MOTOR_CHANNEL_PIN_A 12
  #define DIRECTION_MOTOR_CHANNEL_PIN_B 13

#else

  #error CANNOT COMPILE - PLEASE SELECT A PROPER MOTOR SHIELD TYPE

#endif

/////////////////////////////////////////////////////////////////////////////////////
// SELECT COMMUNICATION INTERFACE
/////////////////////////////////////////////////////////////////////////////////////

#if COMM_INTERFACE == 0

  #define COMM_TYPE 0
  #define INTERFACE Serial

#elif (COMM_INTERFACE==1) || (COMM_INTERFACE==2) || (COMM_INTERFACE==3)

  #define COMM_TYPE 1
  #define INTERFACE eServer
  #define SDCARD_CS 4
  
#else

  #error CANNOT COMPILE - Please select a proper value for COMM_INTERFACE in CONFIG.H file

#endif

/////////////////////////////////////////////////////////////////////////////////////
// SELECT S88 INTERACE
/////////////////////////////////////////////////////////////////////////////////////

#if defined ARDUINO_AVR_UNO || defined ARDUINO_AVR_NANO || defined ARDUINO_AVR_MEGA2560
  #define S88DataPin A2
  #define S88ClkPin A3
  #define S88PSPin A4
  #define S88ResetPin A5

  #define S88AdrBase 100
  #define S88AdrMin 60
  #define S88AdrMax 100
  
#else
  #error CANNOT COMPILE - NO KNOWN S88 INTERFACE
#endif

/////////////////////////////////////////////////////////////////////////////////////
// SET WHETHER TO SHOW PACKETS - DIAGNOSTIC MODE ONLY
/////////////////////////////////////////////////////////////////////////////////////

// If SHOW_PACKETS is set to 1, then for select main operations track commands that modify an internal DCC packet register,
// if printFlag for that command is also set to 1, DCC++ BASE STATION will additionally return the 
// DCC packet contents of the modified register in the following format:

//    <* REG: B1 B2 ... Bn CSUM / REPEAT>
//
//    REG: the number of the main operations track packet register that was modified
//    B1: the first hexidecimal byte of the DCC packet
//    B2: the second hexidecimal byte of the DCC packet
//    Bn: the nth hexidecimal byte of the DCC packet
//    CSUM: a checksum byte that is required to be the final byte in any DCC packet
//    REPEAT: the number of times the DCC packet was re-transmitted to the tracks after its iniital transmission
 
#define SHOW_PACKETS  0       // set to zero to disable printing of every packet for select main operations track commands

/////////////////////////////////////////////////////////////////////////////////////

#endif


/////////////////////////////////////////////////////////////////////////////////////
// Global vars
/////////////////////////////////////////////////////////////////////////////////////
//
// 
extern volatile unsigned long int tickCounter;  // instead of clock because we don't have micros() on the UNO, TIMER0 used for DCC.
class CurrentMonitor;
extern CurrentMonitor mainMonitor;
extern CurrentMonitor progMonitor;
class VoltageMonitor;
extern VoltageMonitor mainVoltageMonitor;
