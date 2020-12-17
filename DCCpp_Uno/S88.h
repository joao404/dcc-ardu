/*
 * S88.h
 *
 * Created: 14.03.2019 20:48:52
 *  Author: Marcel Maage
 */ 


 /*
  * S88 Controlmodul
  * Call in loop-routine
  * operates with millis
  * 
  */

  #ifndef S88_H
  #define S88_H

  #include "Arduino.h"



  #define S88CLOCKTIME 400     //Microseconds = 1100Hz
  

struct S88{
  bool activ;
  uint8_t* S88InputArray;//[INPUT_BYTES];
  int S88InputBytes;
  unsigned long S88previousMicros;       // will store last time of S88 clock
  unsigned int S88RCount;    //Lesezähler 0-39 Zyklen (S88Module * 8 * 2 + 10)
  byte S88MCount;   //Lesezähler für S88 Module
  byte S88PCount;   //Lesezähler für S88 Pin am Modul
  void init(int ModuleNumber);
  void cycle(void);
  void readData(void);
  void GetInputData(void);
  void deactivate(int ModuleNumber);
}; 


  #endif
