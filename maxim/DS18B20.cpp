#include "DS18B20.h"
#include <spark_wiring_usbserial.h>

#define DS18B20_DEBUG 1

static const byte READ_TEMP[] = {MAXIM_SKIP_ROM, MAXIM_CONVERT_T};
static const byte READ_SCRATCH[] = {MAXIM_SKIP_ROM, MAXIM_READ_SCRATCH};

static const byte DEVICES[][8] = {
    {0x28, 0xAB, 0xE4, 0x56, 0x07, 0x00, 0x00, 0xD4},
    {0x28, 0x0B, 0x77, 0xD4, 0x05, 0x00, 0x00, 0xE0},
    {0x28, 0x08, 0xD7, 0x43, 0x07, 0x00, 0x00, 0xA2}
};

DS18B20::DS18B20(DS2482 *owBridge) {
  this->owBridge = owBridge;
//  if (address != NULL) {
//    memcpy(this->address, address, 8);
//  }
  this->waitTime = 0;
  this->lastTemp = 0;
  this->len = 8;
  this->state = DS18B20_START;
  this->nextState = DS18B20_START;
}

/*!
 * Main poll loop. Returns true after a successful reading has been completed.
 */
bool DS18B20::poll() {
  bool result = false;
  if (waitTime > 0 && (millis() < waitTime)) {
    owBridge->poll();
    return result;
  }

  if (state != DS18B20_1W_BUSY && nextState != DS18B20_NONE) {
    state = nextState;
    nextState = DS18B20_NONE;
  }
  waitTime = 0;

  // poor man's state machine
  switch(state) {
    case DS18B20_START:
      changeState(1000, DS18B20_READY);
      break;

    case DS18B20_READY:
      owBridge->poll();
      break;

    case DS18B20_1W_BUSY:
      if (owBridge->oneWireComplete()) {
        state = nextState;
      }
      waitTime = 10;
      break;

    case DS18B20_REQUEST_TEMP:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Request Temp");
#endif
      buildRequest(MAXIM_CONVERT_T);
      owBridge->writeTo1W(request, 10);
      len = 9;
      pollStatus(800, DS18B20_READ_SCRATCHPAD);
      break;

    case DS18B20_READ_ROM:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Read Scratch");
#endif
      owBridge->writeTo1W(MAXIM_READ_ROM);
      len = 8;
      pollStatus(0, DS18B20_READ_RESPONSE);
      break;

    case DS18B20_READ_SCRATCHPAD:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Read Scratch");
#endif
      buildRequest(MAXIM_READ_SCRATCH);
      owBridge->writeTo1W(request, 10);
      pollStatus(0, DS18B20_READ_RESPONSE);
      break;

    case DS18B20_READ_RESPONSE:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Read Response");
#endif
      owBridge->readFrom1W(len);
      changeState(0, DS18B20_READ_DATA);
      break;

    case DS18B20_READ_DATA:
      if (owBridge->oneWireComplete()) {
#ifdef DS18B20_DEBUG
        Serial.println("DS18B20 Read Data");
#endif
        owBridge->copyBuffer(data, len);
        convertData();
        state = DS18B20_READY;
//        changeState(250, DS18B20_REQUEST_TEMP);
        result = true;
      }
      break;
  }

  return result;
}

void DS18B20::update(int device) {
  if (state == DS18B20_READY) {
    address = DEVICES[device];
    changeState(0, DS18B20_REQUEST_TEMP);
  }
}

void DS18B20::readRom() {
  if (state == DS18B20_READY) {
    changeState(0, DS18B20_READ_ROM);
  }
}

void DS18B20::changeState(ulong wait, DS18B20_STATE next) {
  eventTime = millis();
  waitTime = eventTime + wait;
  nextState = next;
}

void DS18B20::pollStatus(ulong wait, DS18B20_STATE next) {
  changeState(wait, next);
  state = DS18B20_1W_BUSY;
}

void DS18B20::convertData() {
#ifdef DS18B20_DEBUG
  Serial.println("DS18B20 Data");
  Serial.println(data[0], HEX);
  Serial.println(data[1], HEX);
  Serial.println(data[2], HEX);
  Serial.println(data[3], HEX);
  Serial.println(data[4], HEX);
  Serial.println(data[5], HEX);
  Serial.println(data[6], HEX);
  Serial.println(data[7], HEX);
#endif

  uint16_t newTemp;
  newTemp = (data[1] << 8) | data[0];
  Serial.println(newTemp, BIN);

  lastTemp = (newTemp * 10) / 16;
  Serial.println(lastTemp);
}

void DS18B20::buildRequest(const byte command) {
  request[0] = MAXIM_MATCH_ROM;
  memcpy(request+1, address, 8);
  request[9] = command;
}

int DS18B20::getTempC() {
  return lastTemp;
}

int DS18B20::getTempF() {
  return int(lastTemp * 1.8) + 320;
}
