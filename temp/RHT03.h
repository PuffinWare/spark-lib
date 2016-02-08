
#ifndef RHT03_H
#define RHT03_H
#include "spark_wiring.h"

#define RHT_MSG_BITS 40

#ifndef NO_LED
#define NO_LED -1
#endif

/*!
 * RHT03 Temperature and Relative Humidity library
 * Device uses single pin bi-directional 40-bit serial communication.
 * This library uses interrupts to read bits since the duration of the pin
 * being HIGH denotes a 0 or 1 bit. Using interrupts was much cleaner than
 * delay-check-loop logic.
 */
class RHT03 {
public:
  /*! Constructor
   * @param ioPin the digital io pin it is connected to (D0 through D4)
   * @param ledPin the pin of an LED to display reading activity (for testing)
   */
  RHT03(int ioPin, int ledPin=NO_LED);

  bool poll();        //! Call this on the main poll loop and it will keep the data updated
  int getTempC();     //! Returns temp * 10, 315 = 31.5c
  int getTempF();     //! Returns temp * 10, 723 = 72.3f
  int getRH();        //! Returns RH * 10, 585 = 58.5rh
  void handleInterrupt();
  int getIntCount();  //! Debugging
  int getIgnCount();  //! Debugging

private:
  void update();            //! Initiate comm with the RHT03
  void finished();          //! Message received, process and reset
  void timeout();           //! Timeout waiting for interrupts
  void convertBits();       //! Convert the received bits to Temp and RH
  volatile int ioPin;       //! The IO Pin connected to the RHT03
  volatile int lastTemp;    //! The last updated temperature
  volatile int lastRH;      //! The last updated relative humidity
  volatile bool acquiring;  //! Currently acquiring a reading
  volatile int intCount;    //! The number of interrupts, and bits received
  volatile int ignCount;    //! Debugging
  volatile unsigned long lastInt;       //! When was the last interrupt received
  volatile unsigned long acquireStart;  //! When did we start comm
  volatile unsigned long lastUpdate;    //! When did we last acquire a reading
  volatile byte bits[RHT_MSG_BITS];    //! Received data buffer

  volatile int ledPin;      //! Testing
};

#endif // RHT03_H