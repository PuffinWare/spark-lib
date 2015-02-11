#include "TMP3x.h"
#include "spark_wiring.h"

TMP3x::TMP3x(int analogPin, int numReadings, long delay) {
  tmpPin = analogPin;
  maxReadings = numReadings;
  readingDelay = delay;

  avgIndex = 0;
  curTemp = 0;
  readingsToAvg = 0;
  readings = (double*)malloc(maxReadings * sizeof(double));
}

void TMP3x::poll() {
  unsigned long now = millis();
  unsigned long dur = (now > lastReading)
                    ? now - lastReading
                    : (UINT32_MAX - lastReading) + now;

  if (dur > readingDelay) {
    read();
    lastReading = now;
  }
}

int TMP3x::getTempC() {
  return int(curTemp * 10);
}

int TMP3x::getTempF() {
  return int(curTemp * 18) + 320;
}

void TMP3x::read() {
  double reading = analogRead(tmpPin);
  double volts = (reading * 3.3) / 4095;
  readings[avgIndex++] = (volts - 0.5) * 100;

  if (avgIndex == maxReadings) {
    avgIndex = 0;
  }
  // during startup, there will only be a few readings
  if (readingsToAvg < maxReadings) {
    readingsToAvg++;
  }

  double newTemp = 0;
  for (int i=0; i<readingsToAvg; i++) {
    newTemp += readings[i];
  }
  curTemp = newTemp / readingsToAvg;
}
