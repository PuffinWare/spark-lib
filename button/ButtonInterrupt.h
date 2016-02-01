#ifndef BUTTON_INTERRUPT_H
#define BUTTON_INTERRUPT_H
#include <spark_wiring.h>
#include "common.h"

typedef enum ButtonMode {
  UP,       //! Up and waiting for an event
  LATCHED,  //! First falling event, latch and wait
  FIRST,    //! Down for de-bounce time, fire first event
  REPEAT    //! Held down, start repeating
} ButtonMode;

typedef void (*raw_btn_callback) (int);
typedef std::function<void(int)> obj_btn_callback;

class ButtonInterrupt {
  public:
    ButtonInterrupt(int ioPin, ulong duration, raw_btn_callback callback=NULL, ulong repeat=0, ulong period=0);
    ButtonInterrupt(int ioPin, ulong duration, obj_btn_callback callback=NULL, ulong repeat=0, ulong period=0);

    bool poll();
    bool checkPinLow();
    void handleInterrupt();

  private:
    int ioPin;              //! The pin the button is connected to
    raw_btn_callback raw_callback;  //! Event callback
    obj_btn_callback obj_callback;  //! Event callback

    ulong duration;         //! De-bounce duration to ignore errant button states, should be short
    ulong repeat;           //! The button down time to start repeat mode - 0 = none
    ulong period;           //! The repeat period

    ulong latchTime;        //! The time the button was initially pressed
    ButtonMode mode;        //! Track the button state and repeat
    bool invoke;            //! Invoke callback flag
};

#endif // BUTTON_INTERRUPT_H