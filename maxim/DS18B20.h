#ifndef DS18B20_H
#define DS18B20_H

#include <spark_wiring.h>
#include "common.h"
#include "DS2482.h"

#define MAXIM_SEARCH_ROM    0xF0
#define MAXIM_MATCH_ROM     0x55
#define MAXIM_SKIP_ROM      0xCC
#define MAXIM_READ_ROM      0x33
#define MAXIM_READ_SCRATCH  0xBE
#define MAXIM_WRITE_SCRATCH 0x4E
#define MAXIM_COPY_SCRATCH  0x48
#define MAXIM_CONVERT_T     0x44
#define MAXIM_REG_CONFIG    0xC3

static const int NUM_DEVICES = 4;

//! One Wire states
typedef enum DS18B20_STATE {
  DS18B20_NONE, // None specified
  DS18B20_START,
  DS18B20_READY,
  DS18B20_REQUEST_TEMP,
  DS18B20_1W_BUSY,
  DS18B20_READ_ROM,
  DS18B20_READ_SCRATCHPAD,
  DS18B20_READ_DEVICES,
  DS18B20_READ_RESPONSE,
  DS18B20_READ_DATA,
} DS18B20_STATE;

class DS18B20 {

public:
  DS18B20(DS2482 *owBridge, int interval=500);

  bool poll();
  void update(int device);  //! Trigger and update now
  void readRom();
  int getTempC(int device);     //! Returns temp * 10, 315 = 31.5c
  int getTempF(int device);     //! Returns temp * 10, 723 = 72.3f

private:
  DS2482 *owBridge;
  DS18B20_STATE state;
  DS18B20_STATE nextState;
  ulong interval;     //! Update interval
  ulong eventTime;    //! When did we start something
  ulong waitTime;     //! How long should we wait before next op
  int16_t reading[NUM_DEVICES];
  int len;            //! The length of the response
  int curDevice;      //! The current device being read
  byte data[9];
  byte request[12];
  const byte *address;

  void changeState(ulong wait, DS18B20_STATE next);
  void pollStatus(ulong wait, DS18B20_STATE next);
  void convertData();
  void buildRequest(byte command, bool all);
};

#endif //DS18B20_H
