#ifndef TMP3X_H
#define TMP3X_H

class TMP3x {
  public:
    TMP3x(int analogPin, int numReadings, long delay);
    void poll();
    int getTemp();

  private:
    void read();
    int tmpPin;
    int curTemp;      //! Current temp to be returend in getTemp()
    int avgIndex;     //! Index within reading array
    int avgSize;      //! Number of readings to average
    unsigned long avgDelay;     //! Time between readings
    unsigned long lastReading;  //! When last reading was taken
    int startupCount; //! Number of readings taken average
    double *readings;
};
#endif // TMP3X_H