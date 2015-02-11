#include "RHT03.h"
#include "spark_wiring.h"

#define ACQUIRE_TIMEOUT  1000
#define READING_DELAY  5000

// linker issues forced me to do this... probably a better way?
static RHT03 *thisX;
void interruptHandler() {
  thisX->handleInterrupt();
}

RHT03::RHT03(int ioPin, int ledPin) {
  thisX = this;
  this->ioPin = ioPin;
  this->ledPin = ledPin;

  acquiring = false;
  acquireStart = millis();
  lastTemp = 0;
  lastRH = 0;
  intCount = 0;

  // Start with a HIGH mode
  pinMode(ioPin, OUTPUT);
  digitalWrite(ioPin, HIGH);
  if (ledPin != NO_LED) {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
  }
}

int RHT03::getTempC() {
  return lastTemp;
}

int RHT03::getTempF() {
  double t = lastTemp;
  return int(t * 1.8) + 320;
}

int RHT03::getRH() {
  return lastRH;
}

bool RHT03::poll() {
  bool result = false;
  if (acquiring) {
    if (intCount >= MSG_BITS) { // last bit received
      detachInterrupt(ioPin);
      pinMode(ioPin, OUTPUT);
      digitalWrite(ioPin, HIGH);
      if (ledPin != NO_LED) { digitalWrite(ledPin, LOW); }
      convertBits();
      acquiring = false;
      intCount = 0;
      result = true;
    } else if (getDuration(millis(), acquireStart) > ACQUIRE_TIMEOUT) { // timeout
      if (ledPin != NO_LED) { digitalWrite(ledPin, LOW); }
      detachInterrupt(ioPin);
      acquiring = false;
    }
    return result;
  }

  if (getDuration(millis(), acquireStart) > READING_DELAY) {
    update(); // trigger an update
  }
  return result;
}

void RHT03::update() {
  if (acquiring) {
    return;
  }
  if (ledPin != NO_LED) { digitalWrite(ledPin, HIGH); }

  acquiring = true;
  acquireStart = millis();
  intCount = 0;
  ignCount = 0;
  lastInt = micros();
  digitalWrite(ioPin, LOW);
  delay(2); // pull low for 1-10ms
  digitalWrite(ioPin, HIGH);
  delayMicroseconds(10); // pull high then it will pull low in 20-40us
  // Prepare for reading
  pinMode(ioPin, INPUT_PULLUP);
  attachInterrupt(ioPin, interruptHandler, FALLING);
}

/*!
 * Once the device is triggered to transmit, there will be 3 pulse types:
 * - low 80us -> high 80us - start sending  (160us)
 * - low 50us -> high 26-28us - 0 bit       (76-78Us)
 * - low 50us -> high 70us - 1 bit          (129us)
 * These durations between falling edges determine the data type
 */
void RHT03::handleInterrupt() {
  unsigned long now = micros();
  // Falling edge tells us how long it's been high, determining 0 or 1
  unsigned long dur = getDuration(now, lastInt);
  lastInt = now;

  // First falling edge is the device starting upg, the second is it's start msg
  // Both are ignored, but sometimes we do not receive the first.
  if (dur > 150) {
    ignCount += 1;
    return;
  }

  if (intCount < MSG_BITS) { // protect from overrun
    bits[intCount] = dur < 100 ? 0x0 : 0x1;
    intCount++; // must be separate
  }
}

void RHT03::convertBits() {
  uint newTemp = 0;
  uint newRH = 0;
  byte newCS = 0;

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

  // validate
  byte sum = newTemp & 0xff;  // temp low byte
  sum += newTemp >> 8;        // temp high byte
  sum += newRH & 0xff;        // rh low byte
  sum += newRH >> 8;          // rh high byte

  if (sum == newCS) {
    lastTemp = newTemp;
    lastRH = newRH;
  } else {
    ignCount *= -1;
  }
}

unsigned long RHT03::getDuration(unsigned long now, unsigned long last) {
  // micros rolls over every 59.6 seconds, millis every 49 days
  if (now > last) {
    return now - last;
  }
  return (UINT32_MAX - last) + now;
}

int RHT03::getIntCount() {
  return intCount;
}

int RHT03::getIgnCount() {
  return ignCount;
}
