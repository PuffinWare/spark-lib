#include "RHT03.h"
#include "spark_wiring.h"

#define ACQUIRE_TIMEOUT  1000
#define READING_DELAY  5000

// linker issues forced me to do this... probably a better way?
static RHT03 *thisX;
void interruptHandler() {
  thisX->handleInterrupt();
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
RHT03::RHT03(int ioPin, int ledPin) {
  thisX = this;
  this->ioPin = ioPin;
  this->ledPin = ledPin;

  acquiring = false;
  acquireStart = millis();
  lastTemp = 0;
  lastRH = 0;
  intCount = 0;
  // for (int i=0; i<MSG_BITS; i++) {
  //   bits[i]=0x0;
  // }

  // Start with a HIGH mode
  pinMode(ioPin, OUTPUT);
  digitalWrite(ioPin, HIGH);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void RHT03::poll() {
  if (acquiring) {
    if (intCount >= MSG_BITS) { // last bit received
      detachInterrupt(ioPin);
      pinMode(ioPin, OUTPUT);
      digitalWrite(ioPin, HIGH);
      digitalWrite(ledPin, LOW);
      convertBits();
      acquiring = false;
      intCount = 0;
    } else if (getDuration(millis(), acquireStart) > ACQUIRE_TIMEOUT) {
      digitalWrite(ledPin, LOW);
      detachInterrupt(ioPin);
      acquiring = false;
    }
    return;
  }

  if (getDuration(millis(), acquireStart) > READING_DELAY) {
    update();
    return;
  }

}

void RHT03::update() {
  if (acquiring) {
    return;
  }
  digitalWrite(ledPin, HIGH);

  acquiring = true;
  acquireStart = millis();
  intCount = 0;
  ignCount = 0;
  lastInt = micros();
  digitalWrite(ioPin, LOW);
  delay(2); // pull low for 1-10ms
  digitalWrite(ioPin, HIGH);
  delayMicroseconds(10); // pull high for 20-40us
  // Then prepare for reading
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
  if (lastInt == 0) { // first falling event, ignore
    lastInt = now;
    ignCount += 1;
    return;
  }

  // Falling edge tells us how long it's been high, determining 0 or 1
  unsigned long dur = getDuration(now, lastInt);
  lastInt = now;

  if (dur > 150) { // start msg
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
