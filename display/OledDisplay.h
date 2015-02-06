#ifndef MICRO_OLED_H
#define MICRO_OLED_H

#include "spark_wiring.h"

// Clear actions
#define CLEAR_BUFF      0x01
#define CLEAR_OLED      0x02
#define CLEAR_BOTH      0x03

#define PAGE_MIN            0x00
#define PAGE_MAX            0x05
#define COL_MIN             0x00
#define COL_MAX             0x3f

// *** OLED Commands ***
#define OLED_CHARGEPUMP       0x8D
#define OLED_EXTERNALVCC            0x01
#define OLED_SWITCHCAPVCC           0x02

#define OLED_DISPLAY_ON             0xAF
#define OLED_DISPLAY_OFF            0xAE
#define OLED_DISPLAY_NORMAL         0xA6
#define OLED_DISPLAY_INVERT         0xA7
#define OLED_DISPLAY_ALL            0xA5
#define OLED_DISPLAY_RESUME         0xA4
#define OLED_CONTRAST               0x81

#define OLED_DISPLAY_OFFSET       0xD3
#define OLED_VCOM_DESELECT        0xDB
#define OLED_DISPLAY_CLOCKDIV       0xD5
#define OLED_PRECHARGE            0xD9
#define OLED_MULTIPLEX            0xA8
#define OLED_LOW_COLUMN         0x00
#define OLED_HIGH_COLUMN        0x10
#define OLED_START_LINE         0x40

#define OLED_COM_PINS           0xDA
#define OLED_MEMORY_MODE      0x20
#define OLED_PAGE_ADDRESS       0x22
#define OLED_COLUMN_ADDRESS     0x21
#define OLED_COMSCAN_INC      0xC0
#define OLED_COMSCAN_DEC      0xC8
#define OLED_SEGMENT_REMAP        0xA0

// Scroll Commands
#define OLED_ACTIVATE_SCROLL          0x2F
#define OLED_DEACTIVATE_SCROLL            0x2E
#define OLED_VERTICAL_SCROLL_AREA           0xA3
#define OLED_RIGHT_HORIZONTAL_SCROLL      0x26
#define OLED_LEFT_HORIZONTAL_SCROLL       0x27
#define OLED_VERTICAL_RIGHT_HORIZONTAL_SCROLL 0x29
#define OLED_VERTICAL_LEFT_HORIZONTAL_SCROLL  0x2A
// *** OLED Commands ***

typedef struct {
    int pageStart;
    int pageEnd;
    int colStart;
    int colEnd;
} page_t;

typedef struct {
    int id;
    int width;
    int height;
    byte startChar;
    int totalChars;
    int mapSize;
    const byte *data;
} font_t;

class OledDisplay {
  public:
    OledDisplay(int reset, int dc, int cs);

    void begin(void);
    void end(void);

      // fundamentals
    void command(byte cmd);
    void setByte(int page, int col, byte val);
    void fill(byte val);

    void resetPage();
    void setPage(page_t page);

    // Drawing
    void clear(int mode);
    void display(void);
    void line(int begX, int begY, int endX, int endY);
    // Text
    void setFont(int fontId);
    void writeChar(int x, int y, char c);
    void writeText(int x, int y, char *text);

  private:
    void selectDevice(bool enable, bool command);
    void write(byte data);
    int rstPin;
    int dcPin;
    int csPin;
    page_t activePage;
    font_t activeFont;
    static const byte *fonts[];
};

#endif