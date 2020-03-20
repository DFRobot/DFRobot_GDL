/*!
 * @file UI_gesture.ino
 * @brief 在屏幕指定区域可以识别到用户所使用的手势，手势的名称会显示到文本框内
 * @n 本示例支持的主板有Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @n 需要文本框显示时，需要点击文本框以使光标移到文本框内
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



void setup()
{
  
  Serial.begin(9600);
  ui.begin();
  // 设置UI的主题，有两种主题可供选择 1.CLASSIC ，2.MODERN。
  ui.setTheme(DFRobot_UI::MODERN);
  
  //创建一个文本框控件
  DFRobot_UI::sTextBox_t &tb = ui.creatText();
  //在屏幕上创建一个文本框控件，根据自定义或初始化的参数绘制文本框
  ui.draw(&tb);
  /**
   * @brief 设置触摸的手势识别区域
   */
  ui.setGestureArea(/*x=*/screen.width()/2-100,/*y=*/200,/*width=*/150,/*height=*/200);
  while(true){
     //刷新
    ui.refresh();
    // getGestures()： 获取手势
    switch(ui.getGestures()){
      //setText：使文本框显示字符串
      case ui.SUPGLIDE : tb.setText("upwards slide"); break;
      case ui.SDOWNGLIDE : tb.setText("down slide"); break;
      case ui.SLEFTGLIDE : tb.setText("left slide"); break;
      case ui.SRIGHTGLIDE : tb.setText("right slide"); break;
      case ui.DLONGPRESSED : tb.setText("long press"); break;
      case ui.SCLICK : tb.setText("click"); break;
      case ui.DDOUBLECLICK : tb.setText("double click"); break;
      default  :  break;
      }
  }
}


void loop()
{


}