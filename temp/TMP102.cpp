#include "TMP102.h"
#include <spark_wiring_i2c.h>
#include <spark_wiring_usbserial.h>


#ifndef PUFFIN_DEBUG
#define PUFFIN_DEBUG 1
#endif

TMP102::TMP102(byte address) {
  this->address = address;

  this->mode = START;
  this->lastUpdate = 0;
  this->waitTime = 0;
  this->lastTemp = 0;
}

int TMP102::getTempC() {
  return lastTemp;
}

int TMP102::getTempF() {
  double t = lastTemp;
  return int(t * 1.8) + 320;
}

bool TMP102::poll() {
  bool result = false;
  if (waitTime > 0 && (millis() - eventTime) < waitTime) {
    return result;
  }
  waitTime = 0;

  // poor man's state machine
  switch(mode) {
    case START:
      event(1000, READY);
      break;

    case READY:
      event(0, REQUEST);
      break;

    case REQUEST:
#ifdef PUFFIN_DEBUG
      Serial.println("Request");
#endif
      requestReading();
      event(100, RESPONSE);
      break;

    case RESPONSE:
#ifdef PUFFIN_DEBUG
      Serial.println("Response");
#endif
      readData();
      event(500, READY);
      result = true;
      break;
  }

  return result;
}

void TMP102::update() {
  if (mode == READY) {
    event(0, REQUEST);
  }
}

void TMP102::event(ulong wait, I2C_MODE next) {
  waitTime = wait;
  mode = next;
  eventTime = millis();
}

void TMP102::requestReading() {
  Wire.beginTransmission(address);
  Wire.write(0x00);
  Wire.endTransmission();
}

void TMP102::readData() {
  byte data[2];
  int idx = 0;
  byte temp_H, temp_L;
  uint16_t newTemp;

  memset(data, 0 , 2);
  Wire.requestFrom((int)address, (int)2);
  while (idx < 2) {
    while(Wire.available() > 0) {
      data[idx++] = Wire.read();
    }
  }
#ifdef PUFFIN_DEBUG
  Serial.println(data[0], BIN);
  Serial.println(data[1], BIN);
#endif

  uint8_t type = (data[1]) & 0x01;
  newTemp = (data[0] << 8) | (data[1]);
  newTemp = newTemp >> (type == TMP_NORMAL ? 4 : 3);

  lastTemp = (newTemp * 10) / 16;
#ifdef PUFFIN_DEBUG
  Serial.println(newTemp, BIN);
  Serial.println(newTemp);
#endif
}
