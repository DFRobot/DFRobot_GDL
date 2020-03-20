/*!
 * @file UI_bar.ino
 * @brief 在屏幕上创建一个进度条控件，用户可以自定义进度条的参数，也可以使用默认的参数
 * @n 用户可以通过进度条的回调函数控制进度条的值
 * @n 本示例支持的主板有Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
*/


#include "DFRobot_UI.h"
#include "Arduino.h"
#include "DFRobot_GDL.h"
#include "DFRobot_Touch.h"

/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
#define TOUCH_CS A3
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D3
#define TFT_CS  D4
#define TFT_RST D5
#define TOUCH_CS D6
/*AVR系列主板*/
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TOUCH_CS 5
#endif

/**
 * @brief Constructor  当触摸采用Gt911芯片时，可以调用此构造函数
 */
DFRobot_Touch_GT911 touch;
 
/**
 * @brief Constructor  当屏采用硬件SPI通信，驱动IC是ILI9488，屏幕分辨率是320x480时，可以调用此构造函数
 * @param dc  SPI通信的命令/数据线引脚
 * @param cs  SPI通信的片选引脚
 * @param rst  屏的复位引脚
 */
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
 


/**
 * @brief 构造函数
 * @param gdl 屏幕对象
 * @param touch 触摸对象
 */
DFRobot_UI ui(&screen, &touch);

uint8_t value1 = 0;
uint8_t value2 = 0;
uint8_t value3 = 0;
//进度条bar1的回调函数
void barCallback1(DFRobot_UI:: sBar_t &obj){
    //自动让进度条值每次+1;
   delay(50);
    obj.setValue(value1);
	if(value1 < 100) value1++;
}
//进度条bar2的回调函数
void barCallback2(DFRobot_UI:: sBar_t &obj){
    //自动让进度条值每次+1;
    delay(50);
    obj.setValue(value2);
	if(value2 < 100) value2++;
	
}
//进度条bar3的回调函数
void barCallback3(DFRobot_UI:: sBar_t &obj){
    //自动让进度条值每次+1;
    delay(50);
    obj.setValue(value3);
	if(value3 < 100) value3++;
}
void setup()
{
  
  Serial.begin(9600);
  //ui初始化
  ui.begin();
  ui.setTheme(DFRobot_UI::MODERN);

  //在屏幕上显示字符串
  ui.drawString(/*x=*/33,/*y=*/screen.height()/5*4,"Page of loading",COLOR_RGB565_WHITE,ui.bgColor,/*fontsize =*/2,/*Invert=*/0);
  //创建一个进度条控件
  DFRobot_UI::sBar_t &bar1 = ui.creatBar();
  /**用户自定义进度条参数*/
  bar1.setStyle(DFRobot_UI::COLUMN);
  bar1.fgColor = COLOR_RGB565_GREEN;
  bar1.setCallback(barCallback1);
  ui.draw(&bar1,/*x=*/33,/*y=*/screen.height()/5*3);
  
  DFRobot_UI::sBar_t &bar2 = ui.creatBar();
  /**用户自定义进度条参数*/
  bar2.setStyle(DFRobot_UI::CIRCULAR);
  bar2.setCallback(barCallback2);
  ui.draw(&bar2,/*x=*/120,/*y=*/screen.height()/5*2);

  DFRobot_UI::sBar_t &bar3 = ui.creatBar();
  /**用户自定义进度条参数*/
  bar3.fgColor = COLOR_RGB565_BLUE;
  bar3.setStyle(DFRobot_UI::BAR);
  bar3.setCallback(barCallback3);
  ui.draw(&bar3,/*x=*/(screen.width()-bar3.width)/2,/*y=*/screen.height()/5*1);
}


void loop()
{
  //刷新所有控件
  ui.refresh();
}