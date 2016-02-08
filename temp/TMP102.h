#ifndef TMP102_H
#define TMP102_H
#include <spark_wiring.h>
#include "common.h"

#ifndef NO_LED
#define NO_LED -1
#endif

#define TMP_NORMAL  0
#define TMP_EXTENDED   1

/*!
 * TMP102 Temperature and Relative Humidity library
 * Uses an I2C interface
 */
class TMP102 {
public:
  /*! Constructor
   * @param ioPin the digital io pin it is connected to (D0 through D4)
   * @param ledPin the pin of an LED to display reading activity (for testing)
   */
  TMP102(byte address = 0x48);

  bool poll();        //! Call this on the main poll loop and it will keep the data updated
  void update();      //! Trigger and update now
  int getTempC();     //! Returns temp * 10, 315 = 31.5c
  int getTempF();     //! Returns temp * 10, 723 = 72.3f

private:
  byte address;       //! The I2C address of the device
  I2C_MODE mode;
  ulong eventTime;    //! When did we start something
  ulong waitTime;     //! How long should we wait before next op
  ulong lastUpdate;   //! When did we last get a reading
  int lastTemp;       //! The last updated temperature

  void requestReading();    //! Initiate a reading update
  void readData();          //! Read response and process
  void event(ulong wait, I2C_MODE next); //! State change
};

#endif //TMP102_H
