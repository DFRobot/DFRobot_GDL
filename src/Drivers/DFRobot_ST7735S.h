#ifndef __DFROBOT_ST7735S_H
#define __DFROBOT_ST7735S_H

#define ST7735S_R80x160_IC_WIDTH  80
#define ST7735S_R80x160_IC_HEIGHT  160
#define ST7735S_R128x160_IC_WIDTH  128
#define ST7735S_R128x160_IC_HEIGHT  160
#define ST7735S_COLSET  0x2A
#define ST7735S_RAWSET  0x2B
#define ST7735S_RAMWR   0x2C
#define ST7735S_MADCTL  0x36
#define ST7735S_R80x160_MADCTL_RGB 0x08
#define ST7735S_R128x160_MADCTL_RGB 0x05
#define ST7735S_INVOFF  0x20
#define ST7735S_INVON   0x21

static const uint8_t PROGMEM DFRobot_ST7735S_R80x160_initCmd[] = {
  //cmd delayFlag/argsNum argsValue Delay (over 255) Delay / 255
//flag
0x01, 0x11, 0x80, 0x00, 120,/*0x11：Exit sleep mode command, 0x80: the highest bit represents whether to delay,
1 represents delay, 0 represents no delay, the last 1 represents the number of parameters following the command, 0 represents no parameters
0x00: indicates a multiple of 255 when delaying, such as a delay of 120ms, can be written as 0x00, 120
Actual delay time = 0x00 * 255 + 120ms */
//0x01, 0x21, 0,//0x21Turn on reverse display, no parameters, no delay
0x01, 0xB1, 3, 0x01, 0x2C, 0x2D,//Frame rate control (normal mode) (GM [1: 0] 00)
0x01, 0xB2, 3, 0x01, 0x2C, 0x2D,//Frame rate control (idle mode)
0x01, 0xB3, 6, 0x01, 0x2C, 0x2D, 0x01, 0x2C, 0x2D,//Frame rate control (local mode)
0x01, 0xB4, 1, 0x07,//Reverse control
0x01, 0xB6, 3, 0xA2, 0x02, 0x84,//Frame rate control (idle mode)

0x01, 0xC0, 3, 0xA2, 0x02, 0x84,//Power consumption control
0x01, 0xC1, 1, 0xC5,//Power consumption control
0x01, 0xC2, 2, 0x0A, 0x00,//Power consumption control
0x01, 0xC3, 2, 0x8A, 0x2A,//Power consumption control
0x01, 0xC4, 2, 0x8A, 0xEE,//Power consumption control
0x01, 0xC5, 1, 0x0E,//VCOM
0x01, 0x20, 0, 
0x01, 0x3A, 1, 0x05,//Color mode settings
0x01, 0x36, 1, 0xc0,//(0x36)MADCTL memory data access control commands
0x01, 0x2A, 4, 0x00,0x00,0x00,0x4F,//(0x36)MADCTL memory data access control commands
0x01, 0x2B, 4, 0x00,0x00,0x00,0x9F,//(0x36)MADCTL memory data access control commands

0x01, 0xE0, 16,0x02, 0x1C, 0X07, 0X12, 0X37, 0X32, 0x29, 0x2D, 0X29, 0X25, 0X2B, 0X39, 0X00, 0X01, 0X03, 0x10,
0x01, 0xE1, 16,0x03, 0x1D, 0X07, 0X06, 0X2E, 0X2C, 0x29, 0x2D, 0X2E, 0X2E, 0X37, 0X3F, 0X00, 0X00, 0X02, 0x10,
0X01, 0X13,0,
0x01, 0x29, 0,
0x00
//The first column 0x01 and 0x00 are used to distinguish whether the array ends
//format:flag-cmd-argNs-args-(time)
};

static const uint8_t PROGMEM DFRobot_ST7735S_R128x160_initCmd[] = {
  //cmd delayFlag/argsNum argsValue Delay (over 255) Delay / 255
//flag
0x01, 0x11, 0x80, 0x00, 120,/*0x11：Exit sleep mode command, 0x80: the highest bit represents whether to delay,
1 represents delay, 0 represents no delay, the last 1 represents the number of parameters following the command, 0 represents no parameters
0x00: indicates a multiple of 255 when delaying, such as a delay of 120ms, can be written as 0x00, 120
Actual delay time = 0x00 * 255 + 120ms */
//0x01, 0x21, 0,//0x21Turn on reverse display, no parameters, no delay
0x01, 0xB1, 3, 0x05, 0x3C, 0x3C,//Frame rate control (normal mode) (GM [1: 0] 00)
0x01, 0xB2, 3, 0x05, 0x3C, 0x3C,//Frame rate control (idle mode)
0x01, 0xB3, 6, 0x05, 0x3C, 0x3C, 0x05, 0x3C, 0x3C,//Frame rate control (local mode)
0x01, 0xB4, 1, 0x03,//Reverse control
//0x01, 0xB6, 3, 0xA2, 0x02, 0x84,//Frame rate control (idle mode)

0x01, 0xC0, 3, 0x28, 0x08, 0x04,//Power consumption control
0x01, 0xC1, 1, 0xC0,//Power consumption control
0x01, 0xC2, 2, 0x0D, 0x00,//Power consumption control
0x01, 0xC3, 2, 0x8D, 0x2A,//Power consumption control
0x01, 0xC4, 2, 0x8D, 0xEE,//Power consumption control
0x01, 0xC5, 1, 0x1A,//VCOM
//0x01, 0x20, 0, 
0x01, 0x3A, 1, 0x05,//Color mode settings
0x01, 0x36, 1, 0xc0,//(0x36)MADCTL memory data access control commands
//0x01, 0x2A, 4, 0x00,0x00,0x00,0x4F,//(0x36)MADCTL memory data access control commands
//0x01, 0x2B, 4, 0x00,0x00,0x00,0x9F,//(0x36)MADCTL memory data access control commands

0x01, 0xE0, 16,0x04, 0x22, 0X07, 0X0A, 0X2E, 0X30, 0x25, 0x2A, 0X28, 0X26, 0X2E, 0X3A, 0X00, 0X01, 0X03, 0x13,
0x01, 0xE1, 16,0x04, 0x16, 0X06, 0X0D, 0X2D, 0X26, 0x23, 0x27, 0X27, 0X25, 0X2D, 0X3B, 0X00, 0X01, 0X04, 0x13,
//0X01, 0X13,0,
0x01, 0x29, 0,
0x00
//The first column 0x01 and 0x00 are used to distinguish whether the array ends
//format:flag-cmd-argNs-args-(time)
};
#endif
