#include "DS18B20.h"
#include <spark_wiring_usbserial.h>
#include "cellar_util.h"

#define DS18B20_DEBUG 1

//static const byte READ_TEMP[] = {MAXIM_SKIP_ROM, MAXIM_CONVERT_T};
//static const byte READ_SCRATCH[] = {MAXIM_SKIP_ROM, MAXIM_READ_SCRATCH};

static const byte DEVICES[NUM_DEVICES][8] = {
    {0x28, 0xAB, 0xE4, 0x56, 0x07, 0x00, 0x00, 0xD4},
    {0x28, 0x0B, 0x77, 0xD4, 0x05, 0x00, 0x00, 0xE0},
    {0x28, 0x08, 0xD7, 0x43, 0x07, 0x00, 0x00, 0xA2},
    {0x28, 0x0A, 0xD2, 0x54, 0x07, 0x00, 0x00, 0xA0}
};

/*!
 * Constructor for reading DS18B20 devices.
 * @param owBridge the DS2482 that is connected to the sensors
 * @param interval between reading updates (one update takes > 750ms)
 */
DS18B20::DS18B20(DS2482 *owBridge, int interval) {
  this->owBridge = owBridge;
  this->interval = interval;
  this->waitTime = 0;
  this->len = 8;
  this->state = DS18B20_START;
  this->nextState = DS18B20_START;

  memset(this->reading, 0, sizeof(reading));
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
  DS_STATE dsState;
  switch(state) {
    case DS18B20_START:
      changeState(1500, DS18B20_REQUEST_TEMP);
      break;

    case DS18B20_READY:
      owBridge->poll();
      break;

    case DS18B20_1W_BUSY:
      dsState = owBridge->poll();
      if (dsState == DS_READY) {
        state = nextState;
      } else if (dsState == DS_FAIL) {
        blink(2, 300, 300);
        nextState = DS18B20_NONE;
        state = DS18B20_START;
        break;
      }
      waitTime = 10;
      break;

    case DS18B20_READ_ROM:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Read ROM");
#endif
      owBridge->writeTo1W(MAXIM_READ_ROM);
      len = 8;
      pollStatus(0, DS18B20_READ_RESPONSE);
      break;

    case DS18B20_REQUEST_TEMP:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Request Temp");
#endif
      buildRequest(MAXIM_CONVERT_T, true);
      owBridge->writeTo1W(request, 2);
      curDevice = 0;
      pollStatus(750, DS18B20_READ_DEVICES);
      break;

    case DS18B20_READ_DEVICES:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Read Devices");
#endif
      owBridge->readFrom1W(len);
      len = 9;
      changeState(0, DS18B20_READ_SCRATCHPAD);
      break;

    case DS18B20_READ_SCRATCHPAD:
#ifdef DS18B20_DEBUG
      Serial.println("DS18B20 Read Scratch");
#endif
      buildRequest(MAXIM_READ_SCRATCH, false);
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
      dsState = owBridge->poll();
      if (dsState == DS_READY) {
#ifdef DS18B20_DEBUG
        Serial.println("DS18B20 Read Data");
#endif
        owBridge->copyBuffer(data, len);
        convertData();
        curDevice++;
        if (curDevice < NUM_DEVICES) {
          changeState(0, DS18B20_READ_DEVICES);
        } else {
          state = DS18B20_READY;
          changeState(interval, DS18B20_REQUEST_TEMP);
          result = true;
        }
      } else if (dsState == DS_FAIL) {
        blink(3, 300, 300);
        changeState(interval, DS18B20_REQUEST_TEMP);
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

  reading[curDevice] = (data[1] << 8) | data[0];

#ifdef DS18B20_DEBUG
  Serial.println(reading[curDevice], BIN);
  Serial.println(getTempC(curDevice));
#endif
}

void DS18B20::buildRequest(const byte command, bool all) {
  request[0] = all ? MAXIM_SKIP_ROM : MAXIM_MATCH_ROM;
  if (!all) {
    memcpy(request+1, DEVICES[curDevice], 8);
  }
  request[all ? 1 : 9] = command;
}

int DS18B20::getTempC(int device) {
  return (reading[device] * 10) / 16;
}

int DS18B20::getTempF(int device) {
  return (reading[device] * 18) / 16 + 320;
}
