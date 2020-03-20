/*!
 * @file DFRobot_GDL.h
 * @brief 声明DFRobot_GDL类，并为其声明多个屏子类
 * @n DFRobot_GDL显示框架继承自Adafruit的GFX框架，链接:https://github.com/adafruit/Adafruit-GFX-Library
 * @n 支持UNO、Mega2560、Leonardo、ESP32、ESP8266、M0等主控
 * @n 支持硬件IIC、SPI、SPI_DMA接口
 * @n 支持彩屏和黑白屏
 * @n 不同屏之间参考u8glib的命名方式，通过驱动IC/分辨率/通信接口来区分
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @https://github.com/DFRobot/DFRobot_GDL
 */

#ifndef __DFROBOT_GDL_H
#define __DFROBOT_GDL_H

#include "DFRobot_Type.h"
#include <Arduino.h>
#include "Print.h"
#include "Frame/Adafruit_GFX.h"
#include "Interface/DFRobot_IF.h"
#include "Drivers/DFRobot_LCDType.h"
#include "Fonts/gdlFontDef.h"

#define COLOR_MODE_SINGLE  0
#define COLOR_MODE_RGB111  1
#define COLOR_MODE_RGB565  2
#define COLOR_MODE_RGB666  3
#define COLOR_MODE_RGB888  4

#if 1
#ifdef ARDUINO_SAM_ZERO//M0板子的串口与其他串口使用方式有区别
#define DBG(...) {SerialUSB.print("["); SerialUSB.print(__FUNCTION__); SerialUSB.print("(): "); SerialUSB.print(__LINE__); SerialUSB.print(" ] "); SerialUSB.println(__VA_ARGS__);}
#else
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#endif
#else
#define DBG(...)
#endif


/*
 0x36 寄存器的MADCTL：
   * -------------------------------------------------------------------------
   * |   D7   |   D6   |   D5   |   D4   |   D3   |   D2   |   D1   |   D0   |
   * -------------------------------------------------------------------------
   * |   MY   |   MX   |   MV   |   ML   |   RGB  |   MH   |      reserve    |
   * -------------------------------------------------------------------------
   *默认值0x00,显示方式从上到下，从左到右，RGB顺序
*/
typedef union{
  struct{
      uint8_t rsrv: 2; /**< reserve */
      uint8_t mh: 1;   /**< Display Data Latch Order */
      uint8_t rgb: 1;  /**< RGB/BGR Order 0: RGB, 1: BGR*/
      uint8_t ml: 1;   /**< Line Address Order */
      uint8_t mv: 1;   /**< Page/Column Order */
      uint8_t mx: 1;   /**< Column Address Order */
      uint8_t my: 1;   /**< Page Address Order */
  };
  uint8_t value; /**<寄存器MADCTL的值*/
}uMadctlArgs_t;

typedef struct{
  uint8_t madctl; /**<寄存器的MADCTL命令*/
  uMadctlArgs_t args; /**<寄存器MADCTL的值*/
}sMadctlCmd_t;

typedef struct{
  uint8_t rotation;/**<旋转角度 0：不旋转，1：旋转90°，2：旋转180°，3：旋转270°*/
  uint8_t cMode; /**<屏的颜色格式，如RGB565，RGB666，RGB888*/
  uint8_t *buffer;/**<一般用在黑白或灰阶等显示，及一个字节控制几个像素点或一个像素点由非整数个字节控制，且无法支持读模式*/
}sGdlLcdDev_t;

class DFRobot_GDL : public Adafruit_GFX, public DFRobot_IF{
public:
  /**
   * @brief Constructor  当屏采用硬件SPI通信时，可以调用此构造函数
   * @param dev  通信接口结构体指针，该结构体保存了屏的通信接口类型、通信频率、相关IO引脚，在不同的主控上，一次通信处理的最大字节数，及屏初始化数组和接口函数指针
   * @param w  屏的宽分辨率
   * @param h  屏的高分辨率
   * @param dc  SPI通信的命令/数据线引脚
   * @param cs  SPI通信的片选引脚
   * @param rst  屏的复位引脚
   * @param bl  屏的背光控制引脚
   */
  DFRobot_GDL(sGdlIFDev_t *dev, int16_t w, int16_t h, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl);
  /**
   * @brief Constructor  当屏采用硬件IIC通信时，可以调用此构造函数
   * @param dev  通信接口结构体指针，该结构体保存了屏的通信接口类型、通信频率、相关IO引脚，在不同的主控上，一次通信处理的最大字节数，及屏初始化数组和接口函数指针
   * @param w  屏的宽分辨率
   * @param h  屏的高分辨率
   * @param addr  IIC通信地址
   * @param rst  屏的复位引脚
   * @param bl  屏的背光控制引脚
   */
  DFRobot_GDL(sGdlIFDev_t *dev, int16_t w, int16_t h, uint8_t addr, uint8_t rst, uint8_t bl);
  ~DFRobot_GDL();
  virtual void begin(uint32_t freq = 0){};
  /**
   * @brief 画像素点函数
   * @param x  像素点的x坐标位置
   * @param y  像素点的y坐标位置
   * @param color  像素点的颜色，该颜色为RGB565格式
   */
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  /**
   * @brief 刷屏函数
   * @param color  屏幕填充颜色，默认为RGB565格式
   */
  void fillScreen(uint16_t color);
  /**
   * @brief 快速垂直画线
   * @param x  垂直线的起始点的x坐标位置
   * @param y  垂直线的起始点的y坐标位置
   * @param h  垂直线的高度
   * @param color  垂直线的颜色，该颜色为RGB565格式
   */
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  /**
   * @brief 快速水平画线
   * @param x  水平线的起始点的x坐标位置
   * @param y  水平线的起始点的y坐标位置
   * @param w  水平线的长度
   * @param color  垂直线的颜色，该颜色为RGB565格式
   */
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  /**
   * @brief 矩形填充
   * @param x  矩形的起始点的x坐标位置
   * @param y  矩形的起始点的y坐标位置
   * @param w  矩形的宽度
   * @param h  矩形的高度
   * @param color  矩形的颜色，该颜色为RGB565格式
   */
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  /**
   * @brief 设置字体
   * @param font  字体结构体指针，指向GFXfont或gdl_Font_t
   */
  void setFont(const void *font = NULL);
  /**
   * @brief 设置旋转方式
   * @param r  旋转方式 0,1,2,3有效
   */
  void setRotation(uint8_t r);
  
  /**
   * @brief  Invert the colors of the display (if supported by hardware).
   * @n        Self-contained, no transaction setup required.
   * @param  i  true = inverted display, false = normal display.
   */
  void invertDisplay(bool i);
  /**
   * @brief 将RGB565格式的颜色数据伸缩为RGB666格式的颜色数据
   * @param rgb666  存放的RGB666的颜色数据指针数组
   * @param color  原始RGB565格式颜色数据
   */
  uint8_t rgb565ToRGB666(uint8_t *rgb666, uint16_t color);
  /**
   * @brief 将RGB565格式的颜色数据伸缩为RGB888格式的颜色数据,24位真彩色
   * @param r  RGB888的红色分量
   * @param g  RGB888的绿色分量
   * @param b  RGB888的蓝色分量
   * @param color  原始RGB565格式颜色数据
   */
  void rgb565ToRGB888(uint8_t *r, uint8_t *g, uint8_t *b, uint16_t color);
  /**
   * @brief 将RGB888格式的颜色数据压缩为RGB666格式的颜色数据
   * @param r  原始颜色数据RGB888格式的红色分量
   * @param g  原始颜色数据RGB888格式的绿色分量
   * @param b  原始颜色数据RGB888格式的蓝色分量
   * @return  RGB565格式颜色数据
   */
  uint16_t rgb888ToRGB565(uint8_t r, uint8_t g, uint8_t b);
  /**
   * @brief Write  bytes into screen.
   * @n The following are the overload functions of the byte of different data type. 
   * @param buffer  存放发送数据的buffer
   * @param size  发送的字节数
   * @return 返回实际写入的字节数
   */
  virtual size_t write(const uint8_t *buffer, size_t size);
  /**
   * @brief 将utf-8编码转为Unicode编码
   * @param num  数据UTF-8编码的字节数
   * @param buf  数据的UTF-8编码
   * @return  32位的Unicode编码数据
   */
  uint32_t utf8ToUnicode(uint8_t num, uint8_t *buf);
  /**
   * @brief 获取某个数据UTF-8编码的字节数
   * @param b  数据UTF-8编码的第一个数据
   * @return  数据UTF-8编码的字节数
   */
  uint8_t getUtf_8Bytes(uint8_t b);
  /**
   * @brief 自定义字体的画位图方式
   * @param x  字符在屏上显示的x起始坐标
   * @param y  字符在屏上显示的y起始坐标
   * @param gdlFont 自定义字体结构体指针，存放字符的数据信息
   * @param fg  前景色，文本颜色
   * @param bg  背景色，文本底色
   */
  void drawCharBitmaps(uint16_t x, uint16_t y, gdl_Font_t *gdlFont, uint16_t fg, uint16_t bg);
  /**
   * @brief 使用oled屏时，操作完成后，必须调用update函数才会生效
   */
  void update();
  
protected:
  /**
   * @brief 显示屏初始化函数,初始化完毕后，即可显示
   * @param freq  频率
   */
  void initDisplay();
  /**
   * @brief 显示屏初始化函数
   * @param freq  频率
   */
  void gdlInit(uint32_t freq = 0);
  /**
   * @brief 纯虚函数，设置矩形显示区域，该函数有子类实例化
   * @param x  起始行的位置
   * @param y  起始列的位置
   * @param w  显示区域宽度
   * @param h  显示区域高度
   * @param color  像素点的颜色，该颜色为RGB565格式
   */
  virtual void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) = 0;
  /**
   * @brief 写命令
   * @param cmd  该数据是命令
   */
  void sendCommand(uint8_t cmd);
  /**
   * @brief 写命令和数据
   * @param cmd  该数据是命令
   * @param args  const指针，存放的是数据
   * @param len  指针数据的长度
   * @param isRamData  const指针是指向ROM还是RAM, 默认指向ROM,需用特殊的方法读取args指向的数据
   */
  void sendCommand(uint8_t cmd, void *args, uint32_t len, bool isRamData = false);
  /**
   * @brief 写一个字节数据
   * @param data  该参数是屏的数据
   */
  void sendData(uint8_t data);
  /**
   * @brief 写一个字节数据
   * @param data  该参数是屏的数据
   */
  void sendData16(uint16_t data);
  /**
   * @brief 如果屏是RGB565格式的，可以直接调用此函数
   * @param color  RGB565格式的颜色数据
   * @param len  要操作的像素点的个数
   */
  void sendColor(uint16_t color, uint32_t len);
  /**
   * @brief 给像素点设置颜色
   * @param c uint8_t指针，指向的是颜色数据，该颜色格式可能是RGB565、RGB666等，不同颜色格式的数据
   * @param cBytes  表示一个像素点用几个字节表示
   * @param isRamData  指针c是指向ROM还是RAM, 默认指向ROM,需用特殊的方法读取args指向的数据
   */
  void sendColor(uint8_t *c, uint8_t cBytes, uint32_t len, bool isRamData = true);
  /**
   * @brief 设置显示驱动IC的分辨率
   * @param w 驱动IC的x分辨率，当rotation = 0时的分辨率
   * @param h 驱动IC的y分辨率，当rotation = 0时的分辨率
   */
  void setDriverICResolution(int16_t w, int16_t h);
  /**
   * @brief 将16位RGB565格式颜色数据，切换成其他颜色格式，如RGB888，黑白格式等
   * @param pBuf 里面存放的是切换后的颜色数据
   * @param len pBuf里颜色数据的长度，代表颜色数据可以用几个byte表示
   * @param pixel pBuf里的颜色数据可控制的像素点的个数
   * @param color 原始颜色数据，为RGB565
   */
  void getColorFormat(uint8_t *pBuf, uint8_t &len, uint8_t &pixel, uint16_t color);

protected:
  uint8_t _xStart;
  uint8_t _yStart;
  int16_t _icWidth;
  int16_t _icHeight;
  uint8_t invertOnCmd;
  uint8_t invertOffCmd;
  sGdlLcdDev_t _lcd;
  sMadctlCmd_t madctlReg;

private:
  gdl_Font_t *_gdlFont;
  uint8_t _fontType;
};

/**
 * @brief 1.54寸SPI彩屏
 * 驱动IC: ST7789
 * 分辨率: 240x240（宽x高）
 * 通信接口：硬件SPI
 */
class DFRobot_ST7789_240x240_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x240_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x240_HW_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};


/**
 * @brief 1.54寸SPI彩屏
 * 驱动IC: ST7789
 * 分辨率: 240x320（宽x高）
 * 通信接口：硬件SPI
 */
class DFRobot_ST7789_240x320_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x320_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x320_HW_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

class DFRobot_ST7735S_80x160_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7735S_80x160_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7735S_80x160_HW_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ILI9488_320x480_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9488_320x480_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9488_320x480_HW_SPI();
  void begin(uint32_t freq = 0);
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ILI9341_240x320_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9341_240x320_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9341_240x320_HW_SPI();
  void begin(uint32_t freq = 0);
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

/**
 * @brief 0.91寸oled屏（黑白）
 * 驱动IC: SSD1306
 * 分辨率: 128x32（宽x高）
 * 通信接口：硬件IIC
 * IIC通信地址: 默认0x3C
 */
class DFRobot_SSD1306_128x32_HW_IIC: public DFRobot_GDL{
public:
  DFRobot_SSD1306_128x32_HW_IIC(uint8_t addr = 0x3C, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_SSD1306_128x32_HW_IIC();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

#ifdef ARDUINO_SAM_ZERO
class DFRobot_ST7789_240x240_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x240_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x240_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ST7735S_80x160_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7735S_80x160_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7735S_80x160_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ST7789_240x320_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x320_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x320_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ILI9488_320x480_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9488_320x480_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9488_320x480_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

class DFRobot_ILI9341_240x320_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9341_240x320_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9341_240x320_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
#endif
#endif