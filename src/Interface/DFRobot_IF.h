/*!
 * @file DFRobot_IF.h
 * @brief Declaration the basic structure of class DFRobot_IF, compatible with reading and writing of different interfaces
 * @n Support hardware I2C
 * @n Support hardware SPI
 * @n Support DMA of M0
 * @n Support reading data from rom / ram
 * @n Support SPI / I2C / SPI_DMA read and write
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


/* Maximum number of bytes that hardware IIC can transfer at one time */
#ifdef I2C_BUFFER_LENGTH
#undef I2C_BUFFER_LENGTH
#endif
#if defined(__AVR__)||defined(ESP8266) //#ifdef ARDUINO_ARCH_AVR  //AVR series MCU can transfer up to 32 bytes of data at a time
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
Define device type, for example: screen and touch
********************************************************/
#define DEV_TYPE_TOUCH  0
#define DEV_TYPE_SCREEN 1
#define DEV_TYPE_NONE 255

 
/**
 * @brief Communication interface function command.
 * @param IF_COM_PROTOCOL_INIT: Communication protocol initialization.
 * @param IF_COM_SET_FREQUENCY: Set the communication frequency.
 * @param IF_COM_WRITE_CMD: write command.
 * @param IF_COM_READ_DATA: write data
 * @param IF_COM_WRITE_FLASH_FIXED: Transfer the data stored in the ROM, and the data position remains unchanged during the transmission, and the same data is transmitted multiple times
 * @param IF_COM_WRITE_FLASH_INC: Transfer the data stored in ROM, and the data address increases sequentially
 * @param IF_COM_WRITE_RAM_FIXED: Transfer the data stored in RAM, and the data position remains unchanged during the transmission, and the same data is transferred multiple times
 * @param IF_COM_WRITE_RAM_INC: Transfer the data saved in RMA, and the data address increases sequentially
 */
#define IF_COM_PROTOCOL_INIT  0
#define IF_COM_SET_FREQUENCY     1
#define IF_COM_WRITE_CMD      2
#define IF_COM_READ_DATA      3
#define IF_COM_WRITE_FLASH_FIXED 4
#define IF_COM_WRITE_FLASH_INC   5
#define IF_COM_WRITE_RAM_FIXED   6
#define IF_COM_WRITE_RAM_INC     7
#define IF_COM_PROTOCOL_INIT1     8

/* Useless pin, empty pin */
#define GDL_PIN_NC     0xFF 
/* IO port operation mode */
#define PIN_IN(pin)   if(pin != GDL_PIN_NC) pinMode(pin, INPUT)
#define PIN_OUT(pin)  if(pin != GDL_PIN_NC) pinMode(pin, OUTPUT)
#define PIN_HIGH(pin)  if(pin != GDL_PIN_NC) digitalWrite(pin, HIGH)
#define PIN_LOW(pin)  if(pin != GDL_PIN_NC) digitalWrite(pin, LOW)


#define IF_HW_IIC 3  //3 represents the maximum number of hardware I2C parameters, (rst pin), (bl pin), addr
#define IF_SW_IIC 5  //5 represents the maximum number of parameters of the software I2C, (rst), (bl), addr, scl, sda,
#define IF_HW_SPI 4  //4 represents the maximum number of hardware SPI parameters,  (rst), (bl), dc, (cs)
#define IF_SW_SPI 7  //7 represents the maximum number of hardware SPI parameters, (rst), (bl), dc, (cs), sck, mosi, (miso)


#define IF_PIN_RST    0 //Reset
#define IF_PIN_BL     1 //Backlight
#define IF_PIN_IRQ    1 //Interrupt
#define IF_PIN_ADDR   2 //I2C address
#define IF_PIN_DC     2 //SPI command data control pin
#define IF_PIN_CS     3 //SPI chip select pin
 
typedef struct sGdlIF sGdlIF_t;
typedef uint8_t(*devInterfaceFunctionPtr)(sGdlIF_t *gdl, uint8_t step, uint8_t *addr, uint32_t len);//FP->function pointer函数指针
typedef void* (*devInterfaceInit)();
/**
 * @struct sGdlIFDev_t
 */
typedef struct{
  uint8_t devName; /**<Record the device type, DEV_TYPE_TOUCH represents touch, DEV_TYPE_SCREEN represents screen*/
  uint8_t *addr;  /**<Record the device's initialization array*/
  devInterfaceFunctionPtr talk;/**<Register communication interface function of the screen, pointer to the function*/
}sGdlIFDev_t;

/**
 * @union uProtocol_t
 */
typedef union{
  TwoWire *iic;/**<Define an I2C pointer*/
  SPIClass *spi;/**<Define a SPI pointer*/
#ifdef ARDUINO_SAM_ZERO
  DFRobot_DMA_SPI *dma;/**<Define a dma pointer*/
#endif
  void *interface;/**<Public interface*/
}uProtocol_t;

/**
 * @struct sGdlIF
 */
struct sGdlIF{
  uint8_t interface;/**<Display communication type of storage screen*/
  uint32_t freq;/**<Communication frequency*/
  uProtocol_t pro;/**<Communication protocol: IIC, parallel port, SPI, DMA, etc.*/
  bool  isBegin;/**<Determine whether the initialization is successful*/
  uint8_t *pinList;/**<Save the pin data of the communication interface, its length is determined by the maximum number of pins of the related communication interface*/
  uint16_t length;/**<Maximum number of bytes for communication, default to 0xFFFF*/
  sGdlIFDev_t *dev;/**<Record initialization array and communication interface of device (screen or touch)*/
}__attribute__ ((packed));


#define GDL_IF_PB_DEV(name, type, init, interface) \
sGdlIFDev_t name = {(uint8_t)type, init, interface};

/**<Define functions of different interfaces*/
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
   * @fn DFRobot_IF
   * @brief Constructor Get the information of I2C interface
   * @param dev Pointer to communication interface structure. The sctruct holds the screen's communication interface type, 
   * @n communication frequency and related IO pins. On different main-controllers, the maximum number of bytes processed in
   * @n one communication and the screen initialization array and Interface function pointer are different.
   * @param addr IIC communication address
   * @param rst Reset signal
   * @param irq Interrupt signal
   */
  DFRobot_IF(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t bl/*irq*/, TwoWire *pwire = &Wire);
  /**
   * @fn DFRobot_IF
   * @brief Constructor Get the information of SPI interface
   * @param dev Pointer to communication interface structure. The struct holds the screen's communication interface type,
   * @n communication frequency, and related IO pins. On different main-controllers, the maximum number of bytes processed in
   * @n one communication and the screen initialization array and Interface function pointer are different.
   * @param dc Some devices use DC to distinguish between data and commands.
   * @param cs SPI chip select signal
   * @param rst Reset signal
   * @param bl Backlight or interrupt signal
   */
  DFRobot_IF(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/, SPIClass *pspi = &SPI);
  ~DFRobot_IF();
  /**
   * @fn initInterface
   * @brief communication interface initialization
   */
  void initInterface(devInterfaceInit fun = NULL);
  /**
   * @fn setFrequency
   * @brief Set communication interface frequency
   * @param freq frequency in Hz
   */
  void setFrequency(uint32_t freq);
protected:
  void initIFCommon();
  void initHWIIC(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t bl/*irq*/);
  void initHWSPI(sGdlIFDev_t *dev, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl/*irq*/);
  void writeData32(uint32_t data, bool isRamData);
  void writeBuf(uint16_t reg, void *pBuf, uint32_t len, bool flag = false);//flag represents whether the data match with the register
  /**
   * @fn readReg
   * @brief Read register data
   * @param reg can be a register
   * @param pBuf Receive array
   * @param len The number of bytes read
   * @param flag When reg represents a register, the flag is false, representing that the 16-bit register 
   * @n corresponds to an 8-bit data, and true represents that it corresponds to 16-bit data
   */
  void readReg(uint16_t reg, void *pBuf, uint32_t len, bool flag = false);
  void readCommand(uint8_t cmd, void *pBuf, uint32_t len);
  void sendBuf(void *buf, uint32_t len, bool isRamData = true);
private:
  /**
   * @fn readBuf
   * @brief Read data
   * @param reg can be a register or command
   * @param regBytes reg Effective data length of reg
   * @param pBuf Receive array
   * @param len The number of bytes read
   * @param regIscmd indicates whether reg is a command or a register, true: command, false: register
   * @param valBytes When regIscmd is true, the data is invalid, any value can be filled, when regIscmd is false, 
   * @n the value represents the number of bytes corresponding to the value of the register
   */
  bool readBuf(void *reg, uint8_t regBytes, void *pBuf, uint32_t len, bool regIscmd, uint8_t valBytes = 0);
};

#endif
