#ifndef DS2482_H
#define DS2482_H
#include <spark_wiring.h>
#include "common.h"

// Registers
#define MAXIM_REG_STATUS      0xF0
#define MAXIM_REG_READ_DATA   0xE1
#define MAXIM_REG_CHAN_SEL    0xD2
#define MAXIM_REG_CONFIG      0xC3

// Commands
#define MAXIM_DEVICE_RESET    0xF0
#define MAXIM_SET_READ_PTR    0xE1
#define MAXIM_WRITE_CONFIG    0xD2
#define MAXIM_CHANNEL_SELECT  0xC3
#define MAXIM_1WR_RESET       0xB4
#define MAXIM_1WR_SINGLE_BIT  0x87
#define MAXIM_1WR_WRITE_BYTE  0xA5
#define MAXIM_1WR_READ_BYTE   0x96
#define MAXIM_1WR_TRIPLET     0x78

//! One Wire states
typedef enum DS_STATE {
  DS_NONE, // None specified
  DS_START,
  DS_DEV_RESET,
  DS_1W_RESET,
  DS_CONFIG,
  DS_READ_CONFIG,
  DS_READY,
  DS_POLL_1W_BUSY,
  DS_1W_REQUEST,
  DS_1W_RESPONSE,
  DS_RESPONSE
} DS_STATE;

//! DS Status flags
typedef struct {
  bool oneWireBusy;
  bool presencePulse;
  bool shortDetected;
  bool logicLevel;
  bool deviceReset;
  bool singleBitResult;
  bool tripletSecondBit;
  bool branchDirTaken;
} DS_STATUS;

typedef struct {
  DS_STATE from;
  byte action;
  ulong wait;
  DS_STATE to;
} STATE_ENTRY;

/*!
 * DS2482 I2C to 1 Wire Bridge
 */
class DS2482 {
public:
  /*! Constructor
   * @param address The I2C address of the DS2482 (selectable)
   */
  DS2482(byte address = 0x18);
  DS_STATE poll();
  bool oneWireComplete();
  void writeTo1W(byte data);
  void writeTo1W(const byte *data, int len);
  void readFrom1W(int len);
  void copyBuffer(byte *dest, int len);

private:
  DS_STATE state;
  DS_STATE nextState;
  DS_STATUS status;
  ulong eventTime;    //! When did we start something
  ulong waitTime;     //! How long should we wait before next op

  byte address;   //! The I2C address of the device
  byte idx;
  byte len;
  byte request[8];   //! The 1W request data to send
  byte response[16]; //! The 1W response data received
  bool readyLogged;  //! For debugging

  void changeState(ulong wait, DS_STATE next);
  void pollStatus(ulong wait, DS_STATE next);
  void readConfig(bool setPtr);
  void readStatus(bool setPtr);
  void send(byte command, bool stop);
  void sendData(byte command, byte data, bool stop);
  void readByte();
  void writeConfig(bool ows, bool spu, bool apu);
};

#endif //HIH6130_H
