#include "RHT03.h"
#include "spark_wiring.h"

// linker issues forced me to do this... probably a better way?
static RHT03 *thisX;
void interruptHandler() {
  thisX->handleInterrupt();
}

RHT03::RHT03(int io, int led) {
  thisX = this;
  ioPin = io;
  ledPin = led;

  acquiring = false;
  lastTemp = 0;
  lastRH = 0;
  checksum = 0;
  intCount = 0;
  for (int i=0; i<MSG_BITS; i++) {
    bits[i]=0x0;
  }

  // Start with a HIGH mode
  pinMode(ioPin, OUTPUT);
  digitalWrite(ioPin, HIGH);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void RHT03::poll() {

}

void RHT03::update() {
  if (acquiring) {
    return;
  }

  intCount = 0;
  intFirst = true;
  acquiring = true;
  digitalWrite(ioPin, LOW);
  delay(10); // pull low for 1-10ms
  digitalWrite(ioPin, HIGH);
  delayMicroseconds(40); // pull low for 20-40us
  // Then prepare for reading
  pinMode(ioPin, INPUT_PULLUP);
  attachInterrupt(ioPin, interruptHandler, CHANGE);

  digitalWrite(ledPin, HIGH);
}

int RHT03::getTemp() {
  return lastTemp;
}

int RHT03::getRH() {
  return lastRH;
}
/*!
 * Once the device is triggered to transmit, there will be 3 pulse types:
 * - low 80us -> high 80us - start sending  (160us)
 * - low 50us -> high 26-28us - 0 bit       (76-78Us)
 * - low 50us -> high 70us - 1 bit          (129us)
 * These durations between falling edges determine the data type
 */
void RHT03::handleInterrupt() {
  if (digitalRead(ioPin) == HIGH) {
    lastInt = micros(); // just record when it happened
    return;
  } else if (intFirst) {
    // THe first two interrupts are the preparation for sending
    intFirst = false;
    return;
  }

  // Falling edge tells us how long it's been high, determining 0 or 1
  unsigned long now = micros();
  unsigned long dur = 0;
  // micros rolls over every 59.6 seconds
  if (now > lastInt) {
    dur = now - lastInt;
  } else {
    dur = UINT32_MAX - lastInt + now;
  }

  if (intCount < MSG_BITS) { // protect from overrun
    // 26-28us = 0, 70us = 1
    bits[intCount++] = dur > 50 ? 0x1 : 0x0;
  }

  if (intCount == MSG_BITS) { // last bit received
    detachInterrupt(ioPin);
    pinMode(ioPin, OUTPUT);
    digitalWrite(ioPin, HIGH);
    digitalWrite(ledPin, LOW);
    convertBits();
    acquiring = false;
  }
}

int RHT03::getChecksum() {
  return checksum;
}

void RHT03::convertBits() {
  uint newTemp = 0;
  uint newRH = 0;
  byte newCS = 0;

  // The first 2 interrupts start the message
  for (int i=0; i<40; i++) {
    if (i < 16) {
      newRH = newRH << 1;
      newRH |= bits[i];
    } else if (i < 32) {
      newTemp = newTemp << 1;
      newTemp |= bits[i];
    } else {
      newCS = newCS << 1;
      newCS |= bits[i];
    }
  }
  lastTemp = newTemp;
  lastRH = newRH;

  // validate
  byte sum = 0;
  byte b = newTemp & 0xff; // temp low byte
  sum += b;
  b = newTemp >> 8;
  sum += b;
  b = newRH & 0xff; // temp low byte
  sum += b;
  b = newRH >> 8;
  sum += b;

  checksum = newCS * (sum == newCS ? 1 : -1);

  lastTemp = newTemp;
  lastRH = newRH;
}
