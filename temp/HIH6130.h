#ifndef HIH6130_H
#define HIH6130_H
#include <spark_wiring.h>
#include "common.h"

#ifndef NO_LED
#define NO_LED -1
#endif

#define HIH_NORMAL  0
#define HIH_STALE   1
#define HIH_COMMAND 2

/*!
 * HIH6130 Temperature and Relative Humidity library
 * Uses an I2C interface
 */
class HIH6130 {
public:
  /*! Constructor
   * @param ioPin the digital io pin it is connected to (D0 through D4)
   * @param ledPin the pin of an LED to display reading activity (for testing)
   */
  HIH6130(byte address = 0x27, int interval=500, int tempAdjust=0);

  bool poll();        //! Call this on the main poll loop and it will keep the data updated
  void update();      //! Trigger and update now
  int getTempC();     //! Returns temp * 10, 315 = 31.5c
  int getTempF();     //! Returns temp * 10, 723 = 72.3f
  int getRH();        //! Returns RH * 10, 585 = 58.5rh

private:
  byte address;       //! The I2C address of the device
  I2C_MODE mode;
  int status;
  int interval;       //! How often to get an updated reading
  int tempAdjust;     //! Adjustment to the temp reading to calibrate
  ulong eventTime;    //! When did we start something
  ulong waitTime;     //! How long should we wait before next op
  ulong lastUpdate;   //! When did we last get a reading
  int lastTemp;       //! The last updated temperature
  int lastRH;         //! The last updated relative humidity

  void requestReading();    //! Initiate a reading update
  void readData();          //! Read response and process
  void event(ulong wait, I2C_MODE next); //! State change
};

#endif //HIH6130_H
