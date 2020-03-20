/*!
 * @file font.ino
 * @brief 演示不同自带英文字库效果 
 * @n 本示例支持的主板有Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2020-01-07
 * @url https://github.com/DFRobot/DFRobot_GDL
 */

#include "DFRobot_GDL.h"
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D3
#define TFT_CS  D4
#define TFT_RST D5
/*AVR系列主板*/
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
 * @brief Constructor  硬件SPI通信的构造函数
 * @param dc  SPI通信的命令/数据线引脚
 * @param cs  SPI通信的片选引脚
 * @param rst  屏的复位引脚
 */
DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);



/*
 *可供用户选择的宏定义颜色
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  screen.begin();
}

void loop() {
  //设置字体字号为4 字号范围1-4
  screen.setTextSize(2);
  /*设置屏幕颜色*/
  screen.fillScreen(COLOR_RGB565_BLACK);
  /*
   *当前可使用的字体如下，可在gfxfont.h中添加其他字体文件(字体文件存放在src/Frame/Fonts文件夹中)
   *FreeMono9pt7b, FreeMono12pt7b, FreeMonoBold12pt7b,
   *FreeMonoBoldOblique12pt7b, FreeMonoOblique12pt7b,
   *FreeSans12pt7b,FreeSansBold12pt7b, FreeSansBoldOblique12pt7b,
   *FreeSansOblique12pt7b, FreeSerif12pt7b, FreeSerifBold12pt7b,
   *FreeSerifBoldItalic12pt7b, FreeSerifItalic12pt7b, FreeMono24pt7b
   */
  screen.setFont(&FreeMono12pt7b);//设置字体为FreeMono12pt7b
  
  /*
   *@brief 设置文本位置
   *@param x 文本第一个字横坐标
   *@param y 文本第一个字纵坐标
   */
  screen.setCursor(/*x=*/10,/*y=*/120);
  //设置文本颜色
  //可选颜色列表和fillScreen函数中使用的颜色列表相同
  screen.setTextColor(COLOR_RGB565_LGRAY);
  //设置文本自动换行模式
  //true=文本自动换行，false=不自动换行
  screen.setTextWrap(true);
  // 输出文本
  screen.print("DFRobot");
  delay(500);
  
  //使用FreeMonoBold12pt7b字体
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&FreeMonoBold12pt7b);
  screen.setCursor(10,120);
  screen.setTextColor(COLOR_RGB565_GREEN);
  screen.setTextWrap(true);
  screen.print("GDL");
  delay(500);
  
  //使用FreeMonoBoldOblique12pt7b字体
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&FreeMonoBoldOblique12pt7b);
  screen.setCursor(10,160);
  screen.setTextColor(COLOR_RGB565_RED);
  screen.setTextWrap(true);
  screen.print("fonts test");
  delay(500);
  
  //使用FreeMonoOblique12pt7b字体
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&FreeMonoOblique12pt7b);
  screen.setCursor(80,160);
  screen.setTextColor(COLOR_RGB565_BLUE);
  screen.setTextWrap(true);
  screen.print("hello,world!");
  delay(500);
}