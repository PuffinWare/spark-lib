#ifndef BREATHER_H
#define BREATHER_H

#include "common.h"

class Blinker {

public:
  Blinker(int ioPin);
//  Blinker(int ioPin, int blinks=2, int onTime=250, int offTime=500);
  void loop(ulong now=0);
  void trigger(int blinks, int onTime, int offTime);
  void disable();

private:
  int ioPin;
  int delayOn;
  int delayOff;
  bool enabled;
  int blinkCount;
  bool blinkMode;
  ulong blinkTime;
};

#endif // BREATHER_H
