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
  intCount = 0;
  for (int i=0; i<MSG_BITS; i++) {
    bits[i]=false;
  }

  // Start with a HIGH mode
  pinMode(ioPin, OUTPUT);
  digitalWrite(ioPin, HIGH);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
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

// void RHT03::interruptHandler() {

// }

void RHT03::handleInterrupt() {
  if (digitalRead(ioPin) == HIGH) {
    lastInt = micros(); // just record when it happened
    return;
  } else if (intFirst) {
    // THe first low interrupt is the preparation for sending
    intFirst = false;
    return;
  }
  digitalWrite(ledPin, LOW);

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

int RHT03::getIntCount() {
  return intCount;
}

void RHT03::convertBits() {
  int newTemp = 0;
  uint newRH = 0;
  uint checkSum = 0;

  for (int i=0; i<40; i++) {
    if (i < 8) {
      newRH = newRH << 1;
      newRH |= bits[i];
    } else if (i < 16) {
      newTemp = newTemp << 1;
      newTemp |= bits[i];
    } else {
      checkSum = checkSum << 1;
      checkSum |= bits[i];
    }
  }

  // validate
  byte sum = 0;
  byte b = 0;
  for (int i=0; i<32; i++) {
    b = b << 1;
    if (i%8 == 0) {
      sum += b;
    }
  }
  intCount = (sum == checkSum ? 1 : 0);
  lastTemp = newTemp;
  lastRH = newRH;
}
