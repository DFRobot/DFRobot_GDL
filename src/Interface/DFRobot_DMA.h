#ifdef ARDUINO_SAM_ZERO
#include <Arduino.h>
#include <SPI.h>
#pragma once
#define DF_DMA_NUM_CHANNELS  1//Define the channel number, the DMA channel of each board is different

#define DMA_CHANNEL_NONE   0xFF //Define invalid channel number as 0xFF
//Define data width
#define DF_DMA_TRANSFER_WIDTH_BYTE  1
#define DF_DMA_TRANSFER_WIDTH_HWORD  2
#define DF_DMA_TRANSFER_WIDTH_WORD  4
//Define data growth mode
#define DF_DMA_SRCINC     0
#define DF_DMA_DSTINC     1
#define DF_DMA_BOTHINC    2
#define DF_DMA_BOTHPAUSE  3

//The transmission direction is always src-> dst
 
class DFRobot_DMA{//A customized DMA, universal function
public:
  DFRobot_DMA();
  virtual ~DFRobot_DMA(){}
  void begin();
  uint8_t allocateChannel();//Assign the channel and set the channel ID, if it returns 0xFF, it means the allocation failed
  void setIncMode(uint8_t channel, uint8_t mode);//Target address growth, SRC target address growth
  void setSrcAddr(uint8_t channel, void *src, uint16_t size = 0, bool inc = false);//Set the source address, and the address growth mode, false means no increase
  void setDstAddr(uint8_t channel, void *dst, uint16_t size = 0, bool inc = false);//Set the target address and address growth mode, false means no increase
  void setRoundTransMode(bool flag = false);//Whether set to cyclic transmission mode, false: acyclic transmission mode, true：cyclic transmission mode
  void setDataTransWidthAndSize(uint8_t channel, uint16_t size, uint8_t width);//Set the total data transfer volume and the number of bytes per transfer (byte, halfword, word)
  void setPorityLevel(uint8_t channel, uint8_t level);
  void setTriggerSource(uint8_t channel, int source);
  void start(uint8_t channel);//Start transferring
  void end();


  static int _beginCount;
  uint32_t _channelMask;
  DmacDescriptor _descriptors[DF_DMA_NUM_CHANNELS] __attribute__ ((aligned (16)));
  DmacDescriptor _descriptorsWriteBack[DF_DMA_NUM_CHANNELS]  __attribute__ ((aligned (16)));
};

extern DFRobot_DMA DMA1;

class DFRobot_DMA_SPI: public DFRobot_DMA{
public:
  SPIClass *_spi;
  DFRobot_DMA_SPI():_channel(0){}
  void begin();
  void transfer(void *src, uint16_t size);
///  int transfer
private:
  uint8_t _channel;
};
extern DFRobot_DMA_SPI DMASPI;
#endif
