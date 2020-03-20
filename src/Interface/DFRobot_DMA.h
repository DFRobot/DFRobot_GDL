#ifdef ARDUINO_SAM_ZERO
#include <Arduino.h>
#include <SPI.h>
#pragma once
#define DF_DMA_NUM_CHANNELS  1//定义通道号，每个板子的DMA通道不一样

#define DMA_CHANNEL_NONE   0xFF //定义无效通道号为0xFF
//定义数据宽度
#define DF_DMA_TRANSFER_WIDTH_BYTE  1
#define DF_DMA_TRANSFER_WIDTH_HWORD  2
#define DF_DMA_TRANSFER_WIDTH_WORD  4
//定义数据增长模式
#define DF_DMA_SRCINC     0
#define DF_DMA_DSTINC     1
#define DF_DMA_BOTHINC    2
#define DF_DMA_BOTHPAUSE  3

//传输方向永远是src-->dst

class DFRobot_DMA{//一个被定制的DMA，通用功能
public:
  DFRobot_DMA();
  virtual ~DFRobot_DMA(){}
  void begin();
  uint8_t allocateChannel();//分配通道并设置通道ID，如果返回0xFF，则表示分配失败
  void setIncMode(uint8_t channel, uint8_t mode);//目标地址增长、SRC目标地址增长
  void setSrcAddr(uint8_t channel, void *src, uint16_t size = 0, bool inc = false);//设置源地址,及地址增长模式 false为不增长
  void setDstAddr(uint8_t channel, void *dst, uint16_t size = 0, bool inc = false);//设置目标地址及地址增长模式 false为不增长
  void setRoundTransMode(bool flag = false);//是否设置循环传输模式，false:非循环传输模式，true循环传输模式
  void setDataTransWidthAndSize(uint8_t channel, uint16_t size, uint8_t width);//设置总数据传输量和每次传输的字节数（字节、半字、字）
  void setPorityLevel(uint8_t channel, uint8_t level);
  void setTriggerSource(uint8_t channel, int source);
  void start(uint8_t channel);//开始传输
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