
#ifndef FONT_11X16_NUM_H
#define FONT_11X16_NUM_H

static const byte FONT_11X16_NUM[] = {
  // width px, height px, startchar, numchars,
  11, 16, 0x20, 32,

  // font data:  !"#$%&'()*+,-./0123456789:;<=>?
  // 0x20 | (space)
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x21 | !
  0x00, 0x00, 0x00, 0x00, 0xfe, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x77, 0x77, 0x77, 0x00, 0x00, 0x00, 0x00, 
  // 0x22 | "
  0x00, 0x00, 0x00, 0x7c, 0x7c, 0x00, 0x7c, 0x7c, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x23 | #
  0x30, 0x30, 0xfe, 0xfe, 0x30, 0x30, 0xfe, 0xfe, 0x30, 0x30, 0x00, 
  0x0c, 0x0c, 0x7f, 0x7f, 0x0c, 0x0c, 0x7f, 0x7f, 0x0c, 0x0c, 0x00, 
  // 0x24 | $
  0x78, 0xfc, 0xce, 0x86, 0xff, 0xff, 0x86, 0x8e, 0x1c, 0x18, 0x00, 
  0x18, 0x38, 0x71, 0x61, 0xff, 0xff, 0x61, 0x73, 0x3f, 0x1e, 0x00, 
  // 0x25 | %
  0x1c, 0x36, 0x36, 0x36, 0x9c, 0xc0, 0x60, 0x38, 0x1e, 0x06, 0x00, 
  0x60, 0x78, 0x1c, 0x06, 0x03, 0x39, 0x6c, 0x6c, 0x6c, 0x38, 0x00, 
  // 0x26 | &
  0x38, 0x7c, 0xee, 0xc6, 0xc6, 0xee, 0x7c, 0x38, 0x00, 0x00, 0x00, 
  0x1e, 0x3f, 0x71, 0x60, 0x60, 0x61, 0x73, 0x3e, 0x1c, 0x37, 0x33, 
  // 0x27 | '
  0x00, 0x00, 0x00, 0x80, 0xfc, 0x7c, 0x3c, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x28 | (
  0x00, 0x00, 0x00, 0xe0, 0xf8, 0x1e, 0x07, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x07, 0x1f, 0x78, 0xe0, 0x80, 0x00, 0x00, 0x00, 
  // 0x29 | )
  0x00, 0x00, 0x00, 0x01, 0x07, 0x1e, 0xf8, 0xe0, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x80, 0xe0, 0x78, 0x1f, 0x07, 0x00, 0x00, 0x00, 
  // 0x2a | *
  0x08, 0x18, 0x30, 0x60, 0xfc, 0xfc, 0x60, 0x30, 0x18, 0x08, 0x00, 
  0x08, 0x0c, 0x06, 0x03, 0x1f, 0x1f, 0x03, 0x06, 0x0c, 0x08, 0x00, 
  // 0x2b | +
  0x80, 0x80, 0x80, 0x80, 0xf8, 0xf8, 0x80, 0x80, 0x80, 0x80, 0x00, 
  0x01, 0x01, 0x01, 0x01, 0x1f, 0x1f, 0x01, 0x01, 0x01, 0x01, 0x00, 
  // 0x2c | ,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xb8, 0xf8, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x2d | -
  0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x00, 0x00, 
  0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 
  // 0x2e | .
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x2f | /
  0x00, 0x00, 0x00, 0x00, 0xc0, 0xf8, 0x3f, 0x07, 0x00, 0x00, 0x00, 
  0x00, 0xe0, 0xfc, 0x1f, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x30 | 0
  0xf8, 0xfc, 0x0e, 0x06, 0x06, 0xc6, 0x36, 0x0e, 0xfc, 0xf8, 0x00, 
  0x1f, 0x3f, 0x70, 0x6c, 0x63, 0x60, 0x60, 0x70, 0x3f, 0x1f, 0x00, 
  // 0x31 | 1
  0x00, 0x00, 0x00, 0x0c, 0xfe, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x60, 0x7f, 0x7f, 0x60, 0x00, 0x00, 0x00, 0x00, 
  // 0x32 | 2
  0x18, 0x1c, 0x0e, 0x06, 0x06, 0x86, 0x86, 0xce, 0x7c, 0x38, 0x00, 
  0x78, 0x7c, 0x6e, 0x67, 0x63, 0x61, 0x61, 0x60, 0x60, 0x60, 0x00, 
  // 0x33 | 3
  0x18, 0x1c, 0x06, 0x86, 0x86, 0x86, 0x86, 0xce, 0x7c, 0x38, 0x00, 
  0x18, 0x38, 0x60, 0x61, 0x61, 0x61, 0x61, 0x73, 0x3f, 0x1e, 0x00, 
  // 0x34 | 4
  0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0xfe, 0xfe, 0x00, 0x00, 
  0x0f, 0x0f, 0x0d, 0x0c, 0x0c, 0x0c, 0x0c, 0x7f, 0x7f, 0x0c, 0x00, 
  // 0x35 | 5
  0xfe, 0xfe, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0xc6, 0x86, 0x06, 0x00, 
  0x18, 0x38, 0x70, 0x60, 0x60, 0x60, 0x60, 0x71, 0x3f, 0x1f, 0x00, 
  // 0x36 | 6
  0xf8, 0xfc, 0x8e, 0xc6, 0xc6, 0xc6, 0xc6, 0xce, 0x9c, 0x18, 0x00, 
  0x1f, 0x3f, 0x71, 0x60, 0x60, 0x60, 0x60, 0x71, 0x3f, 0x1f, 0x00, 
  // 0x37 | 7
  0x06, 0x06, 0x06, 0x06, 0x06, 0xc6, 0xf6, 0x3e, 0x1e, 0x06, 0x00, 
  0x00, 0x00, 0x00, 0x78, 0x7f, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 
  // 0x38 | 8
  0x38, 0x7c, 0xce, 0x86, 0x86, 0x86, 0x86, 0xce, 0x7c, 0x38, 0x00, 
  0x1e, 0x3f, 0x73, 0x61, 0x61, 0x61, 0x61, 0x73, 0x3f, 0x1e, 0x00, 
  // 0x39 | 9
  0xf8, 0xfc, 0x8e, 0x06, 0x06, 0x06, 0x06, 0x8e, 0xfc, 0xf8, 0x00, 
  0x30, 0x71, 0x63, 0x63, 0x63, 0x63, 0x63, 0x79, 0x3f, 0x0f, 0x00, 
  // 0x3a | :
  0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x38, 0x38, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x3b | ;
  0x00, 0x00, 0x00, 0x70, 0x70, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0xb8, 0xb8, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 
  // 0x3c | <
  0x00, 0x80, 0xc0, 0xe0, 0x70, 0x38, 0x1c, 0x0c, 0x04, 0x00, 0x00, 
  0x00, 0x01, 0x03, 0x07, 0x0e, 0x1c, 0x38, 0x30, 0x20, 0x00, 0x00, 
  // 0x3d | =
  0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x00, 
  0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 
  // 0x3e | >
  0x00, 0x04, 0x0c, 0x1c, 0x38, 0x70, 0xe0, 0xc0, 0x80, 0x00, 0x00, 
  0x00, 0x20, 0x30, 0x38, 0x1c, 0x0e, 0x07, 0x03, 0x01, 0x00, 0x00, 
  // 0x3f | ?
  0x18, 0x1c, 0x0e, 0x06, 0x06, 0x86, 0xc6, 0xee, 0x7c, 0x38, 0x00, 
  0x00, 0x00, 0x00, 0x76, 0x77, 0x77, 0x01, 0x00, 0x00, 0x00, 0x00
  
};
#endif // FONT_11X16_NUM_H
