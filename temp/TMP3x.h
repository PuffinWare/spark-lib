#ifndef TMP3X_H
#define TMP3X_H

class TMP3x {
  public:
    TMP3x(int analogPin, int numReadings, long delay);
    bool poll();
    int getTempC();    //! Returns temp * 10, 315 = 31.5c
    int getTempF();    //! Returns temp * 10, 723 = 72.3f

  private:
    void read();
    int tmpPin;
    double curTemp;      //! Current temp to be returend in getTemp()
    int avgIndex;     //! Index within reading array
    int maxReadings;
    int readingsToAvg;
    unsigned long readingDelay; //! Time between readings
    unsigned long lastReading;  //! When last reading was taken
    double *readings;
};
#endif // TMP3X_H