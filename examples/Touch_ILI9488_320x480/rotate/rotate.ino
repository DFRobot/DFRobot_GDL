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
/*!
 * @file rotate.ino
 * @brief two fingers rotate left or right to change the display direction
 * @n The motherboards supported by this example are Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-6
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
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D3
#define TFT_CS  D4
#define TFT_RST D5
/*AVR系列主板*/  /* AVR series motherboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
   @brief Constructor  当触摸采用gt系列芯片时，可以调用此构造函数 When the touch uses the gt series chip, you can call this constructor
*/
DFRobot_Touch_GT911 touch;

/**
   @brief Constructor  当屏采用硬件SPI通信，驱动IC是st7789，屏幕分辨率是240x320时，可以调用此构造函数
   @param dc  SPI通信的命令/数据线引脚
   @param cs  SPI通信的片选引脚
   @param rst  屏的复位引脚
*/
/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is st7789, and the screen resolution is 240x320, this constructor can be called
   @param dc Command / data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
*/
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/ /* M0 motherboard DMA transfer */
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
   @brief 构造函数
   @param gdl 屏幕对象
   @param touch 触摸对象
*/
/**
   @brief constructor
   @param gdl screen object
   @param touch touch object
*/
DFRobot_UI ui(&screen, &touch);
int8_t rotate =0;
void setup()
{
  
  Serial.begin(9600);
  //UI initialization
  ui.begin();
  //Draw picture
  screen.drawRGBBitmap(/*x=*/(screen.width()-100)/2,/*y=*/(screen.height()-100)/2,/*bitmap gImage_Bitmap=*/(const unsigned uint16_t*)gImage_GrayscaleBitmap,/*w=*/100,/*h=*/100);
}


void loop()
{

    //getGestures()：识别手势  Recognize gestures
    DFRobot_UI:: eGesture_t gesture = ui.getGestures();
	
   switch (gesture) {
	//顺时针旋转 clockwise rotation
    case ui.DWROTATE : { 
      rotate++;
      if(rotate>3) rotate=0;
	  //设置屏幕显示方向  Set screen orientation
       screen.setRotation(rotate);
       screen.drawRGBBitmap(/*x=*/(screen.width()-100)/2,/*y=*/(screen.height()-100)/2,/*bitmap gImage_Bitmap=*/(const unsigned uint16_t*)gImage_GrayscaleBitmap,/*w=*/100,/*h=*/100);

      } break; 
    //逆时针旋转  Anticlockwise rotation
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