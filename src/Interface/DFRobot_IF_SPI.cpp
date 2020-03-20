#include "DFRobot_IF.h"
#include <DFRobot_Type.h>

#if defined(__AVR__)
#define AVR_SPI_WRITE(d) for(SPDR = d; (!(SPSR & _BV(SPIF))); )
#endif

uint8_t interfaceComHardwareSPI(sGdlIF_t *p, uint8_t cmd, uint8_t *pBuf, uint32_t len){
  if((p == NULL))
      return 0;
  if(p->isBegin && cmd != IF_COM_SET_FREQUENCY){
      #if defined(SPI_HAS_TRANSACTION)
      #if defined(ARDUINO_SAM_ZERO)
      uint8_t baud = 48000000/(p->freq*2) - 1;
      sercom4.disableSPI();
      while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
         SERCOM4->SPI.BAUD.reg = baud; 
         sercom4.enableSPI();

      #else
          p->pro.spi->beginTransaction(SPISettings(p->freq, MSBFIRST, SPI_MODE0));
      #endif
      #else
      #if defined(__AVR__)
         p->pro.spi->setClockDivider(SPI_CLOCK_DIV2);
      #elif defined(ESP8266) || defined(ESP32)
         p->pro.spi->setFrequency(p->freq);
      #endif
         p->pro.spi->setBitOrder(MSBFIRST);
         p->pro.spi->setDataMode(SPI_MODE0);
      #endif
  }
  switch(cmd){
      case IF_COM_PROTOCOL_INIT:
      {
           p->pro.spi = &SPI;
           p->pro.spi->begin();
           if(p->freq == 0) {
             p->freq = DEFAULT_SPI_FREQ;
           }else{
             #if defined(SPI_HAS_TRANSACTION)
             #if defined(ARDUINO_SAM_ZERO)
             uint8_t baud = 48000000/(p->freq*2) - 1;
             sercom4.disableSPI();
             while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
             SERCOM4->SPI.BAUD.reg = baud; 
             sercom4.enableSPI();
             #else
                  p->pro.spi->beginTransaction(SPISettings(p->freq, MSBFIRST, SPI_MODE0));
             #endif
             #else
             #if defined(__AVR__)
                    p->pro.spi->setClockDivider(SPI_CLOCK_DIV2);
             #elif defined(ESP8266) || defined(ESP32)
                    p->pro.spi->setFrequency(p->freq);
             #endif
                    p->pro.spi->setBitOrder(MSBFIRST);
                    p->pro.spi->setDataMode(SPI_MODE0);
             #endif 
           }
           p->isBegin = true;
      }
           break;
      case IF_COM_SET_FREQUENCY:
      {
           //Serial.println("IF_COM_SET_FREQUENCY");
           if(!p->freq) return 0;
           #if defined(SPI_HAS_TRANSACTION)
           #if defined(ARDUINO_SAM_ZERO)
           uint8_t baud = 48000000/(p->freq*2) - 1;
           sercom4.disableSPI();
           while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
           SERCOM4->SPI.BAUD.reg = baud; 
           sercom4.enableSPI();
           #else
                 p->pro.spi->beginTransaction(SPISettings(p->freq, MSBFIRST, SPI_MODE0));
           #endif
           #else // No transactions, configure SPI manually...
           #if defined(__AVR__)
                  p->pro.spi->setClockDivider(SPI_CLOCK_DIV2);
           #elif defined(ESP8266) || defined(ESP32)
                  p->pro.spi->setFrequency(p->freq);
           #endif
                  p->pro.spi->setBitOrder(MSBFIRST);
                  p->pro.spi->setDataMode(SPI_MODE0);
           #endif 
      }
           break;
      case IF_COM_WRITE_CMD:
      {
           if(!(p->isBegin)) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_LOW(p->pinList[IF_PIN_DC]);
           for(uint32_t i = 0; i < len; i++){
               #if defined(ESP8266)
               if(i%100000 == 0) yield();
               #endif
               p->pro.spi->transfer(pBuf[i]);
           }
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break;
      case IF_COM_READ_DATA:
      {
          // delay(10);
           if(!(p->isBegin)) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           for(uint8_t i = 0; i < pBuf[0]; i++){
               p->pro.spi->transfer(pBuf[i+1]);
           }
           for(uint32_t i = 0; i < len; i++){
               #if defined(ESP8266)
               if(i%100000 == 0) yield();
               #endif
               pBuf[i] = p->pro.spi->transfer(0x00);
           }
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break;
      case IF_COM_WRITE_FLASH_FIXED:   //只能一次处理4个字节
      {
           //Serial.println("IF_COM_WRITE_FLASH_FIXED");
           uint8_t num = pgm_read_byte(&pBuf[0]);
           if(!(p->isBegin) || num > 4) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           uint8_t *addr = pBuf + 1;
           uint8_t buf[num];
           do{
               uint32_t datBytes = len;
               uint32_t args = 100000/num;
               if(datBytes > args) datBytes = args;
               #if defined(ESP8266)
               yield();
               #endif
               len -= datBytes;
               while(datBytes--){
                   switch(num){
                       case 1: 
                               #if defined(__AVR__)
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[1]));
                               #else
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[1]));
                               #endif
                               break;
                       case 2: 
                               #if defined(__AVR__)
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[1]));
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[2]));
                               #else
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[1]));
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[2]));
                               #endif
                               break;
                       case 3: 
                               #if defined(__AVR__)
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[1]));
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[2]));
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[3]));
                               #else
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[1]));
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[2]));
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[3]));
                               break;
                               #endif
                       case 4: 
                               #if defined(__AVR__)
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[1]));
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[2]));
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[3]));
                               AVR_SPI_WRITE(pgm_read_byte(&pBuf[4]));
                               #else
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[1]));
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[2]));
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[3]));
                               p->pro.spi->transfer(pgm_read_byte(&pBuf[4]));
                               #endif
                               break;
                       default:
                              break;
                   }
               }
           }while(len);
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break;
      case IF_COM_WRITE_FLASH_INC:
      {
           if(!(p->isBegin)) return 0;
           
           //Serial.println("IF_COM_WRITE_FLASH_INC");
           PIN_LOW(p->pinList[IF_PIN_CS]);
           do{
               uint32_t datBytes = len;
               #if defined(ESP8266)
               if(datBytes > 100000) datBytes = 100000;
               yield();
               #endif
               len -= datBytes;
               while(datBytes--){
                   #if defined(__AVR__)
                   AVR_SPI_WRITE(pgm_read_byte(pBuf));
                   #else
                   p->pro.spi->transfer(pgm_read_byte(pBuf));
                   #endif
                   pBuf++;
               }
           }while(len);
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break;
      case IF_COM_WRITE_RAM_FIXED:
      {
           if(!(p->isBegin)) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           #if defined(ESP32)
           uint8_t bytesLen = pBuf[0]/4;
           uint8_t bytesMod = pBuf[0]%4;
           uint8_t index = bytesLen*4;
           uint8_t buf[4];
           memset(buf, 0 , 4);
           for(uint8_t i = 0; i < bytesLen; i++){
               memcpy(buf, pBuf+1+i*4, 4);
               pBuf[1+i*4] = buf[3];
               pBuf[1+i*4 + 1] = buf[2];
               pBuf[1+i*4 + 2] = buf[1];
               pBuf[1+i*4 + 3] = buf[0];
            }
            if(bytesMod)
                memcpy(buf, &pBuf[1+index], bytesMod);
            for(uint8_t i = 0; i < bytesMod; i++){
               pBuf[index+i+1] = buf[bytesMod - 1 - i];
            }
           #endif
           do{
               uint32_t datBytes = len;
               uint32_t args = 100000/pBuf[0];
               if(datBytes > args) datBytes = args;
               #if defined(ESP8266)
               yield();
               #endif
               len -= datBytes;
               #if defined(ESP32)
               while(datBytes--){
					if(pBuf[0] == 3){
						p->pro.spi->writePixels(pBuf+3, 1);
                        p->pro.spi->writePixels((const void *)(pBuf+1), 2);
					}else{
						p->pro.spi->writePixels((const void *)(pBuf+1), pBuf[0]);
					}
               }
               #else
		
               while(datBytes--){
                   if(pBuf[0] < 5){
                       #if defined(__AVR__)

                       if(pBuf[0] == 1){
                       AVR_SPI_WRITE(pBuf[1]);
					   }
					   else if(pBuf[0] == 2){
						  AVR_SPI_WRITE(pBuf[1]);
					      AVR_SPI_WRITE(pBuf[2]);
					   
					   }
                       else if(pBuf[0] == 3){
					
						  AVR_SPI_WRITE(pBuf[1]);
					      AVR_SPI_WRITE(pBuf[2]);
                           AVR_SPI_WRITE(pBuf[3]);
					   }
                       else if(pBuf[0] == 4){
						  AVR_SPI_WRITE(pBuf[1]);
					      AVR_SPI_WRITE(pBuf[2]);
                           AVR_SPI_WRITE(pBuf[3]);
                           AVR_SPI_WRITE(pBuf[4]);
					   }
					   
                       #else
                       if(pBuf[0] == 1)
						   p->pro.spi->transfer(pBuf[1]);
                       else if(pBuf[0] == 2){
						   p->pro.spi->transfer(pBuf[1]);
                           p->pro.spi->transfer(pBuf[2]);
					   }
                       else if(pBuf[0] == 3){
					   
						   p->pro.spi->transfer(pBuf[1]);
                           p->pro.spi->transfer(pBuf[2]);
						   p->pro.spi->transfer(pBuf[3]);
					   }
                       else if(pBuf[0] == 4){
                           
						   p->pro.spi->transfer(pBuf[1]);
                           p->pro.spi->transfer(pBuf[2]);
						   p->pro.spi->transfer(pBuf[3]);
						   p->pro.spi->transfer(pBuf[4]);
					   }
                       #endif
                   }else{
                       for(uint8_t i = 0; i < pBuf[0];i++){
                           #if defined(__AVR__)
                           AVR_SPI_WRITE(pBuf[1+i]);
                           #else
                           p->pro.spi->transfer(pBuf[1]);
                           #endif
                       }
                   }
               }
               #endif
           }while(len);
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break;
      case IF_COM_WRITE_RAM_INC:
      {
           //Serial.println("IF_COM_WRITE_RAM_INC");
           if(!(p->isBegin)) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           do{
               uint32_t datBytes = len;
               #if defined(ESP8266)
               if(datBytes > 100000) datBytes = 100000;
               yield();
               #endif
               len -= datBytes;
               while(datBytes--){
                   #if defined(__AVR__)
                   AVR_SPI_WRITE(*pBuf);
                   #else
                   p->pro.spi->transfer(*pBuf);
                   #endif
                   pBuf++;
               }
           }while(len);
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break;
      default:
            break;
  }
#if defined(ESP8266)
  yield();
#endif
  p->pro.spi->endTransaction();
     #if defined(ARDUINO_SAM_ZERO)
         SPI.setClockDivider(12); 
     #endif

  return 1;
}