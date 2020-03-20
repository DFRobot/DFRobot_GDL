/*!
 * @file DFRobot_IF.h
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

#ifndef __DFROBOT_IF_H
#define __DFROBOT_IF_H

#include "DFRobot_Type.h"
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#ifdef ARDUINO_SAM_ZERO
#include "DFRobot_DMA.h"
#endif


/*硬件IIC一次能传输的最大字节数*/
#ifdef I2C_BUFFER_LENGTH
#undef I2C_BUFFER_LENGTH
#endif
#if defined(__AVR__)||defined(ESP8266) //#ifdef ARDUINO_ARCH_AVR  //AVR系列单片机一次最多传32字节数据
#define I2C_BUFFER_LENGTH  32
#elif defined(ESP32)
#define I2C_BUFFER_LENGTH  31
#elif defined(ARDUINO_SAM_ZERO)
#define I2C_BUFFER_LENGTH  256
#else
#define I2C_BUFFER_LENGTH  16
#endif

#ifdef MCU_SPI_FREQ
#undef MCU_SPI_FREQ
#endif
#ifdef DEFAULT_SPI_FREQ
#undef DEFAULT_SPI_FREQ
#endif
#if defined(__AVR__)
 #define MCU_SPI_FREQ  8000000L  ///8M
#elif defined(ESP32)||defined(ESP8266)
 #define MCU_SPI_FREQ  40000000L//40M
#elif defined(ARDUINO_SAM_ZERO)
 #define MCU_SPI_FREQ  24000000L  ///24M
#else
 #define MCU_SPI_FREQ 16000000L  ///< Hardware SPI default speed
#endif

#if defined(ESP32)||defined(ESP8266)
#define DEFAULT_SPI_FREQ  1000000//1M
#else
#define DEFAULT_SPI_FREQ  4000000L
#endif

/*******************************************************
定义设备类型，例：屏和触摸
********************************************************/
#define DEV_TYPE_TOUCH  0
#define DEV_TYPE_SCREEN 1
#define DEV_TYPE_NONE 255

/**
 * @brief Communication interface function command.
 * @param IF_COM_PROTOCOL_INIT: Communication protocol initialization.
 * @param IF_COM_SET_FREQUENCY: 设置通信频率.
 * @param IF_COM_WRITE_CMD: 写命令.
 * @param IF_COM_READ_DATA: 写数据
 * @param IF_COM_WRITE_FLASH_FIXED: 传输保存在ROM中的数据，且传输过程中数据位置不变，及多次传输同一个数据
 * @param IF_COM_WRITE_FLASH_INC: 传输保存在ROM中的数据，且数据地址依次增加
 * @param IF_COM_WRITE_RAM_FIXED: 传输保存在RAM中的数据，且传输过程中数据位置不变，及多次传输同一个数据
 * @param IF_COM_WRITE_RAM_INC: 传输保存在RMA中的数据，且数据地址依次增加
 */
#define IF_COM_PROTOCOL_INIT  0
#define IF_COM_SET_FREQUENCY     1
#define IF_COM_WRITE_CMD      2
#define IF_COM_READ_DATA      3
#define IF_COM_WRITE_FLASH_FIXED 4
#define IF_COM_WRITE_FLASH_INC   5
#define IF_COM_WRITE_RAM_FIXED   6
#define IF_COM_WRITE_RAM_INC     7

/*无用引脚，空引脚*/
#define GDL_PIN_NC     0xFF 
/*IO口操纵方式*/
#define PIN_IN(pin)   if(pin != GDL_PIN_NC) pinMode(pin, INPUT)
#define PIN_OUT(pin)  if(pin != GDL_PIN_NC) pinMode(pin, OUTPUT)
#define PIN_HIGH(pin)  if(pin != GDL_PIN_NC) digitalWrite(pin, HIGH)
#define PIN_LOW(pin)  if(pin != GDL_PIN_NC) digitalWrite(pin, LOW)


#define IF_HW_IIC 3  //3代表硬件IIC的最大参数个数，即(rst pin)，(bl pin),addr
#define IF_SW_IIC 5  //5代表软件IIC的最大参数个数，即(rst),(bl),addr,scl,sda,
#define IF_HW_SPI 4  //4代表硬件SPI的最大参数个数，即(rst),(bl),dc,(cs)
#define IF_SW_SPI 7  //7代表硬件SPI的最大参数个数，即(rst),(bl),dc,(cs),sck,mosi,(miso)


#define IF_PIN_RST    0 //复位
#define IF_PIN_BL     1 //背光
#define IF_PIN_IRQ    1 //中断
#define IF_PIN_ADDR   2 //IIC地址
#define IF_PIN_DC     2 //SPI命令数据控制引脚
#define IF_PIN_CS     3 //SPI片选引脚

typedef struct sGdlIF sGdlIF_t;
typedef uint8_t(*devInterfaceFunctionPtr)(sGdlIF_t *gdl, uint8_t step, uint8_t *addr, uint32_t len);//FP->function pointer函数指针

typedef struct{
  uint8_t devName; /**<记录设备类型，DEV_TYPE_TOUCH代表触摸，DEV_TYPE_SCREEN代表屏*/
  uint8_t *addr;  /**<记录设备的初始化数组*/
  devInterfaceFunctionPtr talk;/**<注册屏的通信接口函数，指向函数的指针*/
}sGdlIFDev_t;

typedef union{
  TwoWire *iic;/**<定义一个IIC类指针*/
  SPIClass *spi;/**<定义一个SPI类指针*/
#ifdef ARDUINO_SAM_ZERO
  DFRobot_DMA_SPI *dma;/**<定义一个dma类指针*/
#endif
  void *interface;/*公共接口*/
}uProtocol_t;

struct sGdlIF{
  uint8_t interface;/**<存储屏的显示通信类型*/
  uint32_t freq;/**<通信频率*/
  uProtocol_t pro;/**<通信协议：IIC，并口，SPI，DMA等等*/
  bool  isBegin;/**<判断初始化是否成功*/
  uint8_t *pinList;/**<保存通信接口的引脚数据，它的长度由某种通信接口的最大引脚数决定*/
  uint16_t length;/**<通信的最大字节数，默认0xFFFF*/
  sGdlIFDev_t *dev;/**<记录设备（屏或触摸）的初始化数组和通信接口*/
}__attribute__ ((packed));


#define GDL_IF_PB_DEV(name, type, init, interface) \
sGdlIFDev_t name = {type, init, interface};

/*定义不同接口的功能函数*/
uint8_t interfaceComHardwareSPI(sGdlIF_t *p, uint8_t cmd, uint8_t *pBuf, uint32_t len);
uint8_t interfaceComHardwareIIC(sGdlIF_t *p, uint8_t cmd, uint8_t *pBuf, uint32_t len);
uint8_t interfaceComDmaSPI(sGdlIF_t *p, uint8_t cmd, uint8_t *pBuf, uint32_t len);

#define IF_COM_HW_SPI interfaceComHardwareSPI
#define IF_COM_HW_IIC interfaceComHardwareIIC
#define IF_COM_DMA_SPI interfaceComDmaSPI


class DFRobot_IF{
public:
  sGdlIF_t _if;/*interface*/
  /**
   * @brief Constructor  获取IIC接口的信息
   * @param dev  通信接口结构体指针，该结构体保存了屏的通信接口类型、通信频率、相关IO引脚，在不同的主控上，一次通信处理的最大字节数，及屏初始化数组和接口函数指针
   * @param addr  IIC通信地址
   * @param rst  复位信号
   * @param irq  中断信号
   */
  DFRobot_IF(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t bl/*irq*/);
  /**
   * @brief Constructor  获取SPI接口的信息
   * @param dev  通信接口结构体指针，该结构体保存了屏的通信接口类型、通信频率、相关IO引脚，在不同的主控上，一次通信处理的最大字节数，及屏初始化数组和接口函数指针
   * @param dc   有些设备通过DC来区分数据和命令2种数据
   * @param cs   SPI片选信号
   * @param rst  复位信号
   * @param bl   背光或中断信号
   */
  DFRobot_IF(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/);
  ~DFRobot_IF();
  /**
   * @brief 通信接口初始化
   */
  void initInterface();
  /**
   * @brief 设置通信接口频率
   * @param freq  频率，单位Hz
   */
  void setFrequency(uint32_t freq);
protected:
  void initIFCommon();
  void initHWIIC(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t bl/*irq*/);
  void initHWSPI(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/);
  void writeData32(uint32_t data, bool isRamData);
  void writeBuf(uint16_t reg, void *pBuf, uint32_t len, bool flag = false);//flag代表数据和寄存器是否匹配
  /**
   * @brief 读寄存器数据
   * @param reg  可以是寄存器
   * @param pBuf  接收数组
   * @param len  读取的字节数
   * @param flag  当reg代表寄存器时，flag为false，代表16位寄存器对应一个8位的数据，true代表对应16位数据
   */
  void readReg(uint16_t reg, void *pBuf, uint32_t len, bool flag = false);
  void readCommand(uint8_t cmd, void *pBuf, uint32_t len);
  void sendBuf(void *buf, uint32_t len, bool isRamData = true);
private:
  /**
   * @brief 读数据
   * @param reg  可以是寄存器或命令
   * @param regBytes  reg的有效数据长度
   * @param pBuf  接收数组
   * @param len  读取的字节数
   * @param regIscmd  说明reg是命令还是寄存器，true:命令，false:寄存器
   * @param valBytes  当regIscmd为true时，该数据无效，可填任意值，当regIscmd为false时，该值代表寄存器对应的数值的字节数
   */
  bool readBuf(void *reg, uint8_t regBytes, void *pBuf, uint32_t len, bool regIscmd, uint8_t valBytes = 0);
};

#endif