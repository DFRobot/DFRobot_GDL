#ifndef __DFROBOT_ST7735S_H
#define __DFROBOT_ST7735S_H

#define ST7735S_IC_WIDTH  132
#define ST7735S_IC_HEIGHT  162
#define ST7735S_COLSET  0x2A
#define ST7735S_RAWSET  0x2B
#define ST7735S_RAMWR   0x2C
#define ST7735S_MADCTL  0x36
#define ST7735S_MADCTL_RGB 0x08
#define ST7735S_INVOFF  0x20
#define ST7735S_INVON   0x21

static const uint8_t PROGMEM DFRobot_ST7735S_initCmd[] = {
  //cmd delayFlag/argsNum argsValue Delay (over 255) Delay / 255
//flag
0x01, 0x11, 0x80, 0x00, 120,/*0x11：Exit sleep mode command, 0x80: the highest bit represents whether to delay,
1 represents delay, 0 represents no delay, the last 1 represents the number of parameters following the command, 0 represents no parameters
0x00: indicates a multiple of 255 when delaying, such as a delay of 120ms, can be written as 0x00, 120
Actual delay time = 0x00 * 255 + 120ms */
0x01, 0x21, 0,//0x21Turn on reverse display, no parameters, no delay
0x01, 0xB1, 3, 0x05, 0x3A, 0x3A,//Frame rate control (normal mode) (GM [1: 0] 00)
0x01, 0xB2, 3, 0x05, 0x3A, 0x3A,//Frame rate control (idle mode)
0x01, 0xB3, 6, 0x05, 0x3A, 0x3A, 0x05, 0x3A, 0x3A,//Frame rate control (local mode)
0x01, 0xB4, 1, 0x03,//Reverse control
0x01, 0xC0, 3, 0x62, 0x02, 0x04,//Power consumption control
0x01, 0xC1, 1, 0xC0,//Power consumption control
0x01, 0xC2, 2, 0x0D, 0x00,//Power consumption control
0x01, 0xC3, 2, 0x8D, 0x6A,//Power consumption control
0x01, 0xC4, 2, 0x8D, 0xEE,//Power consumption control
0x01, 0xC5, 1, 0x0E,//VCOM
0x01, 0xE0, 16,0x10, 0x0E, 0X02, 0X03, 0X0E, 0X07, 0x02, 0x07, 0X0A, 0X12, 0X27, 0X37, 0X00, 0X0D, 0X0E, 0x10,
0x01, 0xE1, 16,0x10, 0x0E, 0X03, 0X03, 0X0F, 0X06, 0x02, 0x08, 0X0A, 0X13, 0X26, 0X36, 0X00, 0X0D, 0X0E, 0x10,
0x01, 0x3A, 1, 0x55,//Color mode settings
0x01, 0x36, 1, 0x08,//(0x36)MADCTL memory data access control commands
0x01, 0x29, 0,
0x00
//The first column 0x01 and 0x00 are used to distinguish whether the array ends
//format:flag-cmd-argNs-args-(time)
};

#endif