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
/*!
 * @file gettureFont.ino
 * @brief There are several words in the center of the screen, which can be controlled by zooming in, zooming out, sliding up, and sliding down in a two-finger gesture
 * @n The motherboards supported by this example are Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 *
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

uint8_t fontSize = 2; //字体的大小 Font size
uint16_t fontColor = 0xf800; //字体的颜色 Font color
uint8_t fontLen = 4 * fontSize * 8; //字符串宽度 String width
char * font = "DFRC"; //字符串 String
uint8_t fontHeight = fontSize * 8;//字符串高度  String height
uint16_t posx =  screen.width() / 2 ; //字符串起始位置的x坐标 The x coordinate of the starting position of the string
uint16_t posy =  screen.height()  / 2;//字符串起始位置的y坐标 The y coordinate of the beginning of the string

void setup()
{

  Serial.begin(9600);
  //UI initialization
  ui.begin();
  //绘制最初的字符串 Draw the initial string
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
	//检测到手势为缩小手势 Gesture detected as zoom out gesture
    case ui.SHRINK : {
        screen.fillRect(posx - fontLen / 2 -8,  posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        //fontSize减小  fontSize decreases
	     	fontSize--;
        if (fontSize <= 1) fontSize = 1;

        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2-8 ,  posy - fontHeight / 2,  font, fontColor , ui.bgColor, fontSize, 0);
      }; break;
	//检测到手势为放大手势 Gesture detected as zoom in gesture
    case ui.MAGNIFY : {
        screen.fillRect(posx - fontLen / 2-8 ,  posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        //fontSize增大  fontSize increase
		    fontSize++;
        if (fontSize >= 7) fontSize = 7;
        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2-8 , posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);
      } break;
    //检测到手势为两指向上滑动 Gesture detected swiping up with two fingers
    case ui.DUPGLIDE : {
        screen.fillRect(posx - fontLen / 2-8 ,   posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        posy -= 10 ;
        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2 - 8,  posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);
      } break;
	//检测到手势为两指向下滑动 Gesture detected swiping down with two fingers
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