#include "oled_font.h"

font_t* parseFont(const byte *fontData) {
  font_t *font = (font_t *)malloc(sizeof(font_t));
  font->width = *fontData;
  font->height = *(fontData + 1);
  font->startChar = *(fontData + 2);
  font->totalChars = *(fontData + 3);
  font->data = fontData+4;
  return font;
}