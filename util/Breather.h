#ifndef BREATHER_H
#define BREATHER_H

#include "common.h"

class Breather {

public:
  Breather(int ioPin, int step, int delay);
  void loop(ulong now=0);
  void enable();
  void disable();

private:
  int ioPin;
  int delay;
  int step;
  bool enabled;
  int breatheMode;   //! Duty cycle
  ulong breatheTime; //! When to change breath duty cycle
};

#endif // BREATHER_H
