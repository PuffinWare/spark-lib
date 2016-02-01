#include "ButtonInterrupt.h"
#include <spark_wiring_interrupts.h>

ButtonInterrupt::ButtonInterrupt(int ioPin, ulong duration, raw_btn_callback callback, ulong repeat, ulong period) {
  this->ioPin = ioPin;
  this->duration = duration;
  this->raw_callback = callback;
  this->obj_callback = NULL;
  this->repeat = repeat;
  this->period = period;

  this->mode = UP;
  this->latchTime = 0;
  this->invoke = false;

  pinMode(ioPin, INPUT_PULLUP);
  attachInterrupt(ioPin, &ButtonInterrupt::handleInterrupt, this, FALLING);
}

ButtonInterrupt::ButtonInterrupt(int ioPin, ulong duration, obj_btn_callback callback, ulong repeat, ulong period) {
  this->ioPin = ioPin;
  this->duration = duration;
  this->obj_callback = callback;
  this->raw_callback = NULL;
  this->repeat = repeat;
  this->period = period;

  this->mode = UP;
  this->latchTime = 0;
  this->invoke = false;

  pinMode(ioPin, INPUT_PULLUP);
  attachInterrupt(ioPin, &ButtonInterrupt::handleInterrupt, this, FALLING);
}

bool ButtonInterrupt::poll() {
  ulong now = millis();

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
    // Invoke the callback here, not from within the interrupt handler
    if (raw_callback != NULL) {
      raw_callback(mode);
    }
    if (obj_callback != NULL) {
      obj_callback(mode);
    }

  }

  return mode;
}

bool ButtonInterrupt::checkPinLow()  {
  if (digitalRead(ioPin) == LOW) {
    return true;
  }

  // Button released, reset and start over
  mode = UP;
  invoke = true;
  return false;
}

void ButtonInterrupt::handleInterrupt() {
  ulong now = millis();

  switch(mode) {
    case UP: // Up and waiting for an event
      latchTime = now;
      mode = LATCHED;
      break;

    default:
      break;
  }
}
