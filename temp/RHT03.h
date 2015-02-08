
#ifndef RHT03_H
#define RHT03_H
#include "spark_wiring.h"

#define MSG_BITS 40
class RHT03;

/*!
 * RHT03 Temperature and Relative Humidity library
 * Device uses single pin bi-directional 40-bit serial
 * communication. This library uses interrupts to read
 * bits since the duration of the pin being HIGH denotes
 * a 0 or 1 bit. Using interruptes was much cleaner than
 * delay-check-loop logic.
 */
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
    volatile bool acquiring; //! Currently acquiring a reading
    volatile int intCount;   //! The number of interrupts, and bits received
    volatile unsigned long lastInt;  //! When was the last interrupt received
    volatile byte bits[MSG_BITS];    //! Received data buffer
};

#endif // RHT03_H