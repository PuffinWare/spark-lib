#include "DS2482.h"
#include <spark_wiring_i2c.h>
#include <spark_wiring_usbserial.h>

//#define DS2482_DEBUG 1

static const byte READ_BYTE_REQ[] = { MAXIM_SET_READ_PTR, MAXIM_REG_READ_DATA };

DS2482::DS2482(byte address) {
  this->address = address;
  this->state = DS_START;
  this->nextState = DS_START;
  this->waitTime = 0;
  this->idx = 0;
  this->readyLogged = false;
}

/*!
 * Convenience method to check for completion of 1W activity.
 * Equivalent to calling poll() and then checking for DS_READY
 */
bool DS2482::oneWireComplete() {
  return poll() == DS_READY;
}

/*!
 * Called on the main poll loop, returns the status of the DS2482
 */
DS_STATE DS2482::poll() {
  // Either there is nothing to do, or we wait for a while before we do it
  if (waitTime > 0 && (millis() < waitTime)) {
    return state;
  }

  // Waiting for the 1Wire activity to complete is different than the other states,
  // the 'next' state will take effect after 1W comm is complete

  if (state != DS_POLL_1W_BUSY && nextState != DS_NONE) {
    state = nextState;
    nextState = DS_NONE;
    readyLogged = false;
    waitTime = 0;
  }

  // state machine
  switch(state) {
    case DS_START:
      changeState(1000, DS_DEV_RESET);
      break;

    case DS_DEV_RESET:
#ifdef DS2482_DEBUG
      Serial.println("Dev Reset");
#endif
      send(MAXIM_DEVICE_RESET, true);
      changeState(10, DS_CONFIG);
      break;

    case DS_CONFIG:
#ifdef DS2482_DEBUG
      Serial.println("Send Config");
#endif
      writeConfig(false, false, true);
      changeState(0, DS_READ_CONFIG);
      break;

    case DS_READ_CONFIG:
#ifdef DS2482_DEBUG
      Serial.println("Read Config");
#endif
      readConfig(false);
      readStatus(true);
      changeState(0, DS_READY);
      break;

    case DS_READY:
#ifdef DS2482_DEBUG
      if (!readyLogged) {
        readyLogged = true;
        Serial.println("Ready");
      }
#endif
      // no op, waiting for something to do
      break;

    case DS_1W_RESET:
#ifdef DS2482_DEBUG
      Serial.println("1W Reset");
#endif
      send(MAXIM_1WR_RESET, true);
      pollStatus(10, DS_1W_REQUEST);
      break;

    case DS_POLL_1W_BUSY:
#ifdef DS2482_DEBUG
      Serial.println("Read Status");
#endif
      readStatus(false);
      if (!status.oneWireBusy) {
        state = nextState;
      }
      break;

    case DS_1W_REQUEST:
#ifdef DS2482_DEBUG
      Serial.println("1W Request");
#endif
      sendData(MAXIM_1WR_WRITE_BYTE, request[idx++], false);
      len--;
      pollStatus(0, len == 0 ?  DS_READY : DS_1W_REQUEST);
      break;

    case DS_1W_RESPONSE:
#ifdef DS2482_DEBUG
      Serial.println("1W Response");
#endif
      send(MAXIM_1WR_READ_BYTE, false);
      pollStatus(0, DS_RESPONSE);
      break;

    case DS_RESPONSE:
#ifdef DS2482_DEBUG
      Serial.println("DS Response");
#endif
      readByte();
      len--;
      changeState(0, len == 0 ?  DS_READY : DS_1W_RESPONSE);
      break;
  }

  return state;
}

// Public Methods

/*!
 * Write a byte of data out to the 1W device
 */
void DS2482::writeTo1W(byte data) {
  request[0] = data;
  this->idx = 0;
  this->len = 1;
  changeState(0, DS_1W_RESET);
}

/*!
 * Write several bytes of data out to the 1W device
 */
void DS2482::writeTo1W(const byte *data, int len) {
  memcpy(request, data, len);
  this->idx = 0;
  this->len = len;
  changeState(0, DS_1W_RESET);
}

/*!
 * Initiate a read from the 1W device
 */
void DS2482::readFrom1W(int len) {
  this->idx = 0;
  this->len = len;
  changeState(0, DS_1W_RESPONSE);
}

/*!
 * Return the byte of data retrieved from the 1W device
 */
void DS2482::copyBuffer(byte *dest, int len) {
  memcpy(dest, response, len);
};


// Private Methods
void DS2482::changeState(ulong wait, DS_STATE next) {
  eventTime = millis();
  waitTime = eventTime + wait;
  nextState = next;
}

void DS2482::pollStatus(ulong wait, DS_STATE next) {
  changeState(wait, next);
  state = DS_POLL_1W_BUSY;
}

void DS2482::send(byte command, bool stop) {
  Wire.beginTransmission(address);
  Wire.write(command);
#ifdef DS2482_DEBUG
  Serial.printlnf("write:%x", command);
#endif
  Wire.endTransmission(stop);
}

void DS2482::sendData(byte command, byte data, bool stop) {
  Wire.beginTransmission(address);
  Wire.write(command);
#ifdef DS2482_DEBUG
  Serial.printlnf("write:%x", command);
#endif
  Wire.write(data);
#ifdef DS2482_DEBUG
  Serial.printlnf("write:%x", data);
#endif
  Wire.endTransmission(stop);
}

/*!
 * Write config to DS2483
 */
void DS2482::writeConfig(bool owSpeed, bool strongPullUp, bool activePullUp) {
  byte cfg = 0x00;
  cfg |= owSpeed ? 0x08 : 0x00;
  cfg |= strongPullUp ? 0x04 : 0x00;
  cfg |= activePullUp ? 0x01 : 0x00;
  cfg |= (~cfg << 4);
  sendData(MAXIM_WRITE_CONFIG, cfg, false);
}

/*!
 * Read config byte
 */
void DS2482::readConfig(bool setPtr) {
  if (setPtr) {
    sendData(MAXIM_SET_READ_PTR, MAXIM_REG_CONFIG, false);
  }

  Wire.requestFrom((int)address, 1);
  byte config = Wire.read();
#ifdef DS2482_DEBUG
  Serial.println("-- Config --");
  Serial.println(config, BIN);
#endif
}

/*!
 * Read status byte
 */
void DS2482::readStatus(bool setPtr) {
  if (setPtr) {
    sendData(MAXIM_SET_READ_PTR, MAXIM_REG_STATUS, false);
  }

  Wire.requestFrom((int)address, 1);
  byte tempStatus = Wire.read();
#ifdef DS2482_DEBUG
  Serial.println("-- Status --");
  Serial.println(tempStatus, BIN);
#endif

  status.oneWireBusy = (tempStatus & 0x01) == 0x01;
  status.presencePulse = (tempStatus & 0x02) == 0x02;
  status.shortDetected = (tempStatus & 0x04) == 0x04;
  status.logicLevel = (tempStatus & 0x08) == 0x08;
  status.deviceReset = (tempStatus & 0x10) == 0x10;
  status.singleBitResult = (tempStatus & 0x20) == 0x20;
  status.tripletSecondBit = (tempStatus & 0x40) == 0x40;
  status.branchDirTaken = (tempStatus & 0x80) == 0x80;
}

void DS2482::readByte() {
  Wire.beginTransmission(address);
  Wire.write(MAXIM_SET_READ_PTR);
  Wire.write(MAXIM_REG_READ_DATA);
  Wire.endTransmission(false);

  Wire.requestFrom((int)address, 1);
  response[idx] = Wire.read();
#ifdef DS2482_DEBUG
  Serial.println("-- Read --");
  Serial.println(response[idx], BIN);
#endif
  idx++;
}
