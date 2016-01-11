#include "common.h"

//! Calculate the duration between two timestamps from either millis() or micros() and handle
// rollover scenarios
ulong get_duration(ulong now, ulong last) {
  // micros rolls over every 59.6 seconds, millis every 49 days
  if (now > last) {
    return now - last;
  }
  return (UINT32_MAX - last) + now;
}
