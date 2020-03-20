/*!
 * @file DFRobot_LCD.cpp
 * @brief 定义DFRobot_GDL显示库的子类，屏的实现
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @https://github.com/DFRobot/DFRobot_GDL
 */

#include "Arduino.h"
#include "DFRobot_Type.h"
#include "DFRobot_GDL.h"
#include "Interface/DFRobot_IF.h"
#include "Drivers/DFRobot_LCDType.h"

GDL_IF_PB_DEV(gdl_Dev_ST7789_R240x240_HW_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ST7789_initCmd, IF_COM_HW_SPI);
GDL_IF_PB_DEV(gdl_Dev_ST7789_R240x320_HW_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ST7789_initCmd, IF_COM_HW_SPI);
GDL_IF_PB_DEV(gdl_Dev_ST7735S_R80x160_HW_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ST7735S_initCmd, IF_COM_HW_SPI);
GDL_IF_PB_DEV(gdl_Dev_ILI9488_R320x480_HW_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ILI9488_initCmd, IF_COM_HW_SPI);
GDL_IF_PB_DEV(gdl_Dev_SSD1306_R128x32_HW_IIC, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_SSD1306_initCmd, IF_COM_HW_IIC);
GDL_IF_PB_DEV(gdl_Dev_ILI9341_R240x320_HW_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ILI9341_initCmd, IF_COM_HW_SPI);


DFRobot_ILI9341_240x320_HW_SPI::DFRobot_ILI9341_240x320_HW_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ILI9341_R240x320_HW_SPI, 240, 320, dc, cs, rst, bl){
  setDriverICResolution(ILI9341_IC_WIDTH, ILI9341_IC_HEIGHT);
  madctlReg.madctl = ILI9341_MADCTL;
  madctlReg.args.value = ILI9341_MADCTL_RGB;
  invertOffCmd = ILI9341_INVOFF;
  invertOnCmd = ILI9341_INVON;
}
DFRobot_ILI9341_240x320_HW_SPI::~DFRobot_ILI9341_240x320_HW_SPI(){}
void DFRobot_ILI9341_240x320_HW_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ILI9341_240x320_HW_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(ILI9341_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ILI9341_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ILI9341_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

DFRobot_ST7789_240x240_HW_SPI::DFRobot_ST7789_240x240_HW_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ST7789_R240x240_HW_SPI, 240, 240, dc, cs, rst, bl){
  setDriverICResolution(ST7789_IC_WIDTH, ST7789_IC_HEIGHT);
  madctlReg.madctl = ST7789_MADCTL;
  madctlReg.args.value = ST7789_MADCTL_RGB;
  invertOffCmd = ST7789_INVOFF;
  invertOnCmd = ST7789_INVON;
}
DFRobot_ST7789_240x240_HW_SPI::~DFRobot_ST7789_240x240_HW_SPI(){}
void DFRobot_ST7789_240x240_HW_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ST7789_240x240_HW_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(ST7789_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ST7789_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ST7789_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

DFRobot_ST7789_240x320_HW_SPI::DFRobot_ST7789_240x320_HW_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ST7789_R240x320_HW_SPI, 240, 320, dc, cs, rst, bl){
  setDriverICResolution(ST7789_IC_WIDTH, ST7789_IC_HEIGHT);
  madctlReg.madctl = ST7789_MADCTL;
  madctlReg.args.value = ST7789_MADCTL_RGB;
  invertOffCmd = ST7789_INVOFF;
  invertOnCmd = ST7789_INVON;
}
DFRobot_ST7789_240x320_HW_SPI::~DFRobot_ST7789_240x320_HW_SPI(){}
void DFRobot_ST7789_240x320_HW_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ST7789_240x320_HW_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(ST7789_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ST7789_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ST7789_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

DFRobot_ST7735S_80x160_HW_SPI::DFRobot_ST7735S_80x160_HW_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ST7735S_R80x160_HW_SPI, 80, 160, dc, cs, rst, bl){
  setDriverICResolution(ST7735S_IC_WIDTH, ST7735S_IC_HEIGHT);
  madctlReg.madctl = ST7735S_MADCTL;
  madctlReg.args.value = ST7735S_MADCTL_RGB;
  invertOffCmd = ST7735S_INVOFF;
  invertOnCmd = ST7735S_INVON;
}
DFRobot_ST7735S_80x160_HW_SPI::~DFRobot_ST7735S_80x160_HW_SPI(){}
void DFRobot_ST7735S_80x160_HW_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ST7735S_80x160_HW_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) ;
  switch(rotation){
      case 0:
      case 2:
            _xStart = (ST7735S_IC_WIDTH - _width)/2;
            _yStart = (ST7735S_IC_HEIGHT - _height)/2;
            break;
      case 1:
      case 3:
            _xStart = (ST7735S_IC_HEIGHT - _width)/2;
            _yStart = (ST7735S_IC_WIDTH - _height)/2;
            break;
  }
  sendCommand(ST7735S_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ST7735S_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ST7735S_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

DFRobot_ILI9488_320x480_HW_SPI::DFRobot_ILI9488_320x480_HW_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ILI9488_R320x480_HW_SPI, 320, 480, dc, cs, rst, bl){
  setDriverICResolution(ILI9488_IC_WIDTH, ILI9488_IC_HEIGHT);
  madctlReg.madctl = ILI9488_MADCTL;
  madctlReg.args.value = ILI9488_MADCTL_RGB;
  invertOffCmd = ILI9488_INVOFF;
  invertOnCmd = ILI9488_INVON;
}
DFRobot_ILI9488_320x480_HW_SPI::~DFRobot_ILI9488_320x480_HW_SPI(){}
void DFRobot_ILI9488_320x480_HW_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ILI9488_320x480_HW_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(ILI9488_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ILI9488_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ILI9488_RAMWR);
  uint8_t rgb666[3];
  rgb565ToRGB666(rgb666, color);
  sendColor(rgb666, 3, (uint32_t)w*h);
}

DFRobot_SSD1306_128x32_HW_IIC::DFRobot_SSD1306_128x32_HW_IIC(uint8_t addr, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_SSD1306_R128x32_HW_IIC, 128, 32, addr, rst, bl){
  invertOffCmd = SSD1306_INVOFF;
  invertOnCmd = SSD1306_INVON;
  madctlReg.madctl = SSD1306_REMAP;
  madctlReg.args.value = SSD1306_SCANDIR;
}
DFRobot_SSD1306_128x32_HW_IIC::~DFRobot_SSD1306_128x32_HW_IIC(){
  
}
void DFRobot_SSD1306_128x32_HW_IIC::begin(uint32_t freq){
  gdlInit(freq);
  _lcd.buffer = (uint8_t *)malloc(128*64/8);
  memset(_lcd.buffer,0xff,128*64/8);
  initDisplay();//显示屏初始化
}
void DFRobot_SSD1306_128x32_HW_IIC::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color){
  //if((x + w ) > _width || (y + h) > _height) return;
  if(x == 0 && y == 0 && w == (uint16_t)_width && h == (uint16_t)_height){
      color ? memset(_lcd.buffer, 0xFF, _width*_height/8) : memset(_lcd.buffer, 0x00, _width*_height/8);
  }else{
      uint16_t index = 0, bi = 0;
      for(uint16_t i = x; i < x+w; i++){
          for(uint16_t j = y; j < y+h; j++){
              index = ((j/8)*128) + i;
              bi = j % 8;
              _lcd.buffer[index] = (color ? _lcd.buffer[index] | (1 << bi) : _lcd.buffer[index] & ~(1 << bi));
          }
      }
  }
  sendCommand(SSD1306_COLSET);
  sendCommand(SSD1306_STARTCOL);
  sendCommand(SSD1306_ENDCOL);
  sendCommand(SSD1306_PAGESET);
  sendCommand(SSD1306_STARTPAGE);
  sendCommand(SSD1306_ENDPAGE);
}


#ifdef ARDUINO_SAM_ZERO
GDL_IF_PB_DEV(gdl_Dev_ST7789_R240x240_DMA_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ST7789_initCmd, IF_COM_DMA_SPI);
GDL_IF_PB_DEV(gdl_Dev_ST7789_R240x320_DMA_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ST7789_initCmd, IF_COM_DMA_SPI);
GDL_IF_PB_DEV(gdl_Dev_ST7735S_R80x160_DMA_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ST7735S_initCmd, IF_COM_DMA_SPI);
GDL_IF_PB_DEV(gdl_Dev_ILI9488_R320x480_DMA_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ILI9488_initCmd, IF_COM_DMA_SPI);
GDL_IF_PB_DEV(gdl_Dev_ILI9341_R240x320_DMA_SPI, DEV_TYPE_SCREEN, (uint8_t *)DFRobot_ILI9341_initCmd, IF_COM_DMA_SPI);

DFRobot_ST7789_240x240_DMA_SPI::DFRobot_ST7789_240x240_DMA_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ST7789_R240x240_DMA_SPI, 240, 240, dc, cs, rst, bl){
  setDriverICResolution(ST7789_IC_WIDTH, ST7789_IC_HEIGHT);
  madctlReg.madctl = ST7789_MADCTL;
  madctlReg.args.value = ST7789_MADCTL_RGB;
  invertOffCmd = ST7789_INVOFF;
  invertOnCmd = ST7789_INVON;
}
DFRobot_ST7789_240x240_DMA_SPI::~DFRobot_ST7789_240x240_DMA_SPI(){}
void DFRobot_ST7789_240x240_DMA_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ST7789_240x240_DMA_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(ST7789_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ST7789_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ST7789_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

DFRobot_ST7735S_80x160_DMA_SPI::DFRobot_ST7735S_80x160_DMA_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ST7735S_R80x160_DMA_SPI, 80, 160, dc, cs, rst, bl){
  setDriverICResolution(ST7735S_IC_WIDTH, ST7735S_IC_HEIGHT);
  madctlReg.madctl = ST7735S_MADCTL;
  madctlReg.args.value = ST7735S_MADCTL_RGB;
  invertOffCmd = ST7735S_INVOFF;
  invertOnCmd = ST7735S_INVON;
}
DFRobot_ST7735S_80x160_DMA_SPI::~DFRobot_ST7735S_80x160_DMA_SPI(){}
void DFRobot_ST7735S_80x160_DMA_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ST7735S_80x160_DMA_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  switch(rotation){
      case 0:
      case 2:
            _xStart = (ST7735S_IC_WIDTH - _width)/2;
            _yStart = (ST7735S_IC_HEIGHT - _height)/2;
            break;
      case 1:
      case 3:
            _xStart = (ST7735S_IC_HEIGHT - _width)/2;
            _yStart = (ST7735S_IC_WIDTH - _height)/2;
            break;
  }
  sendCommand(ST7735S_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ST7735S_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ST7735S_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

DFRobot_ST7789_240x320_DMA_SPI::DFRobot_ST7789_240x320_DMA_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ST7789_R240x320_DMA_SPI, 240, 320, dc, cs, rst, bl){
  setDriverICResolution(ST7789_IC_WIDTH, ST7789_IC_HEIGHT);
  madctlReg.madctl = ST7789_MADCTL;
  madctlReg.args.value = ST7789_MADCTL_RGB;
  invertOffCmd = ST7789_INVOFF;
  invertOnCmd = ST7789_INVON;
}
DFRobot_ST7789_240x320_DMA_SPI::~DFRobot_ST7789_240x320_DMA_SPI(){}
void DFRobot_ST7789_240x320_DMA_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ST7789_240x320_DMA_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(ST7789_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ST7789_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ST7789_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

DFRobot_ILI9488_320x480_DMA_SPI::DFRobot_ILI9488_320x480_DMA_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ILI9488_R320x480_DMA_SPI, 320, 480, dc, cs, rst, bl){
  setDriverICResolution(ILI9488_IC_WIDTH, ILI9488_IC_HEIGHT);
  madctlReg.madctl = ILI9488_MADCTL;
  madctlReg.args.value = ILI9488_MADCTL_RGB;
  invertOffCmd = ILI9488_INVOFF;
  invertOnCmd = ILI9488_INVON;
}
DFRobot_ILI9488_320x480_DMA_SPI::~DFRobot_ILI9488_320x480_DMA_SPI(){}
void DFRobot_ILI9488_320x480_DMA_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ILI9488_320x480_DMA_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(0x2A);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(0x2B);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(0x2C);
  uint8_t rgb666[3];
  rgb565ToRGB666(rgb666, color);
  sendColor(rgb666, sizeof(rgb666), (uint32_t)w*h);
}

DFRobot_ILI9341_240x320_DMA_SPI::DFRobot_ILI9341_240x320_DMA_SPI(uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :DFRobot_GDL(&gdl_Dev_ILI9341_R240x320_HW_SPI, 240, 320, dc, cs, rst, bl){
  setDriverICResolution(ILI9341_IC_WIDTH, ILI9341_IC_HEIGHT);
  madctlReg.madctl = ILI9341_MADCTL;
  madctlReg.args.value = ILI9341_MADCTL_RGB;
  invertOffCmd = ILI9341_INVOFF;
  invertOnCmd = ILI9341_INVON;
}
DFRobot_ILI9341_240x320_DMA_SPI::~DFRobot_ILI9341_240x320_DMA_SPI(){}
void DFRobot_ILI9341_240x320_DMA_SPI::begin(uint32_t freq)
{
  gdlInit(freq);
  initDisplay();
}
void DFRobot_ILI9341_240x320_DMA_SPI::setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
  //if((x + w ) > _width || (y + h) > _height) return;
  sendCommand(ILI9341_COLSET);
  sendData16(_xStart + x);
  sendData16(_xStart + x + w -1);
  sendCommand(ILI9341_RAWSET);
  sendData16(_yStart + y);
  sendData16(_yStart + y + h -1);
  sendCommand(ILI9341_RAMWR);
  sendColor(color, (uint32_t)w*h);
}

#endif//end M0
