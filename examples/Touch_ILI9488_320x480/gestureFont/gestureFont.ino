/*!
 * @file gettureFont.ino
 * @brief 屏幕中央存在几个文字，可以通过两指手势中的放大，缩小，上滑，下滑来控制它
 * @n 本示例支持的主板有Arduino Uno, Leonardo,Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
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
   @brief Constructor  当触摸采用gt系列芯片时，可以调用此构造函数
*/
DFRobot_Touch_GT911 touch;

/**
   @brief Constructor  当屏采用硬件SPI通信，驱动IC是st7789，屏幕分辨率是240x320时，可以调用此构造函数
   @param dc  SPI通信的命令/数据线引脚
   @param cs  SPI通信的片选引脚
   @param rst  屏的复位引脚
*/
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
   @brief 构造函数
   @param gdl 屏幕对象
   @param touch 触摸对象
*/
DFRobot_UI ui(&screen, &touch);

uint8_t fontSize = 2; //字体的大小
uint16_t fontColor = 0xf800; //字体的颜色
uint8_t fontLen = 4 * fontSize * 8; //字符串宽度
char * font = "DFRC"; //字符串
uint8_t fontHeight = fontSize * 8;//字符串高度
uint16_t posx =  screen.width() / 2 ; //字符串起始位置的x坐标
uint16_t posy =  screen.height()  / 2;//字符串起始位置的y坐标

void setup()
{

  Serial.begin(9600);
  //UI初始化
  ui.begin();
  //绘制最初的字符串
  ui.drawString(posx - fontLen / 2-8, posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);


}


void loop()
{
  refresh();
//Serial.println(touch.scan());
}

void refresh() {
  DFRobot_UI:: eGesture_t gesture = ui.getGestures();
  /*if(gesture != ui.NONE){
    Serial.println(gesture);
    }                
  */
  switch (gesture) {
	//检测到手势为缩小手势
    case ui.SHRINK : {
        screen.fillRect(posx - fontLen / 2 -8,  posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        //fontSize减小
	     	fontSize--;
        if (fontSize <= 1) fontSize = 1;

        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2-8 ,  posy - fontHeight / 2,  font, fontColor , ui.bgColor, fontSize, 0);
      }; break;
	//检测到手势为放大手势
    case ui.MAGNIFY : {
        screen.fillRect(posx - fontLen / 2-8 ,  posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        //fontSize增大
		    fontSize++;
        if (fontSize >= 7) fontSize = 7;
        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2-8 , posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);
      } break;
    //检测到手势为两指向上滑动
    case ui.DUPGLIDE : {
        screen.fillRect(posx - fontLen / 2-8 ,   posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        posy -= 10 ;
        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2 - 8,  posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);
      } break;
	//检测到手势为两指向下滑动
    case ui.DDOWNGLIDE : {
        screen.fillRect(posx - fontLen / 2 - 8,  posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        posy += 10;
        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2 - 8,  posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);
      } break;
    case ui.NONE: {
        return;
      }
  }
}