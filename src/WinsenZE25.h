/*
  WinsenZE25.h - This library allows you to set and read the ZE25 Winsen Sensor
  module. Created by Juan Carlos Ferrándiz June 23, 2021. Based on WinsenZE03.h
  by Fabian Gutierrez.
*/

#ifndef WinsenZE25_h
#define WinsenZE25_h

#include "Arduino.h"
#define CO 1
#define SO2 2
#define NO2 2
#define O2 2
#define NH3 1
#define H2S 1
#define HF 1
#define CL2 2
#define O3 2

#define QA false
#define ACTIVE true

class WinsenZE25 {
 public:
  WinsenZE25();
  void begin(Stream *ser, int type, void (*delay_cb)(uint32_t param));
  void setAs(bool active);
  float readManual();
  float readContinuous();

 private:
  Stream *_s;  // Serial - Serial1 are USARTClass objects.
  void (*_delay_cb)(uint32_t param);
  int _type;
};

#endif
