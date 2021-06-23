/*
  WinsenZE25.h - This library allows you to set and read the ZE25 Winsen Sensor
  module. Created by Juan Carlos Ferrándiz June 23, 2021. Based on WinsenZE03.h
  by Fabian Gutierrez.
*/

#include "Arduino.h"
#include "WinsenZE25.h"
#define DEVMODE true //Set as true to debug

WinsenZE25::WinsenZE25(){
  _s  = NULL;
}

void WinsenZE25::begin(Stream *ser, int type){
  _s = ser;
  _type=type;
}

void WinsenZE25::setAs(bool active){
  byte setConfig[] = {0xFF, 0x01, 0x78, 0x41, 0x00, 0x00, 0x00, 0x00, 0x46};//QA config
  byte response[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  if (active){
    setConfig[3] =0x40;
    setConfig[8] =0x47;
  }
  _s->write(setConfig,sizeof(setConfig));
  // Wait for the response
  delay(2000);
  //Flush the incoming buffer
  if (_s->available() > 0) {
    _s->readBytes(response,9);
  }

  while(_s->available()>0){
    byte c = _s->read();
  }
}

float WinsenZE25::readContinuous(){
  float ppm = 0;

  if (_s->available() > 0) {
    byte measure[9];

    while (_s->available() > 0) {
      _s->readBytes(measure, 9);
      // incomingByte = _s.read();
    }

    if (DEVMODE) {
      SerialUSB.println("Measure: ");
      debugPrint(measure);
    }

    ppm = measure[2] * 256 + measure[3]; //Result is in ppb
    ppm = ppm/1000;                      //Result now is in ppm

  } else if (DEVMODE) {
    SerialUSB.println("No data in buffer.");
  }
  return ppm;
}

//This function is not working right now
float WinsenZE25::readManual(){
  float ppm;
  //Modify petition format depending on the sensor
  byte petition[] = {0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79};// Petition to get a single result
  byte measure[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // Space for the response
  _s->write(petition,sizeof(petition));
  
  //TO DO: Modify this delay and use a callback in this function
  delay(1500);
  // read
  if (_s->available() > 0) {
    while (_s->available() > 0) {
      _s->readBytes(measure, 9);
      // incomingByte = _s.read();
    }

    if(DEVMODE){
      SerialUSB.println("Measure: ");
      debugPrint(measure);
    }

  } else if (DEVMODE) {
    SerialUSB.println("No data in buffer.");
  }

  // calculate
  if (measure[0]==0xff && measure[1]==0x86){
    // this formula depends of the sensor is in the dataSheet
    ppm = measure[2] * 256 + measure[3];  // Result is in ppb
    if (_type == 2){
      ppm = ppm / 1000;  // Result now is in ppm
    }
  }else{
    ppm=-1;
  }
  return ppm;
}

void WinsenZE25::debugPrint(byte arr[]){
  SerialUSB.print(arr[0], HEX);
  SerialUSB.print(" ");
  SerialUSB.print(arr[1], HEX);
  SerialUSB.print(" ");
  SerialUSB.print(arr[2], HEX);
  SerialUSB.print(" ");
  SerialUSB.print(arr[3], HEX);
  SerialUSB.print(" ");
  SerialUSB.print(arr[4], HEX);
  SerialUSB.print(" ");
  SerialUSB.print(arr[5], HEX);
  SerialUSB.print(" ");
  SerialUSB.print(arr[6], HEX);
  SerialUSB.print(" ");
  SerialUSB.print(arr[7], HEX);
  SerialUSB.print(" ");
  SerialUSB.println(arr[8], HEX);
}