#include "Blinker.h"
#include <spark_wiring.h>

Blinker::Blinker(int ioPin) {
  this->ioPin = ioPin;
  this->enabled = false;
  this->delayOn = 0;
  this->delayOff = 0;
  this->blinkCount = 0;
  this->blinkTime = 0;

  pinMode(ioPin, OUTPUT);
  digitalWrite(ioPin, LOW);
}

void Blinker::trigger(int blinks, int onTime, int offTime) {
  enabled = true;
  blinkCount = blinks;
  delayOn = onTime;
  delayOff = offTime;
  blinkTime = millis() + onTime;
  blinkMode = true;
  digitalWrite(ioPin, HIGH);
}

void Blinker::disable() {
  enabled = false;
  digitalWrite(ioPin, LOW);
}

void Blinker::loop(ulong now) {
  if (!enabled) {
    return;
  }

  ulong curTime = now==0 ? millis() : now;
  if (curTime < blinkTime) {
    return;
  }

  blinkMode = !blinkMode;
  if (blinkMode) {
    digitalWrite(ioPin, HIGH);
    blinkTime = curTime + delayOn;
  } else {
    digitalWrite(ioPin, LOW);
    blinkTime = curTime + delayOff;
    blinkCount -= 1;
    enabled = blinkCount > 0;
  }
}

