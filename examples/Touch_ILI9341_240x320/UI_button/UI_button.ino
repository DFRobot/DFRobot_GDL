/*!
 * @file UI_button.ino
 * @brief 在屏幕上创建一个按钮控件，用户可以自定义按钮的参数
 * @n 示例里面创建了三个按钮on,off,clr,按下on,off按钮会在文本框内显示，按下clr按钮会删除文本框的一个字符
 * @n 需要文本框显示时，需要点击文本框以使光标移到文本框内
 * @n 本示例支持的主板有Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
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
 * @brief Constructor  当触摸采用XPT2046芯片时，可以调用此构造函数
 * @param cs  SPI片选信号
 * @param rst  复位信号
 * @param irq  中断信号
 */
DFRobot_Touch_XPT2046 touch(TOUCH_CS);
 
/**
 * @brief Constructor  当屏采用硬件SPI通信，驱动IC是ILI9341，屏幕分辨率是240x320时，可以调用此构造函数
 * @param dc  SPI通信的命令/数据线引脚
 * @param cs  SPI通信的片选引脚
 * @param rst  屏的复位引脚
 */
 
DFRobot_ILI9341_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/
//DFRobot_ILI9341_240x320_DMA_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
 * @brief 构造函数
 * @param gdl 屏幕对象
 * @param touch 触摸对象
 * @param width 屏幕的宽度.
 * @param height 屏幕的高度.
 */
DFRobot_UI ui(&screen, &touch);
//三个按钮的回调函数
void btnCallback(DFRobot_UI::sButton_t &btn,DFRobot_UI::sTextBox_t &obj) {
   String text((char *)btn.text);
   if(text == "ON"){
    obj.setText("you have touch button on");
    }
   else if(text == "OFF"){
    obj.setText("you have touch button off");
    }
   else if(text == "clr"){
    obj.deleteChar();
    }
    
}

void setup()
{

  Serial.begin(9600);
  //UI初始化
  ui.begin();
  // 设置UI的主题，有两种主题可供选择 1.CLASSIC ，2.MODERN。
  ui.setTheme(DFRobot_UI::MODERN);
  //创建一个文本框控件
  DFRobot_UI::sTextBox_t & tb = ui.creatText();
  tb.bgColor = 0xe6B6;
  ui.draw(&tb,/**x=*/5,/**y=*/5,/*width*/screen.width()-10,/*height*/screen.height()/5);
  //在屏幕上创建一个按钮控件
  DFRobot_UI::sButton_t & btn1 = ui.creatButton();
  //设置按钮的名字
  btn1.setText("ON");
  btn1.bgColor = COLOR_RGB565_RED;
  btn1.setCallback(btnCallback);
  //每个按钮都有一个文本框的参数，需要自己设定
  btn1.setOutput(&tb);
  ui.draw(&btn1,/**x=*/screen.width()/10,/**y=*/screen.height()/2,/*width*/screen.width()/10*2,/*height*/screen.width()/10*2);
  
  DFRobot_UI::sButton_t & btn2 = ui.creatButton();
  btn2.setText("OFF");
  btn2.bgColor = COLOR_RGB565_GREEN;
  btn2.setCallback(btnCallback);
  //每个按钮都有一个文本框的参数，需要自己设定
  btn2.setOutput(&tb);
  ui.draw(&btn2,/**x=*/(screen.width()/10)*4,/**y=*/screen.height()/2,/*width*/screen.width()/10*2,/*height*/screen.width()/10*2);
 
  DFRobot_UI::sButton_t & btn3 = ui.creatButton();
  btn3.bgColor = COLOR_RGB565_BLUE;
  btn3.setText("clr");

  //设置按钮的回调函数
  btn3.setCallback(btnCallback);
  //每个按钮都有一个文本框的参数，需要自己设定
  
  btn3.setOutput(&tb);
  ui.draw(&btn3,/**x=*/(screen.width()/10)*7,/**y=*/screen.height()/2,/*width*/screen.width()/10*2,/*height*/screen.width()/10*2);
}
void loop()
{
  //刷新所有控件
  ui.refresh();
}