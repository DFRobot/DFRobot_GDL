/*!
 * @file DFRobot_IF.cpp
 * @brief Declaration the basic structure of class DFRobot_IF，兼容不同接口的读写
 * @n 支持硬件IIC
 * @n 支持硬件SPI
 * @n 支持M0的DMA
 * @n 支持从rom/ram读取数据
 * @n 支持SPI/IIC/SPI_DMA读写
 * @n 设置通信接口频率
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @https://github.com/DFRobot/DFRobot_GDL
 */

#include "DFRobot_IF.h"
#include "DFRobot_Type.h"
#include "Arduino.h"

DFRobot_IF::DFRobot_IF(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t bl/*irq*/){
  memset(&_if, 0, sizeof(_if));
  initHWIIC(dev, addr, rst, bl);
}
DFRobot_IF::DFRobot_IF(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/){
  memset(&_if, 0, sizeof(_if));
  initHWSPI(dev, dc, cs, rst, bl);
}
DFRobot_IF::~DFRobot_IF(){
  if(_if.pinList != NULL){
      free(_if.pinList);
  }
  _if.pinList = NULL;
}

void DFRobot_IF::initInterface(){
  PIN_LOW(_if.pinList[IF_PIN_RST]);
  delay(100);
  PIN_HIGH(_if.pinList[IF_PIN_RST]);
  delay(100);
  _if.dev->talk(&_if, IF_COM_PROTOCOL_INIT, NULL, 0);
}

void DFRobot_IF::setFrequency(uint32_t freq){
  if(!freq) return;
  _if.freq = freq;
  if(_if.isBegin == true)
      _if.dev->talk(&_if, IF_COM_SET_FREQUENCY, NULL, 0);
}

void DFRobot_IF::initIFCommon(){
  _if.freq = 0;
  for(uint8_t i = 0; i < _if.interface; i++){
      _if.pinList[i] = GDL_PIN_NC;
  }
  _if.isBegin = false;
  _if.length = 0xFFFF;
}
void DFRobot_IF::initHWIIC(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t bl/*irq*/){
  _if.interface = IF_HW_IIC;
  if((_if.pinList =(uint8_t *)malloc(_if.interface)) == NULL){
      return;
  }
  initIFCommon();
  _if.pinList[IF_PIN_ADDR] = addr;
  _if.pinList[IF_PIN_RST] = rst;
  _if.pinList[IF_PIN_BL] = bl;
  for(uint8_t i = 0; i < _if.interface; i++){
      if(_if.pinList[i] == GDL_PIN_NC)
          continue;
      pinMode(_if.pinList[i], OUTPUT);
      digitalWrite(_if.pinList[i], HIGH);
  }
  _if.length = I2C_BUFFER_LENGTH;
  _if.dev = dev;
}
void DFRobot_IF::initHWSPI(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/){
  _if.interface = IF_HW_SPI;
  if((_if.pinList =(uint8_t *)malloc(_if.interface)) == NULL){
      return;
  }
  initIFCommon();
  _if.pinList[IF_PIN_DC] = dc;
  _if.pinList[IF_PIN_CS] = cs;
  _if.pinList[IF_PIN_RST] = rst;
  _if.pinList[IF_PIN_BL] = bl;
  for(uint8_t i = 0; i < _if.interface; i++){
      if(_if.pinList[i] == GDL_PIN_NC)
          continue;
      pinMode(_if.pinList[i], OUTPUT);
      digitalWrite(_if.pinList[i], HIGH);
  }
  _if.dev = dev;
}

void DFRobot_IF::writeData32(uint32_t data, bool isRamData){
  uint8_t buf[4];
  buf[0] = data >> 24;
  buf[1] = data >> 16;
  buf[2] = data >> 8;
  buf[3] = data;
  isRamData ?_if.dev->talk(&_if, IF_COM_WRITE_RAM_INC, buf, 4) : _if.dev->talk(&_if, IF_COM_WRITE_FLASH_INC, buf, 4);
}

void DFRobot_IF::sendBuf(void *buf, uint32_t len, bool isRamData){
  isRamData ?_if.dev->talk(&_if, IF_COM_WRITE_RAM_INC, (uint8_t *)buf, len) : _if.dev->talk(&_if, IF_COM_WRITE_FLASH_INC, (uint8_t *)buf, len);
}

void DFRobot_IF::writeBuf(uint16_t reg, void *pBuf, uint32_t len, bool flag)
{
  if(pBuf == NULL){
      return;
  }
  uint8_t *_pBuf = (uint8_t *)pBuf;
  uint8_t bufPre[3];
  uint32_t left = len;
  len = 0;
  while(left){
      len = (flag ? len/2 : len);
      reg += len;
      bufPre[0] = 2;
      bufPre[1] = reg >> 8;
      bufPre[2] = reg & 0xFF;
      len = (left > _if.length ? _if.length : left);
      uint8_t buf[3+len];
      memset(buf, 0, sizeof(buf));
      memcpy(buf, bufPre, 3);
      memcpy(buf+3, _pBuf, len);
      _if.dev->talk(&_if, IF_COM_WRITE_RAM_INC, buf, len);
      left -= len;
      _pBuf += len;
  }
}
void DFRobot_IF::readReg(uint16_t reg, void *pBuf, uint32_t len, bool flag)
{
  uint8_t buf[2];
  buf[0] = reg >> 8;
  buf[1] = reg;
  if(flag)
      readBuf(buf, 2, pBuf, len, false, 2);//16位寄存器，16位数据
  else
      readBuf(buf, 2, pBuf, len, false, 1);//16位寄存器，8位数据
}
void DFRobot_IF::readCommand(uint8_t cmd, void *pBuf, uint32_t len){
  readBuf(&cmd, 1, pBuf, len, true);
}
bool DFRobot_IF::readBuf(void *reg, uint8_t regBytes, void *pBuf, uint32_t len, bool regIscmd, uint8_t valBytes){
  if(pBuf == NULL) return false;
  uint8_t *_pBuf = (uint8_t *)pBuf;
  uint8_t bufPre[regBytes+1];
  bufPre[0] = regBytes;
  memcpy(bufPre+1, reg,regBytes);
  uint32_t left = len;
  len = 0;
  if(regIscmd){//代表reg不是寄存器，是命令
      while(left){
          left > _if.length ? len = _if.length : len = left;
          uint8_t buf[sizeof(bufPre)+len];
          memset(buf, 0, sizeof(buf));
          memcpy(buf, bufPre, sizeof(bufPre));
          uint8_t flag = _if.dev->talk(&_if, IF_COM_READ_DATA, buf, len);
          if(!flag) return false;
          memcpy(_pBuf, buf, len);
          left -= len;
          _pBuf += len;
      }
  }else{
      uint32_t regAddr = 0;
      for(uint8_t i = 0; i < regBytes; i++){
          regAddr |= (bufPre[1+i]<<((regBytes - 1 -i)*8));
      }
      uint32_t maxBytes = (_if.length / valBytes)*valBytes;
      while(left){
          len /= valBytes;
          regAddr += len;
          left > maxBytes ? len = maxBytes : len = left;
          uint8_t buf[sizeof(bufPre)+len];
          memset(buf, 0, sizeof(buf));
          buf[0] = regBytes;
          for(uint8_t i = 0; i < regBytes; i++){
              buf[1+i] = (uint8_t)(regAddr >> (regBytes - 1 - i)*8);
          }
          delay(1);
          uint8_t flag = _if.dev->talk(&_if, IF_COM_READ_DATA, buf, len);
          if(!flag) return false;
          memcpy(_pBuf, buf, len);
          left -= len;
          _pBuf += len;
      }
  }
  return true;
}
