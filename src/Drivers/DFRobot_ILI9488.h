#ifndef __DFROBOT_ILI9488_H
#define __DFROBOT_ILI9488_H

#define ILI9488_IC_WIDTH  320
#define ILI9488_IC_HEIGHT  480
#define ILI9488_COLSET  0x2A
#define ILI9488_RAWSET  0x2B
#define ILI9488_RAMWR   0x2C
#define ILI9488_MADCTL  0x36
#define ILI9488_MADCTL_RGB 0x08
#define ILI9488_INVOFF  0x20
#define ILI9488_INVON   0x21

/*static const uint8_t PROGMEM DFRobot_ILI9488_initCmd[] */
static const uint8_t PROGMEM DFRobot_ILI9488_initCmd[]= {
//flag cmd If the highest digit is 1, it means that the last 2 digits are delay, the lower 7 digits indicate the number of parameters
0x01, 0XF7, 4, 0xA9, 0x51, 0x2C, 0x82,
0x01, 0xC0, 2, 0x11, 0x09,
0x01, 0xC1, 1, 0x41,
0x01, 0XC5, 3, 0x00, 0x0A, 0x80,
0x01, 0xB1, 2, 0xB0, 0x11,
0x01, 0xB4, 1, 0x02,
0x01, 0xB6, 2, 0x02, 0x22,
0x01, 0xB7, 1, 0xc6,
0x01, 0xBE, 2, 0x00, 0x04,
0x01, 0xE9, 1, 0x00, 
0x01, 0x36, 1, 0x08,
0x01, 0x3A, 1, 0x66,
0x01, 0xE0, 15, 0x00, 0x07, 0x10, 0x09, 0x17, 0x0B, 0x41, 0x89, 0x4B, 0x0A, 0x0C, 0x0E, 0x18, 0x1B, 0x0F,
0x01, 0XE1, 15, 0x00, 0x17, 0x1A, 0x04, 0x0E, 0x06, 0x2F, 0x45, 0x43, 0x02, 0x0A, 0x09, 0x32, 0x36, 0x0F,
0x01, 0x11, 0x80, 0, 120,
0x01, 0x20, 0,
//0x01, 0x21, 0,
0x01, 0x29, 0,
0x00
};

#endif
