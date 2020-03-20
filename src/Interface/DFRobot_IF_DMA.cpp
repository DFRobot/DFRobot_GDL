#include "DFRobot_IF.h"
#include <DFRobot_Type.h>
#ifdef ARDUINO_SAM_ZERO
#define DF_DMA_SPI_TX_REG 0x42001828
#define DMA_BUFFER_SIZE  0xFFFF

uint8_t interfaceComDmaSPI(sGdlIF_t *p, uint8_t cmd, uint8_t *pBuf, uint32_t len){
  if(p == NULL) return 0;
  if(p->isBegin){
      #if defined(ARDUINO_SAM_ZERO)
     if(p->freq > 12000000)
           {
          sercom4.disableSPI();
          while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
          SERCOM4->SPI.BAUD.reg = 0; 
          sercom4.enableSPI();
      }else{
           p->pro.spi->beginTransaction(SPISettings(p->freq, MSBFIRST, SPI_MODE0));
      }
      #endif
  }
  switch(cmd){
    case IF_COM_PROTOCOL_INIT:
    {
        p->pro.dma = &DMASPI;
        p->pro.dma->begin();
        p->isBegin = true;
    }
          break;
    case IF_COM_SET_FREQUENCY:
    {
           if(!p->freq) return 0;
           #if defined(ARDUINO_SAM_ZERO)
            if(p->freq > 12000000)
           {
               sercom4.disableSPI();
               while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
               SERCOM4->SPI.BAUD.reg = 0; 
               sercom4.enableSPI();
           }else{
               p->pro.spi->beginTransaction(SPISettings(p->freq, MSBFIRST, SPI_MODE0));
           }
           #endif
      }
    case IF_COM_WRITE_CMD:
    {     if(!p->isBegin) return 0;
          PIN_LOW(p->pinList[IF_PIN_CS]);
          if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_LOW(p->pinList[IF_PIN_DC]);
          do{
              uint32_t n = 0;
              (len > DMA_BUFFER_SIZE) ? n = DMA_BUFFER_SIZE : n = len;
              len -= n;
              p->pro.dma->transfer(pBuf, n);
              pBuf += n;
          }while(len);
          PIN_HIGH(p->pinList[IF_PIN_CS]); 
          if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
    }
          break;
    case IF_COM_READ_DATA:
          break;
    case IF_COM_WRITE_FLASH_INC:
          if(!(p->isBegin)) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           do{
               uint32_t n = 0;
               (len > DMA_BUFFER_SIZE) ? n = DMA_BUFFER_SIZE : n = len;
                len -= n;
                for(uint8_t i = 0; i < n; i++){
                    uint8_t b = pgm_read_byte(pBuf+i);
                    p->pro.dma->transfer(&b, 1);
                }
                pBuf += n;
           }while(len);
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
          break;
    case IF_COM_WRITE_RAM_FIXED:
      {
           if(!(p->isBegin)) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           do{
               p->pro.dma->transfer(pBuf+1, pBuf[0]);
               len--;
           }while(len);
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break; 
    case IF_COM_WRITE_RAM_INC:
      {
           if(!(p->isBegin)) return 0;
           PIN_LOW(p->pinList[IF_PIN_CS]);
           do{
               uint32_t n = 0;
               (len > DMA_BUFFER_SIZE) ? n = DMA_BUFFER_SIZE : n = len;
                len -= n;
                p->pro.dma->transfer(pBuf, n);
                pBuf += n;
           }while(len);
           PIN_HIGH(p->pinList[IF_PIN_CS]); 
           if(p->dev->devName == DEV_TYPE_SCREEN)
               PIN_HIGH(p->pinList[IF_PIN_DC]);
      }
           break; 
    default:
            break;
  }
  
  //  p->pro.spi->endTransaction();
    SPI.setClockDivider(12);
}

#endif