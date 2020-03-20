/*!
 * @file UI_keypad.ino
 * @brief 在屏幕上创建一个数字键盘控件
 * @n 用户可以点击键盘上面的数字然后会看到输出的结果显示在上面的文本框里面，在输入时需要点击
 * @n 文本框确保文本框被选中
 * @n 本示例支持的主板有Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
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
 * @param width 屏幕的宽度.
 * @param height 屏幕的高度.
 */
DFRobot_UI ui(&screen, &touch);

void setup()
{

  Serial.begin(9600);

  ui.begin();
  // 设置UI的主题，有两种主题可供选择 1.CLASSIC ，2.MODERN。
  ui.setTheme(DFRobot_UI::CLASSIC);
  
   //创建一个数字键盘
  DFRobot_UI::sObject_t &kp = ui.creatKeyPad();
  ui.draw(&kp);
}

void loop()
{
  // 刷新
  ui.refresh();

}