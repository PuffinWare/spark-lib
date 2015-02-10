#include "TMP3x.h"
#include "spark_wiring.h"

TMP3x::TMP3x(int analogPin, int numReadings, long delay) {
  tmpPin = analogPin;
  avgSize = numReadings;
  avgDelay = delay;

  avgIndex = 0;
  curTemp = 0;
  startupCount = 0;
  readings = (double*)malloc(avgSize * sizeof(double));

}

void TMP3x::poll() {
  unsigned long now = millis();
  unsigned long dur = (now > lastReading)
                    ? now - lastReading
                    : (UINT32_MAX - lastReading) + now;

  if (dur > avgDelay) {
    read();
    lastReading = now;
  }
}

int TMP3x::getTemp() {
  return curTemp;
}

void TMP3x::read() {
  double reading = analogRead(tmpPin);
  double volts = (reading * 3.3) / 4095;
  readings[avgIndex++] = (volts - 0.5) * 100;
  if (avgIndex == avgSize) {
    avgIndex = 0;
  }
  int valsToAvg = 0;
  if (startupCount < avgSize) {
    valsToAvg = startupCount;
    startupCount++;
  } else {
    valsToAvg = avgSize;
  }

  double newTemp = 0;
  for (int i=0; i<valsToAvg; i++) {
    newTemp += readings[i];
  }
  newTemp /= valsToAvg;
  curTemp = int(newTemp * 10);
}
