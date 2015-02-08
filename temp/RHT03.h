
#ifndef RHT03_H
#define RHT03_H
#include "spark_wiring.h"

#define MSG_BITS 40
class RHT03;

class RHT03 {
  public:
    /*! Constructor
     * @param pin the digital io pin it is connected to
     * @param readings How many readings to average for the final value
     */
    RHT03(int ioPin, int ledPin);

    void update();   //! Call this periodically to update the stored temp values
    int getTemp();  //! Returns temp * 10, 315 = 31.5c
    int getRH();    //! Returns RH * 10, 585 = 58.5rh
    int getIntCount();    //! returns number of interrupts (debugging)
    void handleInterrupt();
    // static RHT03 *thisX;

  private:
    // static void interruptHandler();
    void convertBits();  //! Convert the received bits to Temp and RH

    volatile int ioPin;      //! The IO Pin connected to the RHT03
    volatile int ledPin;     //! The IO Pin for the status LED
    volatile int lastTemp;   //! The last updated temperature
    volatile int lastRH;     //! The last updated relative humidity
    volatile bool intFirst;  //! Is this the first interrupt
    volatile bool acquiring;
    volatile int intCount;   //! The number of interrupts, and bits received
    volatile unsigned long lastInt;  //! When was the last interrupt received
    volatile byte bits[MSG_BITS];    //! Received data buffer
};

#endif // RHT03_H