#ifndef __DFROBOT_ST7789_H
#define __DFROBOT_ST7789_H

#define ST7789_IC_WIDTH  240
#define ST7789_IC_HEIGHT  320
#define ST7789_COLSET  0x2A
#define ST7789_RAWSET  0x2B
#define ST7789_RAMWR   0x2C
#define ST7789_MADCTL  0x36
#define ST7789_MADCTL_RGB 0x00
#define ST7789_INVOFF  0x20
#define ST7789_INVON   0x21



static const uint8_t PROGMEM DFRobot_ST7789_initCmd[]= {
0x01, 0x01, 0x80, 0, 150,
0x01, 0x11, 0x80, 0, 120,
0x01, 0x3A, 1, 0x55,
0x01, 0x36, 1, 0x00,
0x01, 0x21, 0,
0x01, 0x13, 0,
0x01, 0x29, 0,
0x00
};
#endif
