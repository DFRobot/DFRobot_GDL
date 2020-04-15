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

/*!
 * @file DFRobot_IF.h
 * @brief Declaration the basic structure of class DFRobot_IF, compatible with reading and writing of different interfaces
 * @n supports hardware IIC
 * @n supports hardware SPI
 * @n supports DMA of M0
 * @n supports reading data from rom / ram
 * @n supports SPI / IIC / SPI_DMA read and write
 * @n Set communication interface frequency
 *
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [Arya] (xue.peng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-23
 * @https: //github.com/DFRobot/DFRobot_GDL
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


/*硬件IIC一次能传输的最大字节数*/  /* Maximum number of bytes that hardware IIC can transfer at one time */
#ifdef I2C_BUFFER_LENGTH
#undef I2C_BUFFER_LENGTH
#endif
#if defined(__AVR__)||defined(ESP8266) //#ifdef ARDUINO_ARCH_AVR  //AVR系列单片机一次最多传32字节数据 //AVR series MCU can transfer up to 32 bytes of data at a time
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
定义设备类型，例：屏和触摸  Define device type, for example: screen and touch
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
 
/**
 * @brief Communication interface function command.
 * @param IF_COM_PROTOCOL_INIT: Communication protocol initialization.
 * @param IF_COM_SET_FREQUENCY: Set the communication frequency.
 * @param IF_COM_WRITE_CMD: write command.
 * @param IF_COM_READ_DATA: write data
 * @param IF_COM_WRITE_FLASH_FIXED: The data stored in the ROM is transmitted, and the data position remains unchanged during the transmission, and the same data is transmitted multiple times
 * @param IF_COM_WRITE_FLASH_INC: transfer the data stored in ROM, and the data address increases sequentially
 * @param IF_COM_WRITE_RAM_FIXED: The data stored in RAM is transferred, and the data position remains unchanged during the transfer, and the same data is transferred multiple times
 * @param IF_COM_WRITE_RAM_INC: transfer the data saved in RMA, and the data address increases in sequence
 */
#define IF_COM_PROTOCOL_INIT  0
#define IF_COM_SET_FREQUENCY     1
#define IF_COM_WRITE_CMD      2
#define IF_COM_READ_DATA      3
#define IF_COM_WRITE_FLASH_FIXED 4
#define IF_COM_WRITE_FLASH_INC   5
#define IF_COM_WRITE_RAM_FIXED   6
#define IF_COM_WRITE_RAM_INC     7

/*无用引脚，空引脚*/ /* Useless pins, empty pins */
#define GDL_PIN_NC     0xFF 
/*IO口操纵方式*/  /* IO port operation mode */
#define PIN_IN(pin)   if(pin != GDL_PIN_NC) pinMode(pin, INPUT)
#define PIN_OUT(pin)  if(pin != GDL_PIN_NC) pinMode(pin, OUTPUT)
#define PIN_HIGH(pin)  if(pin != GDL_PIN_NC) digitalWrite(pin, HIGH)
#define PIN_LOW(pin)  if(pin != GDL_PIN_NC) digitalWrite(pin, LOW)


#define IF_HW_IIC 3  //3代表硬件IIC的最大参数个数，即(rst pin)，(bl pin),addr   3 represents the maximum number of hardware IIC parameters, namely (rst pin), (bl pin), addr
#define IF_SW_IIC 5  //5代表软件IIC的最大参数个数，即(rst),(bl),addr,scl,sda,   5 represents the maximum number of parameters of the software IIC, namely (rst), (bl), addr, scl, sda,
#define IF_HW_SPI 4  //4代表硬件SPI的最大参数个数，即(rst),(bl),dc,(cs)         4 represents the maximum number of hardware SPI parameters, namely (rst), (bl), dc, (cs)
#define IF_SW_SPI 7  //7代表硬件SPI的最大参数个数，即(rst),(bl),dc,(cs),sck,mosi,(miso)  7 represents the maximum number of hardware SPI parameters, namely (rst), (bl), dc, (cs), sck, mosi, (miso)


#define IF_PIN_RST    0 //复位  Reset
#define IF_PIN_BL     1 //背光  Backlight
#define IF_PIN_IRQ    1 //中断  Interrupt
#define IF_PIN_ADDR   2 //IIC地址  IIC address
#define IF_PIN_DC     2 //SPI命令数据控制引脚 SPI command data control pin
#define IF_PIN_CS     3 //SPI片选引脚  SPI chip select pin
 
typedef struct sGdlIF sGdlIF_t;
typedef uint8_t(*devInterfaceFunctionPtr)(sGdlIF_t *gdl, uint8_t step, uint8_t *addr, uint32_t len);//FP->function pointer函数指针

typedef struct{
  uint8_t devName; /**<记录设备类型，DEV_TYPE_TOUCH代表触摸，DEV_TYPE_SCREEN代表屏*/  /**<Record the device type, DEV_TYPE_TOUCH represents touch, DEV_TYPE_SCREEN represents screen*/
  uint8_t *addr;  /**<记录设备的初始化数组*/  /**<Record the device's initialization array*/
  devInterfaceFunctionPtr talk;/**<注册屏的通信接口函数，指向函数的指针*/  /**<Communication interface function of the registration screen, pointer to the function*/
}sGdlIFDev_t;

typedef union{
  TwoWire *iic;/**<定义一个IIC类指针*/  /**<Define an IIC pointer*/
  SPIClass *spi;/**<定义一个SPI类指针*/  /**<Define an SPI class pointer*/
#ifdef ARDUINO_SAM_ZERO
  DFRobot_DMA_SPI *dma;/**<定义一个dma类指针*/  /**<Define a dma class pointer*/
#endif
  void *interface;/*公共接口*/  /**<Public interface*/
}uProtocol_t;

struct sGdlIF{
  uint8_t interface;/**<存储屏的显示通信类型*/  /**<Display communication type of storage screen*/
  uint32_t freq;/**<通信频率*/  /**<Communication frequency*/
  uProtocol_t pro;/**<通信协议：IIC，并口，SPI，DMA等等*/  /**<Communication protocol: IIC, parallel port, SPI, DMA, etc.*/
  bool  isBegin;/**<判断初始化是否成功*/  /**<Determine whether the initialization is successful*/
  uint8_t *pinList;/**<保存通信接口的引脚数据，它的长度由某种通信接口的最大引脚数决定*/  /**<Save the pin data of the communication interface, its length is determined by the maximum number of pins of a certain communication interface*/
  uint16_t length;/**<通信的最大字节数，默认0xFFFF*/  /**<Maximum number of bytes for communication, default 0xFFFF*/
  sGdlIFDev_t *dev;/**<记录设备（屏或触摸）的初始化数组和通信接口*/  /**<Initialization array and communication interface of recording device (screen or touch)*/
}__attribute__ ((packed));


#define GDL_IF_PB_DEV(name, type, init, interface) \
sGdlIFDev_t name = {type, init, interface};

/*定义不同接口的功能函数*/  /**<Define function functions of different interfaces*/
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
  /**
   * @brief Constructor Get the information of IIC interface
   * @param dev pointer of the communication interface structure, which saves the screen's communication interface type, communication frequency, and related IO pins. On different masters, the maximum number of bytes processed in one communication and the screen initialization array and Interface function pointer
   * @param addr IIC communication address
   * @param rst reset signal
   * @param irq interrupt signal
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
  /**
   * @brief Constructor Get the information of SPI interface
   * @param dev pointer of the communication interface structure, which saves the screen's communication interface type, communication frequency, and related IO pins. On different masters, the maximum number of bytes processed in one communication and the screen initialization array and Interface function pointer
   * @param dc Some devices use DC to distinguish between data and commands.
   * @param cs SPI chip select signal
   * @param rst reset signal
   * @param bl backlight or interrupt signal
   */
  DFRobot_IF(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/);
  ~DFRobot_IF();
  /**
   * @brief 通信接口初始化
   */
  /**
   * @brief communication interface initialization
   */
  void initInterface();
  /**
   * @brief 设置通信接口频率
   * @param freq  频率，单位Hz
   */
  /**
   * @brief Set communication interface frequency
   * @param freq frequency in Hz
   */
  void setFrequency(uint32_t freq);
protected:
  void initIFCommon();
  void initHWIIC(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t bl/*irq*/);
  void initHWSPI(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/);
  void writeData32(uint32_t data, bool isRamData);
  void writeBuf(uint16_t reg, void *pBuf, uint32_t len, bool flag = false);//flag代表数据和寄存器是否匹配  flag represents whether the data and the register match
  /**
   * @brief 读寄存器数据
   * @param reg  可以是寄存器
   * @param pBuf  接收数组
   * @param len  读取的字节数
   * @param flag  当reg代表寄存器时，flag为false，代表16位寄存器对应一个8位的数据，true代表对应16位数据
   */
  /**
   * @brief read register data
   * @param reg can be a register
   * @param pBuf receive array
   * @param len the number of bytes read
   * @param flag When reg represents a register, the flag is false, representing that the 16-bit register corresponds to an 8-bit data, and true represents the corresponding 16-bit data
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
  /**
   * @brief read data
   * @param reg can be a register or command
   * Effective data length of @param regBytes reg
   * @param pBuf receive array
   * @param len the number of bytes read
   * @param regIscmd indicates whether reg is a command or a register, true: command, false: register
   * @param valBytes When regIscmd is true, the data is invalid, any value can be filled, when regIscmd is false, the value represents the number of bytes corresponding to the value of the register
   */
  bool readBuf(void *reg, uint8_t regBytes, void *pBuf, uint32_t len, bool regIscmd, uint8_t valBytes = 0);
};

#endif