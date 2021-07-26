/*
  WinsenZE25.h - This library allows you to set and read the ZE25 Winsen Sensor
  module. Created by Juan Carlos Ferrándiz June 23, 2021. Based on WinsenZE03.h
  by Fabian Gutierrez.
*/

#include "Arduino.h"
#include "WinsenZE25.h"

WinsenZE25::WinsenZE25() { _s = NULL; }

void WinsenZE25::begin(Stream *ser, int type,
                       void (*delay_cb)(uint32_t param)) {
  _s = ser;
  _type = type;
  _delay_cb = delay_cb;
}

void WinsenZE25::setAs(bool active) {
  byte setConfig[] = {0xFF, 0x01, 0x78, 0x41, 0x00,
                      0x00, 0x00, 0x00, 0x46};  // QA config
  byte response[9] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  if (active) {
    setConfig[3] = 0x40;
    setConfig[8] = 0x47;
  }
  _s->write(setConfig, sizeof(setConfig));
  // Wait for the response
  _delay_cb(2000);
  // Flush the incoming buffer
  if (_s->available() > 0) {
    _s->readBytes(response, 9);
  }

  while (_s->available() > 0) {
    byte c = _s->read();
    _delay_cb(1);
  }
}

float WinsenZE25::readContinuous() {
  float ppm = 0;

  if (_s->available() > 0) {
    byte measure[9];

    while (_s->available() > 0) {
      _s->readBytes(measure, 9);
      _delay_cb(1);
      // incomingByte = _s.read();
    }

    ppm = measure[2] * 256 + measure[3];  // Result in ppb
    ppm = ppm / 1000;                     // Result in ppm
  }
  return ppm;
}

float WinsenZE25::readManual() {
  float ppm;
  byte petition[] = {
      0xFF, 0x01, 0x86, 0x00, 0x00,
      0x00, 0x00, 0x00, 0x79};  // Petition to get a single result

  byte measure[9] = {0x00, 0x00, 0x00, 0x00, 0x00,
                     0x00, 0x00, 0x00, 0x00};  // Space for the response
  _s->write(petition, sizeof(petition));

  _delay_cb(1500);

  // read
  if (_s->available() > 0) {
    while (_s->available() > 0) {
      _s->readBytes(measure, 9);
      _delay_cb(1);
      // incomingByte = _s.read();
    }
  }

  // calculate
  if (measure[0] == 0xff && measure[1] == 0x86) {
    ppm = measure[2] * 256 + measure[3];  // Result in ppb
    if (_type == 2) {
      ppm = ppm / 1000;  // Result in ppm
    }
  } else {
    ppm = -1;
  }
  return ppm;
}
