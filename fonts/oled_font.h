
#ifndef OLED_FONT_H
#define OLED_FONT_H
#include "spark_wiring.h"

typedef struct {
    int id;
    int width;
    int height;
    byte startChar;
    int totalChars;
    const byte *data;
} font_t;

font_t* parseFont(const byte *fontData);

#endif // OLED_FONT_H