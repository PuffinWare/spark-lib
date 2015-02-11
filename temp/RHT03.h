
#ifndef RHT03_H
#define RHT03_H
#include "spark_wiring.h"

#define MSG_BITS 40 // 40 data bits

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
     * @param pin the digital io pin it is connected to (D0 through D4)
     * @param readings How many readings to average for the final value
     */
    RHT03(int ioPin, int ledPin);

    void poll();    //! Call this on the main poll loop and it will keep the data updated
    void update();  //! Call this periodically to manually update the values
    int getTempC();  //! Returns temp * 10, 315 = 31.5c
    int getTempF();  //! Returns temp * 10, 723 = 72.3f
    int getRH();    //! Returns RH * 10, 585 = 58.5rh
    void handleInterrupt();
    int getIntCount();
    int getIgnCount();

  private:
    void convertBits();  //! Convert the received bits to Temp and RH
    unsigned long getDuration(unsigned long now, unsigned long last);
    volatile int ioPin;      //! The IO Pin connected to the RHT03
    volatile int lastTemp;   //! The last updated temperature
    volatile int lastRH;     //! The last updated relative humidity
    volatile bool acquiring; //! Currently acquiring a reading
    volatile int intCount;   //! The number of interrupts, and bits received
    volatile int ignCount;   //! Debugging
    volatile unsigned long lastInt;       //! When was the last interrupt received
    volatile unsigned long acquireStart;  //! When did we start comm
    volatile unsigned long lastUpdate;    //! When did we start comm
    volatile byte bits[MSG_BITS];    //! Received data buffer

    volatile int ledPin;     //! Debugging
};

#endif // RHT03_H