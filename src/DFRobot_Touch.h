 /*!
  * @file DFRobot_Touch.h
  * @brief Declaration the basic structure of class DFRobot_Touch
  * @n Touch frame, touch of SPI and IIC interface can be added directly
  * @n Supported capacitive touch IC: GT911/GT5588
  * @n Supported resistive touch IC: XPT2046
  * @n Support reading data from rom/ram
  * @n Support SPI/IIC/SPI_DMA read and write
  * @n Set communication interface frequency
  *
  * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
  * @licence The MIT License (MIT)
  * @author [Arya] (xue.peng@dfrobot.com)
  * @version V1.0
  * @date 2019-12-23
  * @https: //github.com/DFRobot/DFRobot_GDL
  */
#ifndef __DFROBOT_TOUCH_H
#define __DFROBOT_TOUCH_H
#include "Arduino.h"
#include "Wire.h"
#include "TouchDrivers/DFRobot_GT911.h"
#include "TouchDrivers/DFRobot_GT5688.h"
#include <DFRobot_Type.h>
#include "Interface/DFRobot_IF.h"
#define XPT2046_XFAC_320x480      920 //663
#define XPT2046_XOFFSET_320x480   (-20) //(-13)
#define XPT2046_YFAC_320x480      1300 //894
#define XPT2046_YOFFSET_320x480   (-30) 
#define XPT2046_XFAC_240x320      663 //663
#define XPT2046_XOFFSET_240x320   (-13) //(-13)
#define XPT2046_YFAC_240x320      894 //894
#define XPT2046_YOFFSET_240x320   (-30) 
#define GT911                     11
#define FT5436                    36
/* Different touch ICs, touch devices */
extern sGdlIFDev_t gdl_Dev_GTXXX_TOUCH_HW_IIC;
extern sGdlIFDev_t gdl_Dev_XPT2046_TOUCH_HW_SPI;

class DFRobot_Touch: public DFRobot_IF{
public:
  typedef struct{
      uint8_t id; /**<Touch point ID, which is the order number of a touch point */
      uint16_t x; /**<X-coordinate of touch point*/
      uint16_t y; /**<Y-coordinate of touch point*/
      uint8_t wSize;/**<Touch point size: width*/
      uint8_t hSize;/**<Touch point size: height*/
  }sPoints_t;
  
  typedef struct{
      uint16_t xw;/**<Touchpad size: width*/
      uint16_t yh;/**<Touchpad size: height*/
  }sResolution_t;
  
  sResolution_t _size;/**<Touchpad size struct for storing the resolution of the touchpad: width x height*/
  String _points;/**<String to store the information of touch point: ID, X/Y coordinates, point size, and the point information needs to be separated by ",", and the points are separated by spaces*/
  sPoints_t _point;/**<Data of one point*/
  uint8_t _pNum;
  /**
    * @brief Constructor When the touch uses hardware IIC communication, this constructor can be called
    * @param dev pointer of the communication interface structure. The struct holds the screen's communication interface type, 
    * @n communication frequency, and related IO pins. On different masters, the maximum number of bytes processed in one
    * @n communication and the screen initialization array and Interface function pointer are different.
    * @param addr IIC communication address
    * @param rst Reset signal
    * @param irq Interrupt signal
    */
  DFRobot_Touch(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t irq);
  /**
    * @brief Constructor When the touch uses hardware SPI communication, this constructor can be called.
    * @param dev pointer of the communication interface structure. The struct holds the screen's communication interface type,
    * @n communication frequency, and related IO pins. On different masters, the maximum number of bytes processed in one 
    * @n communication and the screen initialization array and Interface function pointer are different.
    * @param cs SPI chip select signal
    * @param rst Reset signal
    * @param irq Interrupt signal
    * @param dc Invalid value, written here to distinguish between IIC and SPI, GDL_PIN_NC
    */
  DFRobot_Touch(sGdlIFDev_t *dev, uint8_t cs, uint8_t rst, uint8_t irq, uint8_t dc);
  ~DFRobot_Touch();
  /**
    * @brief Init touch, including interface and configuration initialization
    */
  void initTouch();
  virtual void begin(uint32_t freq = 0)=0;
  /**
    * @brief Touch configuration function
    * @param addr Configuration array
    */
  void touchConfig(uint8_t *addr);
  /**
    * @brief Pure virtual function, the function of scanning touch points, this function should be implemented in subclasses
    * @return String type, stores the id, coordinates, size and other information of all touch points
    */
  virtual String scan() = 0;
};


class DFRobot_Touch_GT911: public DFRobot_Touch{
public:
  DFRobot_Touch_GT911(uint8_t addr = 0x5D, uint8_t rst = 255, uint8_t irq = 255);
  ~DFRobot_Touch_GT911();
  void begin(uint32_t freq = 0);
  String scan();
private:
  typedef struct{
      uint8_t id;
      uint8_t xl;
      uint8_t xh;
      uint8_t yl;
      uint8_t yh;
      uint8_t wSize;
      uint8_t hSize;
      uint8_t reserve;
  }sGtPoints_t;
  String gt911Scan();
  String ft5436Scan();
  uint8_t IC;
  sGtPoints_t _p[5];
  String id;
};

class DFRobot_Touch_XPT2046:public DFRobot_Touch{
public:
  DFRobot_Touch_XPT2046(uint8_t cs, uint8_t rst = GDL_PIN_NC, uint8_t irq = GDL_PIN_NC);
  ~DFRobot_Touch_XPT2046();
  void begin(uint32_t freq = 0);
  String scan();
  uint16_t readxy(uint8_t cmd);
private:
  
  String id;
};
class DFRobot_Touch_FT5436 : public DFRobot_Touch{
public:
  DFRobot_Touch_FT5436(uint8_t addr = 0x38, uint8_t rst = 255, uint8_t irq = 255);
  ~DFRobot_Touch_FT5436();
  void begin(uint32_t freq = 0);
  String scan();
private:

};
#endif
