#include "Breather.h"
#include <spark_wiring.h>

Breather::Breather(int ioPin, int step, int delay) {
  this->ioPin = ioPin;
  this->delay = delay;
  this->step = step;
  this->enabled = false;
  this->breatheMode = 0;
  this->breatheTime = 0;

  pinMode(ioPin, OUTPUT);
  digitalWrite(ioPin, LOW);
}

void Breather::enable() {
  enabled = true;
  breatheTime = millis();
}

void Breather::disable() {
  enabled = false;
  digitalWrite(ioPin, LOW);
}

void Breather::loop(ulong now) {
  if (!enabled) {
    return;
  }

  ulong curTime = now==0 ? millis() : now;
  if (now < breatheTime) {
    return;
  }

  breatheTime = curTime + delay;
  breatheMode += step;
  if (breatheMode > 255) {
    breatheMode = -255;
  }
  int dutyCycle = abs(breatheMode);
  analogWrite(ioPin, dutyCycle<10 ? 10 : dutyCycle);
}

