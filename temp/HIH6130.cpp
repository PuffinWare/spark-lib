#include "HIH6130.h"
#include <spark_wiring_i2c.h>
#include <spark_wiring_usbserial.h>
#include "cellar_util.h"

#define HIH_ACQUIRE_TIMEOUT  1000
#define HIH_BITS_14 16383

//#define HIH_DEBUG 0

HIH6130::HIH6130(byte address, int interval, int tempAdjust)
    : I2CBase(address) {
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
      if (i2cSend()) {
        event(100, RESPONSE);
      } else {
        blink(3, 100, 150);
        event(interval, READY);
      }
      break;

    case RESPONSE:
#ifdef HIH_DEBUG
      Serial.println("HIH Response");
#endif
      if (i2cRead(4)) {
        readData();
        if (status == HIH_NORMAL) {
          result = true;
        }
      }
      event(interval, READY);
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

void HIH6130::readData() {
  uint32_t newRH, newTemp;

  status = (i2cData[0] >> 6) & 0x03;
  if (status != HIH_NORMAL) {
#ifdef HIH_DEBUG
    Serial.println("HIH Fail");
#endif
    return;
  }

  newRH = ((i2cData[0] & 0x3f) << 8) | i2cData[1];
#ifdef HIH_DEBUG
  Serial.println("HIH Data");
  Serial.println(data[0], BIN);
  Serial.println(data[1], BIN);
  Serial.println(data[2], BIN);
  Serial.println(data[3], BIN);
#endif

  //newRH = (((unsigned int)rh_H) << 8) | rh_L;
  newTemp = (i2cData[2] << 8) | i2cData[3];
  newTemp = newTemp >> 2;
#ifdef HIH_DEBUG
  Serial.println(newRH);
  Serial.println(newTemp);
#endif

  lastRH = (newRH * 1000) / HIH_BITS_14;
  lastTemp = ((newTemp * 1650) / HIH_BITS_14 - 400) + tempAdjust;
}
