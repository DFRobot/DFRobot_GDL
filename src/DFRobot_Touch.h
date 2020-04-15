/*!
 * @file DFRobot_Touch.h
 * @brief Declaration the basic structure of class DFRobot_Touch
 * @n 触摸框架，可以直接添加SPI和IIC接口的触摸
 * @n 支持的电容触摸IC：GT911/GT5588
 * @n 支持的电阻触摸IC: XPT2046
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
  * @file DFRobot_Touch.h
  * @brief Declaration the basic structure of class DFRobot_Touch
  * @n touch frame, you can directly add touch of SPI and IIC interface
  * @n supports capacitive touch IC: GT911 / GT5588
  * @n Supported resistive touch IC: XPT2046
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

/*不同触摸IC，触摸设备*/ /* Different touch ICs, touch devices */
extern sGdlIFDev_t gdl_Dev_GTXXX_TOUCH_HW_IIC;
extern sGdlIFDev_t gdl_Dev_XPT2046_TOUCH_HW_SPI;

class DFRobot_Touch: public DFRobot_IF{
public:
  typedef struct{
      uint8_t id; /**<触摸点的id，即第几个触摸点*/ /**<id of touch point, which is the number of touch points */
      uint16_t x; /**<触摸点的x坐标*/ /**<X coordinate of touch point*/
      uint16_t y; /**<触摸点的y坐标*/ /**<Y coordinate of touch point*/
      uint8_t wSize;/**<触摸点尺寸：宽度*/ /**<Touch point size: width*/
      uint8_t hSize;/**<触摸点尺寸：高度*/ /**<Touch point size: height*/
  }sPoints_t;
  
  typedef struct{
      uint16_t xw;/**<触摸板的尺寸：宽度*/ /**<Touchpad size: width*/
      uint16_t yh;/**<触摸板的尺寸：高度*/ /**<Touchpad size: height*/
  }sResolution_t;
  
  sResolution_t _size;/**<触摸板尺寸结构体，用于存储触摸板的分辨率：宽x高*/ /**<Touchpad size structure for storing the resolution of the touchpad: width x height*/
  String _points;/**<字符串，用于存储触摸点的信息：ID,XY坐标，点尺寸，且点的信息以","隔开，点与点之间用空格隔开*/ /**<A character string used to store the information of the touched point: ID, XY coordinates, point size, and the point information is separated by ",", and the points are separated by spaces*/
  sPoints_t _point;/**一个点的数据*/ /**<One point of data*/
  uint8_t _pNum;
  /**
   * @brief Constructor  当触摸采用硬件IIC通信时，可以调用此构造函数
   * @param dev  通信接口结构体指针，该结构体保存了屏的通信接口类型、通信频率、相关IO引脚，在不同的主控上，一次通信处理的最大字节数，及屏初始化数组和接口函数指针
   * @param addr  IIC通信地址
   * @param rst  复位信号
   * @param irq  中断信号
   */
  /**
    * @brief Constructor When the touch uses hardware IIC communication, this constructor can be called
    * @param dev pointer of the communication interface structure, which saves the screen's communication interface type, communication frequency, and related IO pins. On different masters, the maximum number of bytes processed in one communication and the screen initialization array and Interface function pointer
    * @param addr IIC communication address
    * @param rst reset signal
    * @param irq interrupt signal
    */
  DFRobot_Touch(sGdlIFDev_t *dev, uint8_t addr, uint8_t rst, uint8_t irq);
  /**
   * @brief Constructor  当触摸采用硬件SPI通信时，可以调用此构造函数
   * @param dev  通信接口结构体指针，该结构体保存了屏的通信接口类型、通信频率、相关IO引脚，在不同的主控上，一次通信处理的最大字节数，及屏初始化数组和接口函数指针
   * @param cs  SPI片选信号
   * @param rst  复位信号
   * @param irq  中断信号
   * @param dc  无效值，写在这是为了区分IIC和SPI，为GDL_PIN_NC
   */
  /**
    * @brief Constructor When the touch uses hardware SPI communication, you can call this constructor
    * @param dev pointer of the communication interface structure, which saves the screen's communication interface type, communication frequency, and related IO pins. On different masters, the maximum number of bytes processed in one communication and the screen initialization array and Interface function pointer
    * @param cs SPI chip select signal
    * @param rst reset signal
    * @param irq interrupt signal
    * @param dc Invalid value, written here to distinguish between IIC and SPI, GDL_PIN_NC
    */
  DFRobot_Touch(sGdlIFDev_t *dev, uint8_t cs, uint8_t rst, uint8_t irq, uint8_t dc);
  ~DFRobot_Touch();
  /**
   * @brief 触摸初始化，包括接口初始化和配置初始化
   */
  /**
    * @brief touch initialization, including interface initialization and configuration initialization
    */
  void initTouch();
  virtual void begin(uint32_t freq = 0)=0;
  /**
   * @brief 触摸配置函数
   * @param addr 配置数组
   */
  /**
    * @brief touch configuration function
    * @param addr configuration array
    */
  void touchConfig(uint8_t *addr);
  /**
   * @brief 纯虚函数，扫描触摸点的函数，该函数要在子类中实现
   * @return String类型，存储了所有触摸点的id，坐标，尺寸等信息
   */
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
#endif