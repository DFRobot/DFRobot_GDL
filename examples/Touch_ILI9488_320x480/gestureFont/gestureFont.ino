/*!
 * @file gettureFont.ino
 * @brief Control the words in the center of the screen by zooming in, zooming out, sliding up, and sliding down in a two-finger gesture.
 * @n The demo supports Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
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
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
   @brief Constructor  When the touch uses the gt series chip, you can call this constructor
*/
DFRobot_Touch_GT911 touch;


/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is st7789, and the screen resolution is 240x320, this constructor can be called
   @param dc Command/data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
*/
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
   @brief Constructor
   @param gdl Screen object
   @param touch Touch object
*/
DFRobot_UI ui(&screen, &touch);

uint8_t fontSize = 2; //Font size
uint16_t fontColor = 0xf800; //Font color
uint8_t fontLen = 4 * fontSize * 8; //String width
char * font = "DFRC"; // String
uint8_t fontHeight = fontSize * 8;//String height
uint16_t posx =  screen.width() / 2 ; //The x-coordinate of the starting position of the string
uint16_t posy =  screen.height()  / 2;//the y-coordinate of the beginning of the string

void setup()
{

  Serial.begin(9600);
  //UI initialization
  ui.begin();
  //Draw the initial string
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
	//Gesture detected as zoom out 
    case ui.SHRINK : {
        screen.fillRect(posx - fontLen / 2 -8,  posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        //fontSize decrease
	     	fontSize--;
        if (fontSize <= 1) fontSize = 1;

        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2-8 ,  posy - fontHeight / 2,  font, fontColor , ui.bgColor, fontSize, 0);
      }; break;
	//Gesture detected as zoom in 
    case ui.MAGNIFY : {
        screen.fillRect(posx - fontLen / 2-8 ,  posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        //fontSize increase
		    fontSize++;
        if (fontSize >= 7) fontSize = 7;
        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2-8 , posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);
      } break;
    //Gesture detected as swiping up with two fingers
    case ui.DUPGLIDE : {
        screen.fillRect(posx - fontLen / 2-8 ,   posy - fontHeight / 2, 4 * fontSize * 8, fontSize * 8, ui.bgColor);
        posy -= 10 ;
        fontHeight = fontSize * 8;
        fontLen = 4 * fontSize * 8;
        ui.drawString(posx - fontLen / 2 - 8,  posy - fontHeight / 2, font, fontColor , ui.bgColor, fontSize, 0);
      } break;
	//Gesture detected as swiping down with two fingers
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
