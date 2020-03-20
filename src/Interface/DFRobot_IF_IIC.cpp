#include "DFRobot_IF.h"
#include <DFRobot_Type.h>

#define DEFAULT_IIC_FREQ  100000L//除了ESP32，AVR ESP8266 M0都是1khz

uint8_t interfaceComHardwareIIC(sGdlIF_t *p, uint8_t cmd, uint8_t *pBuf, uint32_t len)
{
  //Serial.print("cmd = ");Serial.println(cmd);
  if((p == NULL))
      return 0;
  switch(cmd){
      case IF_COM_PROTOCOL_INIT:
      {
          //Serial.println("IF_COM_PROTOCOL_INIT");
          p->pro.iic = &Wire;
          p->pro.iic->begin();
          /*当频率为0时，默认使用板载频率，并将频率赋值给p->freq*/
          if(p->freq != 0) {
              p->pro.iic->setClock(p->freq);
          }else{
              p->freq = DEFAULT_IIC_FREQ;
          }
          p->isBegin = true;
      }
           break;
      case IF_COM_SET_FREQUENCY:
      {
           if(p->freq == 0) return 0;
           p->pro.iic->setClock(p->freq);
      }
           break;
      case IF_COM_WRITE_CMD:
      {
          if(!p->isBegin) return 0;
          while(len){
              p->pro.iic->beginTransmission(p->pinList[IF_PIN_ADDR]);
              uint32_t n = 0;
              if(p->dev->devName == DEV_TYPE_SCREEN)
              {
                  (len > I2C_BUFFER_LENGTH/2) ? n = I2C_BUFFER_LENGTH/2 : n = len;
                  p->pro.iic->write(0x00);
              }else{
                  (len > I2C_BUFFER_LENGTH) ? n = len : n = I2C_BUFFER_LENGTH;
              }
              p->pro.iic->write(pBuf, n);
              p->pro.iic->endTransmission();
              pBuf += n;
              len -= n;
          }
           break;
      }
      case IF_COM_READ_DATA:
      {
          if(!p->isBegin) return 0;
          if((pBuf[0] > 4) || (len > I2C_BUFFER_LENGTH)) return 0;
          p->pro.iic->beginTransmission(p->pinList[IF_PIN_ADDR]);
          p->pro.iic->write((const uint8_t *)(pBuf+1),(size_t)pBuf[0]);
          if( p->pro.iic->endTransmission() != 0){
                return 0;
          }
          p->pro.iic->requestFrom(p->pinList[IF_PIN_ADDR], len);
          for(uint16_t i = 0; i < len; i++){
              pBuf[i] = p->pro.iic->read();
          }
      }
          break;
      case IF_COM_WRITE_FLASH_FIXED:
      {
          Serial.println("IF_COM_WRITE_FLASH_FIXED");
          if(!(p->isBegin) || pBuf[0] > 4) return 0;
          while(len){
              p->pro.iic->beginTransmission(p->pinList[IF_PIN_ADDR]);
              uint32_t n = 0;
              if(p->dev->devName == DEV_TYPE_SCREEN)
              {
                  (len > (I2C_BUFFER_LENGTH - 1)/pgm_read_byte(&pBuf[0])) ? n = (I2C_BUFFER_LENGTH - 1)/pgm_read_byte(&pBuf[0]) : n = len;
                  p->pro.iic->write(0x40);
              }else{
                  (len > I2C_BUFFER_LENGTH/pgm_read_byte(&pBuf[0])) ? n = I2C_BUFFER_LENGTH/pgm_read_byte(&pBuf[0]) : n = len;
              }
              for(uint32_t i = 0; i < n; i++){
                  p->pro.iic->write(pgm_read_byte(&pBuf[1]));
                  if(pBuf[0] > 1)
                      p->pro.iic->write(pgm_read_byte(&pBuf[2]));
                  else if(pBuf[0] > 2)
                      p->pro.iic->write(pgm_read_byte(&pBuf[3]));
                  else if(pBuf[0] > 3)
                      p->pro.iic->write(pgm_read_byte(&pBuf[4]));
                  #if defined(ESP32)
                  p->pro.iic->flush();
                  #endif
              }
              len -= n;
              p->pro.iic->endTransmission();
          }
      }
           break;
      case IF_COM_WRITE_FLASH_INC:
      {
          Serial.println("IF_COM_WRITE_FLASH_INC");
          if(!(p->isBegin)) return 0;
          while(len){
              p->pro.iic->beginTransmission(p->pinList[IF_PIN_ADDR]);
              uint32_t n = 0;
              if(p->dev->devName == DEV_TYPE_SCREEN)
              {
                  (len > (I2C_BUFFER_LENGTH - 1)) ? n = I2C_BUFFER_LENGTH - 1 : n = len;
                  p->pro.iic->write(0x40);
              }else{
                  (len > I2C_BUFFER_LENGTH) ? n = I2C_BUFFER_LENGTH : n = len;
              }
              for(uint32_t i = 0; i < n; i++){
                  p->pro.iic->write(pgm_read_byte(pBuf));
                  pBuf++;
              }
              len -= n;
              p->pro.iic->endTransmission();
          }
           break;
      }
      case IF_COM_WRITE_RAM_FIXED:
      {
          Serial.println("IF_COM_WRITE_RAM_FIXED");
          if(!(p->isBegin)) return 0;
          while(len){
              p->pro.iic->beginTransmission(p->pinList[IF_PIN_ADDR]);
              uint32_t n = 0;
              if(p->dev->devName == DEV_TYPE_SCREEN)
              {
                  (len > ((I2C_BUFFER_LENGTH-1)/pBuf[0])) ? n = ((I2C_BUFFER_LENGTH-1)/pBuf[0]) : n = len;
                  p->pro.iic->write(0x40);
              }else{
                  (len > (I2C_BUFFER_LENGTH)/pBuf[0]) ? n = I2C_BUFFER_LENGTH/pBuf[0] : n = len;
              }
              for(uint32_t i = 0; i < n; i++){
                  if(pBuf[0] < 5){
                      p->pro.iic->write(pBuf[1]);
                      if(pBuf[0] > 1)
                          p->pro.iic->write(pBuf[2]);
                      else if(pBuf[0] > 2)
                          p->pro.iic->write(pBuf[3]);
                      else if(pBuf[0] > 3)
                          p->pro.iic->write(pBuf[4]);
                  }else{
                      for(uint8_t j = 0; j < pBuf[0]; j++){
                          p->pro.iic->write(pBuf[1+j]);
                      }
                  }
              }
              len -= n;
              p->pro.iic->endTransmission();
          }
      }
           break;
      case IF_COM_WRITE_RAM_INC:
      {
          //Serial.println("IF_COM_WRITE_RAM_INC");
          if(!(p->isBegin)) return 0;
          while(len){
              p->pro.iic->beginTransmission(p->pinList[IF_PIN_ADDR]);
              uint32_t n = 0;
			  if(p->dev->devName == DEV_TYPE_SCREEN)
              {
                  (len > I2C_BUFFER_LENGTH - 1) ? n = I2C_BUFFER_LENGTH - 1 : n = len;
                  p->pro.iic->write(0x40);
                  for(uint32_t i = 0; i < n; i++){
                      p->pro.iic->write(pBuf[i]);
                  }
                  pBuf += n;
                  len -= n;
                  p->pro.iic->endTransmission();
              }else{
                  p->pro.iic->write((const uint8_t *)(pBuf+1),(size_t)pBuf[0]);
                  if(len > I2C_BUFFER_LENGTH) return 0;
                  p->pro.iic->write(&pBuf[(1+pBuf[0])],len);
                  if( p->pro.iic->endTransmission() != 0) return 0;
                  len -= len;
              }
          }
      }
           break;
      default:
           break;
  }
  return 1;
}