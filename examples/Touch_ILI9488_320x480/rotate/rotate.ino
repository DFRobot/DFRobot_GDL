/*!
 * @file rotate.ino
 * @brief 两根手指左旋转或右旋转 改变显示的方向
 * @n 本示例支持的主板有Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
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
#include "GrayscaleBitmap1.h"
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
   @brief Constructor  构造函数
   @param dc  SPI通信的命令/数据线引脚
   @param cs  SPI通信的片选引脚
   @param rst  屏的复位引脚
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
int8_t rotate =0;
void setup()
{
  
  Serial.begin(9600);
  //UI初始化
  ui.begin();
  //绘制图片
  screen.drawRGBBitmap(/*x=*/(screen.width()-100)/2,/*y=*/(screen.height()-100)/2,/*bitmap gImage_Bitmap=*/(const unsigned uint16_t*)gImage_GrayscaleBitmap,/*w=*/100,/*h=*/100);
}


void loop()
{

    //getGestures()：识别手势
    DFRobot_UI:: eGesture_t gesture = ui.getGestures();
	
   switch (gesture) {
	//顺时针旋转
    case ui.DWROTATE : {
      rotate++;
      if(rotate>3) rotate=0;
	  //设置屏幕显示方向
       screen.setRotation(rotate);
       screen.drawRGBBitmap(/*x=*/(screen.width()-100)/2,/*y=*/(screen.height()-100)/2,/*bitmap gImage_Bitmap=*/(const unsigned uint16_t*)gImage_GrayscaleBitmap,/*w=*/100,/*h=*/100);

      } break;
    //逆时针旋转
    case ui.DCWROTATE : {
      if(rotate<0) {rotate=3;}
      else{
        rotate--;
      }
       screen.setRotation(rotate);
       screen.drawRGBBitmap(/*x=*/(screen.width()-100)/2,/*y=*/(screen.height()-100)/2,/*bitmap gImage_Bitmap=*/(const unsigned uint16_t*)gImage_GrayscaleBitmap,/*w=*/100,/*h=*/100);
       
       
      } break;
    case ui.NONE: {
        return;
      }
  }


}