#include "ButtonInterrupt.h"
#include <spark_wiring_interrupts.h>

ButtonInterrupt::ButtonInterrupt(int ioPin, ulong duration, callback_t callback, ulong repeat, ulong period) {
  this->ioPin = ioPin;
  this->duration = duration;
  this->callback = callback;
  this->repeat = repeat;
  this->period = period;

  this->mode = UP;
  this->latchTime = 0;
  this->invoke = false;

  RGB.color(0,255,255);
  pinMode(ioPin, INPUT_PULLUP);
  attachInterrupt(ioPin, &ButtonInterrupt::handleInterrupt, this, FALLING);
}

bool ButtonInterrupt::poll() {
  ulong now = millis();
  bool result = false;

  switch(mode) {
    case LATCHED: // Down and first press, de-bounce short presses
      if ((now - latchTime) > duration) {
        if (checkPinLow()) {
          mode = FIRST;
          latchTime = now;
          invoke = true;
        }
      }
      break;

    case FIRST: // Down and first press, wait for repeat hold
      if (checkPinLow() && repeat > 0 && (now - latchTime) > repeat) {
        RGB.color(0,0,255);
        mode = REPEAT;
        latchTime = now;
        invoke = true;
      }
      break;

    case REPEAT: // Held down, start repeating
      if (checkPinLow() && period > 0 && (now - latchTime) > period) {
        latchTime = millis();
        invoke = true;
      }
      break;

    default:
      break;
  }

  if (invoke) {
    invoke = false;
    result = true;
    // Invoke the callback here, not from within the interrupt handler
    if (callback != NULL) {
      callback();
    }
  }

  return result;
}

bool ButtonInterrupt::checkPinLow()  {
  if (digitalRead(ioPin) == LOW) {
    return true;
  }

  // Button released, reset and start over
  RGB.color(255, 0, 0);
  mode = UP;
  return false;
}

void ButtonInterrupt::handleInterrupt() {
  ulong now = millis();
//#ifdef PUFFIN_DEBUG
  RGB.color(0,255,0);
//#endif

  switch(mode) {
    case UP: // Up and waiting for an event
      latchTime = now;
      mode = LATCHED;
      break;

    default:
      break;
  }
}
