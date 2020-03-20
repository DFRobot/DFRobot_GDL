#ifdef ARDUINO_SAM_ZERO
#include "DFRobot_DMA.h"

int DFRobot_DMA::_beginCount = 0;

DFRobot_DMA::DFRobot_DMA():_channelMask(0){
  memset(_descriptors, 0x00, sizeof(_descriptors));
  memset(_descriptorsWriteBack, 0x00, sizeof(_descriptorsWriteBack));
}

void DFRobot_DMA::begin(){
  if(_beginCount == 0){
      PM->AHBMASK.bit.DMAC_ = 1;
      PM->APBBMASK.bit.DMAC_ = 1;
      DMAC->CTRL.bit.CRCENABLE = 0;
      DMAC->CTRL.bit.DMAENABLE=0;
      DMAC->CTRL.bit.SWRST = 1;
      
      // configure the descriptor addresses
      DMAC->BASEADDR.bit.BASEADDR = (uint32_t)_descriptors;
      DMAC->WRBADDR.bit.WRBADDR = (uint32_t)_descriptorsWriteBack;
      
      // enable with all levels
      DMAC->CTRL.bit.LVLEN0 = 1;
      DMAC->CTRL.bit.LVLEN1 = 1;
      DMAC->CTRL.bit.LVLEN2 = 1;
      DMAC->CTRL.bit.LVLEN3 = 1;
      DMAC->CTRL.bit.DMAENABLE = 1;
  }
  _beginCount++;
}

uint8_t DFRobot_DMA::allocateChannel(){
  uint8_t channel = 0xFF;
  for(uint8_t i = 0; i < DF_DMA_NUM_CHANNELS; i++){
	  if((_channelMask &(1 << i)) == 0){
		  _channelMask |= (1 << i);
		  memset((void*)&_descriptors[i], 0x00, sizeof(_descriptors[i]));//将通道里的所有数据清0
		  DMAC->CHID.bit.ID = i;//记录通道ID
          DMAC->CHCTRLA.bit.ENABLE = 0;
          DMAC->CHCTRLA.bit.SWRST = 1;//通道复位
		  channel = i;
		  break;
	  }
  }
  return channel;
}

void DFRobot_DMA::setSrcAddr(uint8_t channel, void *src, uint16_t size, bool inc){
  _descriptors[channel].SRCADDR.bit.SRCADDR = (uint32_t)src;
  if(inc){
      _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
      _descriptors[channel].BTCTRL.bit.SRCINC = 1;
	  _descriptors[channel].SRCADDR.bit.SRCADDR += size;
  }else{
      _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
      _descriptors[channel].BTCTRL.bit.SRCINC = 0;
  }
  
}

void DFRobot_DMA::setDstAddr(uint8_t channel, void *dst, uint16_t size, bool inc){
  _descriptors[channel].DSTADDR.bit.DSTADDR = (uint32_t)dst;
  if(inc){
      _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
      _descriptors[channel].BTCTRL.bit.DSTINC = 1;
	  _descriptors[channel].DSTADDR.bit.DSTADDR += size;
  }else{
      _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
      _descriptors[channel].BTCTRL.bit.DSTINC = 0;
  }
  
}

void DFRobot_DMA::setRoundTransMode(bool flag){
  
}
void DFRobot_DMA::setIncMode(uint8_t channel, uint8_t mode){
  switch(mode){
	  case DF_DMA_SRCINC:
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
		   _descriptors[channel].BTCTRL.bit.SRCINC = 1;
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
		   _descriptors[channel].BTCTRL.bit.DSTINC = 0;
		   break;
	  case DF_DMA_DSTINC:
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
		   _descriptors[channel].BTCTRL.bit.SRCINC = 0;
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
		   _descriptors[channel].BTCTRL.bit.DSTINC = 1;
		   break;
	  case DF_DMA_BOTHINC:
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
		   _descriptors[channel].BTCTRL.bit.SRCINC = 1;
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
		   _descriptors[channel].BTCTRL.bit.DSTINC = 1;
		   break;
	  case DF_DMA_BOTHPAUSE:
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_SRC_Val;
		   _descriptors[channel].BTCTRL.bit.SRCINC = 0;
	       _descriptors[channel].BTCTRL.bit.STEPSEL = DMAC_BTCTRL_STEPSEL_DST_Val;
		   _descriptors[channel].BTCTRL.bit.DSTINC = 0;
		   break;
  }
}

void DFRobot_DMA::setDataTransWidthAndSize(uint8_t channel, uint16_t size, uint8_t width){
  switch (width) {
    case 1:
    default:
      _descriptors[channel].BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_BYTE_Val;
      break;

    case 2:
      _descriptors[channel].BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_HWORD_Val;
      break;

    case 4:
      _descriptors[channel].BTCTRL.bit.BEATSIZE = DMAC_BTCTRL_BEATSIZE_WORD_Val;
      break;
  }
  _descriptors[channel].BTCNT.bit.BTCNT = size / width; 
}

void DFRobot_DMA::setPorityLevel(uint8_t channel, uint8_t level){
  DMAC->CHID.bit.ID = channel;
  DMAC->CHCTRLB.bit.LVL = level;
}

void DFRobot_DMA::setTriggerSource(uint8_t channel, int source)
{
  DMAC->CHID.bit.ID = channel;
  _descriptors[channel].DESCADDR.bit.DESCADDR = 0;
  _descriptors[channel].BTCTRL.bit.EVOSEL = DMAC_BTCTRL_EVOSEL_DISABLE_Val;
  _descriptors[channel].BTCTRL.bit.BLOCKACT = DMAC_BTCTRL_BLOCKACT_NOACT_Val;
  DMAC->CHCTRLB.bit.TRIGSRC = source;

  if (DMAC->CHCTRLB.bit.TRIGSRC) {
    DMAC->CHCTRLB.bit.TRIGACT = DMAC_CHCTRLB_TRIGACT_BEAT_Val;
  } else {
    DMAC->CHCTRLB.bit.TRIGACT = DMAC_CHCTRLB_TRIGACT_BLOCK_Val;
  }
}

void DFRobot_DMA::start(uint8_t channel){
  while(_descriptorsWriteBack[channel].BTCTRL.bit.VALID); 
  _descriptors[channel].BTCTRL.bit.VALID = 1;
  DMAC->CHCTRLA.bit.ENABLE = 1;
}

void DFRobot_DMA::end(){
  DMAC->CTRL.bit.DMAENABLE = 0;
  PM->APBBMASK.bit.DMAC_ = 0;
  PM->AHBMASK.bit.DMAC_ = 0;
}

void DFRobot_DMA_SPI::begin(){
  DFRobot_DMA::begin();
  _channel = allocateChannel();//分配通道
  if(_channel == DMA_CHANNEL_NONE)
	  return;
  setPorityLevel(_channel, 0);//将通道优先级设置为最高
  setTriggerSource(_channel, 0x0A);
  setDataTransWidthAndSize(_channel, 0, 1);
  setDstAddr(_channel, (uint32_t *)0x42001828);
  setIncMode(_channel, DF_DMA_SRCINC);//设置增量模式
  SPI.begin();
  sercom4.disableSPI();
  while(SERCOM4->SPI.SYNCBUSY.bit.ENABLE);
  SERCOM4->SPI.BAUD.reg = 0; 
  sercom4.enableSPI();
}

void DFRobot_DMA_SPI::transfer(void *src, uint16_t size){
  while (_descriptorsWriteBack[0].BTCTRL.bit.VALID); 
  _descriptors[_channel].SRCADDR.bit.SRCADDR = (uint32_t)src+(uint32_t)size;
  _descriptors[0].BTCNT.bit.BTCNT = size;
  _descriptors[0].BTCTRL.bit.VALID = 1;
  DMAC->CHCTRLA.bit.ENABLE = 1;
}
DFRobot_DMA_SPI DMASPI;
#endif