#include "HIH6130.h"
#include <spark_wiring_i2c.h>
#include <spark_wiring_usbserial.h>

#define HIH_ACQUIRE_TIMEOUT  1000
#define HIH_BITS_14 16383

//#define HIH_DEBUG 0

HIH6130::HIH6130(byte address, int interval, int tempAdjust) {
  this->address = address;
  this->interval = interval;
  this->tempAdjust = tempAdjust;
  this->mode = START;
  this->lastUpdate = 0;
  this->waitTime = 0;
  this->lastTemp = 0;
  this->lastRH = 0;
}

int HIH6130::getTempC() {
  return lastTemp;
}

int HIH6130::getTempF() {
  double t = lastTemp;
  return int(t * 1.8) + 320;
}

int HIH6130::getRH() {
  return lastRH;
}

bool HIH6130::poll() {
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
#ifdef HIH_DEBUG
      Serial.println("HIH Request");
#endif
      requestReading();
      event(100, RESPONSE);
      break;

    case RESPONSE:
#ifdef HIH_DEBUG
      Serial.println("HIH Response");
#endif
      readData();
      event(interval, READY);
    if (status == HIH_NORMAL) {
        result = true;
      }
      break;
  }

  return result;
}

void HIH6130::update() {
  if (mode == READY) {
    event(0, REQUEST);
  }
}

void HIH6130::event(ulong wait, I2C_MODE next) {
  waitTime = wait;
  mode = next;
  eventTime = millis();
}

void HIH6130::requestReading() {
  Wire.beginTransmission(address);
  Wire.endTransmission();
}

void HIH6130::readData() {
  byte data[4];
  int idx = 0;
  uint32_t newRH, newTemp;

  memset(data, 0 , 4);
  Wire.requestFrom((int)address, (int)4);
  while (idx < 4) {
    while(Wire.available() > 0) {
      data[idx++] = Wire.read();
    }
  }

  status = (data[0] >> 6) & 0x03;
  if (status != HIH_NORMAL) {
#ifdef HIH_DEBUG
    Serial.println("HIH Fail");
#endif
    return;
  }

  newRH = ((data[0] & 0x3f) << 8) | data[1];
#ifdef HIH_DEBUG
  Serial.println("HIH Data");
  Serial.println(data[0], BIN);
  Serial.println(data[1], BIN);
  Serial.println(data[2], BIN);
  Serial.println(data[3], BIN);
#endif

  //newRH = (((unsigned int)rh_H) << 8) | rh_L;
  newTemp = (data[2] << 8) | data[3];
  newTemp = newTemp >> 2;
#ifdef HIH_DEBUG
  Serial.println(newRH);
  Serial.println(newTemp);
#endif

  lastRH = (newRH * 1000) / HIH_BITS_14;
  lastTemp = ((newTemp * 1650) / HIH_BITS_14 - 400) + tempAdjust;
}
