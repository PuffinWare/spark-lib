#include "common.h"
#include <stdlib.h>
// Core
#define MICROS_MAX 59652323
// Photon
//#define MICROS_MAX 35791394

//! Calculate the duration between two timestamps from micros() and handles rollover scenarios
ulong get_duration(ulong now, ulong last) {
  // micros rolls over every 59.6 seconds, millis every 49 days
  if (now > last) {
    return now - last;
  }
  return (MICROS_MAX - last) + now;
}
