// John Sokel
#include "OledDisplay.h"

//#include "stdlib.h"
#include "spark_wiring_spi.h"
#include "rgbled.h"

#include "font_6x8.h"
#include "font_14x16.h"
#include "font_11x16_num.h"
#include "font_lcd11x16.h"
#include "font_8x16.h"
#include "font_8x16_bold.h"
#include "font_12x16_bold.h"
#include "font_12x24_num.h"
#include "font_12x32_num.h"

static byte screen_buf[] = {
  /* LCD Memory organised in 64 horizontal pixel and 6 rows of byte
   B  B .............B  -----
   y  y .............y        \
   t  t .............t         \
   e  e .............e          \
   0  1 .............63          \
                                  \
   D0 D0.............D0            \
   D1 D1.............D1            / ROW 0
   D2 D2.............D2           /
   D3 D3.............D3          /
   D4 D4.............D4         /
   D5 D5.............D5        /
   D6 D6.............D6       /
   D7 D7.............D7  ----
  */
  // Puffin LOGO
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80,
  0xc0, 0xe0, 0xf0, 0xf0, 0xf8, 0x78, 0xfc, 0xfc, 0xfe, 0x3e, 0x1e, 0xcf, 0x0f, 0x07, 0x87, 0x87,
  0x87, 0x87, 0x87, 0x07, 0x87, 0x87, 0x0e, 0x0e, 0x0c, 0x1c, 0x1c, 0x38, 0x30, 0x70, 0xe0, 0xc0,
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xf0, 0xfc, 0xfe, 0xdf, 0xcf,
  0x87, 0x83, 0x81, 0x00, 0x00, 0x00, 0x3f, 0x7f, 0xff, 0xf8, 0xe0, 0x8f, 0x30, 0x40, 0x80, 0x00,
  0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x07, 0x02, 0x10, 0x30, 0x20, 0x60, 0xc0, 0x80, 0x80, 0xe1,
  0xff, 0xff, 0xfe, 0xfc, 0xf8, 0xe0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf8, 0xff, 0xff, 0x0f, 0x01, 0x01, 0x03, 0x07,
  0x07, 0x07, 0x07, 0x0f, 0x0f, 0x1e, 0x3e, 0x7c, 0xf8, 0xf3, 0xe7, 0xcf, 0x9f, 0x1e, 0x3c, 0x3d,
  0x3a, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x32, 0x65, 0xe5, 0xc5, 0x0c, 0x0e, 0x0f, 0x0f, 0x1f,
  0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xfe, 0xc0, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0f, 0x1f, 0x7f, 0xfe, 0xf0,
  0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x1f, 0xff, 0xf8, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x0f, 0x3f, 0x7e, 0xf8, 0xf0,
  0xe0, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xff,
  0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x83, 0xff, 0xff, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
  0x03, 0x07, 0x0f, 0x0f, 0x1e, 0x1c, 0x3c, 0x38, 0x78, 0x70, 0xf0, 0xe0, 0xe0, 0xf0, 0xfc, 0x7f,
  0x3f, 0x87, 0x80, 0xc0, 0xc0, 0x40, 0x60, 0x60, 0x30, 0x38, 0x1e, 0x0f, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
 };

// The display controller is actually 128x64 but the display is 64x48
// So we need to draw in the middle of the logical display.
// All page updates must use the same offset.
const page_t FULL_PAGE = {0, 5, 32, 95};

const byte *OledDisplay::fonts[] = {
   FONT_6X8,
   FONT_14X16,
   FONT_8X16,
   FONT_8X16_BOLD,
   FONT_12X16_BOLD,
   FONT_11X16_NUM,
   FONT_LCD11X16,
   FONT_12X24_NUM,
   FONT_12X32_NUM
};

OledDisplay::OledDisplay(int reset, int dc, int cs) {
    rstPin = reset;
    dcPin = dc;
    csPin = cs;
}

void OledDisplay::begin() {
  pinMode(rstPin, OUTPUT);
  pinMode(dcPin, OUTPUT);
  pinMode(csPin, OUTPUT);

  // Default them high
  digitalWrite(dcPin, HIGH);
  digitalWrite(csPin, HIGH);

    // Display reset
  digitalWrite(rstPin, HIGH);
  delay(5);
  digitalWrite(rstPin, LOW);
  delay(10); // Needs to stay low for 10ms
  digitalWrite(rstPin, HIGH);

  // Init SPI
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  SPI.setDataMode(SPI_MODE0);

  selectDevice(true, true);

  // Display Init sequence
  write(OLED_DISPLAY_OFF);

  write(OLED_DISPLAY_CLOCKDIV);
  write(0x80); // the suggested ratio 0x80

  write(OLED_MULTIPLEX);
  write(0x2F);

  write(OLED_DISPLAY_OFFSET);
  write(0x0); // no offset

  write(OLED_CHARGEPUMP);
  write(0x14); // charge pump enable

  write(OLED_START_LINE | 0x0);
  write(OLED_DISPLAY_NORMAL);
  write(OLED_DISPLAY_RESUME);

  write(OLED_SEGMENT_REMAP | 0x1);
  write(OLED_COMSCAN_DEC);

  write(OLED_MEMORY_MODE);
  write(0x00); // Horizontal

  write(OLED_COM_PINS);
  write(0x12);

  write(OLED_CONTRAST);
  write(0x8F);

  write(OLED_PRECHARGE);
  write(0xF1);

  write(OLED_VCOM_DESELECT);
  write(0x40);

  write(OLED_DISPLAY_ON);

  setPage(FULL_PAGE);
  setFont(0);
  display();
}

void OledDisplay::end() {
  selectDevice(true, true);
  write(OLED_DISPLAY_OFF);
  write(OLED_CHARGEPUMP);
  write(0x10); // charge pump enable
  selectDevice(false, false);

  SPI.end();
}

void OledDisplay::resetPage() {
  setPage(FULL_PAGE);
}

void OledDisplay::setPage(page_t page) {
  activePage.pageStart = page.pageStart;
  activePage.pageEnd = page.pageEnd;
  activePage.colStart = page.colStart;
  activePage.colEnd = page.colEnd;

  selectDevice(true, true);
  write(OLED_PAGE_ADDRESS);
  write(page.pageStart);
  write(page.pageEnd);

  write(OLED_COLUMN_ADDRESS);
  write(page.colStart);
  write(page.colEnd);
  selectDevice(false, true);
}

void OledDisplay::command(byte cmd) {
  selectDevice(true, true);
  SPI.transfer(cmd);
  selectDevice(false, true);
}

void OledDisplay::setByte(int page, int col, byte val) {
  screen_buf[page*64 + col] = val;
}

void OledDisplay::fill(byte val) {
  for (int i=0; i<384; i++) {
    screen_buf[i] = val;
  }
}

void OledDisplay::clear(int mode) {
  bool clearBuff = (mode & 0x01) == 0x01;
  bool clearOled = (mode & 0x02) == 0x02;

  if (clearBuff) {
    memset(screen_buf, 0x00, 384); // (64 x 48) / 8 = 384
  }
  if (clearOled) {
    resetPage(); // Do I need this?
    selectDevice(true, false);
    for (int i=0; i<=PAGE_MAX; i++) {
      for (int j=0; j<=COL_MAX; j++) {
        SPI.transfer(0x00);
      }
    }
    selectDevice(false, true);
  }
}

void OledDisplay::setFont(int fontId) {
  const byte *font = fonts[fontId];
  activeFont.id = fontId;
  activeFont.width = *font;
  activeFont.height = *(font+1);
  activeFont.startChar = *(font+2);
  activeFont.totalChars = *(font+3);
  activeFont.data = font+4;
}

void OledDisplay::writeChar(int x, int y, const char c) {
  // for now, this is using standard pages for rows
  const byte *font = activeFont.data;

  int cols = x * activeFont.width;
  int numPages = activeFont.height / 8;
  int charOffset = c - activeFont.startChar;

  page_t page = {
    y * numPages,
    (y * numPages) + numPages - 1,
    FULL_PAGE.colStart + cols,
    FULL_PAGE.colStart + cols + activeFont.width - 1
  };
  setPage(page);

  int offset = charOffset * (activeFont.width * numPages);
  selectDevice(true, false);
  for (int i=0; i<numPages; i++) {
    for (int j=0; j<activeFont.width; j++) {
      SPI.transfer(*(font + offset++));
    }
  }
  selectDevice(false, true);
  setPage(activePage);
}
void OledDisplay::writeText(int x, int y, const char *text) {
  int i = x;
  for (uint j=0; j<strlen(text); j++) {
    writeChar(i++, y, *(text + j));
  }
}

void OledDisplay::display(void) {
  resetPage();
  selectDevice(true, false);
  for (int i=0; i<384; i++)  {
    SPI.transfer(screen_buf[i]);
  }
  selectDevice(false, true);
}

void OledDisplay::write(byte data) {
  SPI.transfer(data);
}


void OledDisplay::line(int begX, int begY, int endX, int endY) {
}

void OledDisplay::selectDevice(bool enable, bool command) {
  digitalWrite(dcPin, command ? LOW : HIGH);
  digitalWrite(csPin, enable  ? LOW : HIGH);
}
