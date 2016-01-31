#include "ButtonInterrupt.h"
#include <spark_wiring_interrupts.h>

ButtonInterrupt::ButtonInterrupt(int ioPin, ulong duration, callback_t callback, ulong repeat, ulong period) {
  this->ioPin = ioPin;
  this->duration = duration;
  this->callback = callback;
  this->repeat = repeat;
  this->period = period;

  this->latchTime = 0;
  this->invoke = false;

  pinMode(ioPin, INPUT_PULLUP);
  attachInterrupt(ioPin, &ButtonInterrupt::handleInterrupt, this, FALLING);
}

bool ButtonInterrupt::poll() {
  int state = digitalRead(ioPin);
  ulong now = millis();
  bool result = false;

  switch(mode) {
    case LATCHED: // Down and first press, de-bounce short presses
      if (get_duration(now, latchTime) > duration) {
        // ignore events during de-bounce time
        break;
      } else {
        if (state == HIGH) {
          // Button release, reset and start over
          mode = UP;
        } else {
          // needed?
          mode = FIRST;
          latchTime = now;
        }
      }
      break;

    case FIRST: // Down and first press, wait for repeat hold
      if (repeat > 0 && get_duration(now, latchTime) > repeat) {
        mode = REPEAT;
        latchTime = now;
        invoke = true;
      }
      break;

    case REPEAT: // Held down, start repeating
      if (period > 0 && get_duration(now, latchTime) > period) {
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

void ButtonInterrupt::handleInterrupt() {
  int state = digitalRead(ioPin);
  ulong now = millis();

  switch(mode) {
    case UP: // Up and waiting for an event
      if (state == LOW) {
        latchTime = now;
        mode = LATCHED;
        invoke = true; // invoke the callback, but not here
      }
      break;

    case LATCHED: // First LOW encountered... ignore others within duration
      if (get_duration(now, latchTime) < duration) {
        // ignore events during de-bounce time
        break;
      }
      // fall through

    case FIRST: // Down and first press, de-bounce short presses
    case REPEAT: // Held down, start repeating
      if (state == HIGH) { // Button released, start over
        mode = UP;
      }
      break;
  }
}
