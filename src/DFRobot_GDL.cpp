/*!
 * @file DFRobot_GDL.cpp
 * @brief 定义DFRobot_GDL显示库抽象类，并定义屏子类
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @https://github.com/DFRobot/DFRobot_GDL
 */
#include "DFRobot_GDL.h"
#include "DFRobot_Type.h"

DFRobot_GDL::DFRobot_GDL(sGdlIFDev_t *dev, int16_t w, int16_t h, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl)
  :Adafruit_GFX(w,h),DFRobot_IF(dev, dc, cs, rst, bl),_xStart(0),_yStart(0),_icWidth(w),_icHeight(h),invertOnCmd(0),invertOffCmd(0){
  memset(&madctlReg, 0, sizeof(madctlReg));
}

DFRobot_GDL::DFRobot_GDL(sGdlIFDev_t *dev, int16_t w, int16_t h, uint8_t addr, uint8_t rst, uint8_t bl)
  :Adafruit_GFX(w,h),DFRobot_IF(dev, addr, rst, bl),_xStart(0),_yStart(0),_icWidth(w),_icHeight(h),invertOnCmd(0),invertOffCmd(0){}
DFRobot_GDL::~DFRobot_GDL(){
  if(_lcd.buffer != NULL){
       free(_lcd.buffer);
  }
  _lcd.buffer = NULL;
}
void DFRobot_GDL::gdlInit(uint32_t freq){
  _gdlFont = NULL;
  _fontType = FONT_TYPE_BUILTIN;
  memset(&_lcd, 0, sizeof(_lcd));
  _lcd.buffer = NULL;
  if((_if.interface == IF_HW_SPI)&&(freq == 0)) freq = MCU_SPI_FREQ;
  _if.freq = freq;
  initInterface();
}
void DFRobot_GDL::initDisplay(){
  if(_if.pro.interface == NULL){
      initInterface();
  }
  uint8_t *addr = _if.dev->addr;
  uint8_t flag, cmd, argsNum, val;
  uint16_t time = 0;
  if(addr == NULL){
      DBG("addr error!");
      return;
  }
  while(((flag = pgm_read_byte(addr++)) > 0)) {
       cmd = pgm_read_byte(addr++);
       val = pgm_read_byte(addr++);
       argsNum = val & 0x7F;
       if(val & 0x80) {
           time = pgm_read_byte(addr)*255 + pgm_read_byte(addr+1);
           delay(time);
           addr += 2;
       }
       sendCommand(cmd, addr, argsNum);
       addr += argsNum;
  }
  setRotation(0);
}

void DFRobot_GDL::drawPixel(int16_t x, int16_t y, uint16_t color){
  setDisplayArea((uint16_t)x, (uint16_t)y, 1, 1, color);
}

void DFRobot_GDL::fillScreen(uint16_t color){
  setDisplayArea(0, 0, (uint16_t)_width, (uint16_t)_height, color);
}

void DFRobot_GDL::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color){
  setDisplayArea((uint16_t)x, (uint16_t)y, 1, h, color);
}

void DFRobot_GDL::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color){
  setDisplayArea((uint16_t)x, (uint16_t)y, w, 1, color);
}

void DFRobot_GDL::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color){
  setDisplayArea((uint16_t)x, (uint16_t)y, w, h, color);
}
void DFRobot_GDL::setRotation(uint8_t r){
  if(madctlReg.madctl == 0) return;
  if(madctlReg.madctl == 0xA0){
      rotation = r&1;
      switch(rotation){
          case 0:
                sendCommand(madctlReg.madctl);
                sendCommand(madctlReg.args.value);
                break;
          case 1:
                sendCommand(madctlReg.madctl | 1);
                sendCommand(madctlReg.args.value | 8);
                break;
      }
  }else{
        rotation = r&3;
        uint8_t temp = madctlReg.args.value;
        switch(rotation){
            case 0:
                _width = WIDTH;
                _height = HEIGHT;
                _xStart = 0;
                _yStart = 0;
                break;
            case 1:
                madctlReg.args.mv = 1;
                madctlReg.args.mx = 1;
                _width = HEIGHT;
                _height = WIDTH;
                _xStart = 0;
                _yStart = 0;
                break;
            case 2:
                madctlReg.args.my = 1;
                madctlReg.args.mx = 1;
                _width = WIDTH;
                _height = HEIGHT;
                _xStart = 0;
                _yStart = _icHeight - HEIGHT;
                break;
            default:
                madctlReg.args.mv = 1;
                madctlReg.args.my = 1;
                _width = HEIGHT;
                _height = WIDTH;
                _xStart = _icHeight - HEIGHT;
                _yStart = 0;
                break;
        }
        sendCommand(madctlReg.madctl, &madctlReg.args.value, 1,true);
        madctlReg.args.value = temp;
  }

}
void DFRobot_GDL::invertDisplay(bool i){
  if(invertOnCmd == invertOffCmd) return;
  sendCommand(i ? invertOnCmd : invertOffCmd);
}
void DFRobot_GDL::getColorFormat(uint8_t *pBuf, uint8_t &len, uint8_t &pixel, uint16_t color){
  switch(_lcd.cMode){
      case COLOR_MODE_SINGLE:
           if(color)
               pBuf[0] = 1;
           else
               pBuf[0] = 0;
           len = 1;
           pixel = 8;//一个字节代表8个像素点
           break;
      case COLOR_MODE_RGB111://一个字节代表2个像素点
           pBuf[0] =((color & 0x07) << 3) | ((color & 0x07));
           len = 1;
           pixel = 2;//一个字节代表2个像素点
           break;
      case COLOR_MODE_RGB565:
           pBuf[0] = color >> 8;
           pBuf[1] = color;
           len = 2;
           pixel = 1;//两个字节代表1个像素点
           break;
      case COLOR_MODE_RGB666:
           pBuf[0] = (color >> 8)&0xF8;
           pBuf[1] = (color >> 3)&0xFC;
           pBuf[2] = color << 3;
           len = 3;
           pixel = 1;//3个字节代表1个像素点
           break;
      case COLOR_MODE_RGB888:
           pBuf[0] = (color >> 8)&0xF8;
           pBuf[1] = (color >> 3)&0xFC;
           pBuf[2] = color << 3;
           len = 3;
           pixel = 1;//3个字节代表1个像素点
           break;
  }
}

uint8_t DFRobot_GDL::rgb565ToRGB666(uint8_t *rgb666, uint16_t color){
  uint8_t r = color >> 11;
  uint8_t g = (color << 5) >> 10;
  uint8_t b = color & 0x001F;
  r = (r << 1) | (r & 0x01);
  b = (b << 1) | (b & 0x01);
  *(rgb666 + 0) = r << 2;
  *(rgb666 + 1) = g << 2;
  *(rgb666 + 2) = b << 2;/*三个字节的前18位为*/
  return 3;
}
void DFRobot_GDL::rgb565ToRGB888(uint8_t *r, uint8_t *g, uint8_t *b, uint16_t color){
  uint8_t r5 = (color >> 11);
  uint8_t g6 = (color << 5) >> 10;
  uint8_t b5 = color & 0x001F;
  *r = (r5 << 3) | ((r5 & 0x01) << 2) | (((r5 >> 1)&0x01) << 1) | ((r5 >> 2)&0x01);
  *g = (g6 << 2) | ((g6 & 0x01) << 1) | ((g6 >> 1)&0x01);
  *b = (b5 << 3) | ((b5 & 0x01) << 2) | (((b5 >> 1)&0x01) << 1) | ((b5 >> 2)&0x01);
}

uint16_t DFRobot_GDL::rgb888ToRGB565(uint8_t r, uint8_t g, uint8_t b){
  uint16_t color = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
  return color;
}

void DFRobot_GDL::update(){
  if(_lcd.buffer == NULL) return;
  sendBuf(_lcd.buffer, (uint32_t)(_width*_height/8));
}

void DFRobot_GDL::sendCommand(uint8_t cmd){
  _if.dev->talk(&_if, IF_COM_WRITE_CMD, &cmd, 1);
}

void DFRobot_GDL::sendCommand(uint8_t cmd, void *args, uint32_t len, bool isRamData)
{
  _if.dev->talk(&_if, IF_COM_WRITE_CMD, &cmd, 1);
  if((args == NULL)||(len == 0)) return;
  if(isRamData)
      _if.dev->talk(&_if, IF_COM_WRITE_RAM_INC, (uint8_t *)args, len);
  else
      _if.dev->talk(&_if, IF_COM_WRITE_FLASH_INC, (uint8_t *)args, len);
}

void DFRobot_GDL::sendData(uint8_t data){
  _if.dev->talk(&_if, IF_COM_WRITE_RAM_INC, &data, 1);
}

void DFRobot_GDL::sendData16(uint16_t data){
  uint8_t buf[2];
  buf[0] = data >> 8;
  buf[1] = data;
  _if.dev->talk(&_if, IF_COM_WRITE_RAM_INC, buf, 2);
}

void DFRobot_GDL::sendColor(uint16_t color, uint32_t len)
{
  uint8_t buf[3];
  buf[0] = 2;
  buf[1] = color >> 8;
  buf[2] = color & 0xFF;
  _if.dev->talk(&_if, IF_COM_WRITE_RAM_FIXED, buf, len);
}

void DFRobot_GDL::sendColor(uint8_t *c, uint8_t cBytes, uint32_t len, bool isRamData){
  if((c == NULL)||(cBytes > 4)) return;
  uint8_t buf[cBytes + 1];
  buf[0] = cBytes;
  memcpy(buf+1, c, cBytes);
  if(isRamData)
      _if.dev->talk(&_if, IF_COM_WRITE_RAM_FIXED, buf, len);
  else
      _if.dev->talk(&_if, IF_COM_WRITE_FLASH_FIXED, buf, len);
}
void DFRobot_GDL::setDriverICResolution(int16_t w, int16_t h){
  if(w < _width ||  h < _height) return;
  _icWidth = w;
  _icHeight = h;
}
