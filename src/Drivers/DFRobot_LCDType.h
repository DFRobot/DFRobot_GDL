#ifndef __DFROBOT_LCDTYPE_H
#define __DFROBOT_LCDTYPE_H
#include "DFRobot_Type.h"
#include "DFRobot_ST7735S.h"
#include "DFRobot_SSD1306.h"
#include "DFRobot_ILI9488.h"
#include "DFRobot_ST7789.h"
#include "DFRobot_ILI9341.h"
extern sGdlIFDev_t gdl_Dev_ST7789_R240x240_HW_SPI;
extern sGdlIFDev_t gdl_Dev_ST7789_R240x320_HW_SPI;
extern sGdlIFDev_t gdl_Dev_SSD1306_R128x32_HW_IIC;
extern sGdlIFDev_t gdl_Dev_ST7735S_R80x160_HW_SPI;
extern sGdlIFDev_t gdl_Dev_ILI9488_R320x480_HW_SPI;
extern sGdlIFDev_t gdl_Dev_ILI9341_R240x320_HW_SPI;
#ifdef ARDUINO_SAM_ZERO
extern sGdlIFDev_t gdl_Dev_ST7789_R240x240_DMA_SPI;
extern sGdlIFDev_t gdl_Dev_ST7789_R240x320_DMA_SPI;
extern sGdlIFDev_t gdl_Dev_ST7735S_R80x160_DMA_SPI;
extern sGdlIFDev_t gdl_Dev_ILI9488_R320x480_DMA_SPI;
extern sGdlIFDev_t gdl_Dev_ILI9341_R240x320_DMA_SPI;
#endif

#endif