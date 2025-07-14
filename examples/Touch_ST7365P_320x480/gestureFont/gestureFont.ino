/*!
 * @file gestureFont.ino
 * @brief Control the words in the center of the screen by zooming in, zooming out, sliding up, and sliding down in a two-finger gesture.
 * @n The demo supports Mega2560, FireBeetle-ESP32, FireBeetle-M0
 *
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL
*/
#include "DFRobot_UI.h"
#include "Arduino.h"
#include "DFRobot_GDL.h"
#include "DFRobot_Touch.h"
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC    7
#define TFT_CS    5
#define TFT_RST   6
#define TFT_BL    9
#define TOUCH_RST 10
#define TOUCH_INT 2
/*ESP32 and ESP8266*/
#elif defined(ESP32)
#define TFT_DC    D2    
#define TFT_CS    D6     
#define TFT_RST   D3    
#define TFT_BL    D13   
#define TOUCH_RST D10
#define TOUCH_INT D11  
/* AVR series mainboard */
#else
#define TFT_DC    34
#define TFT_CS    36
#define TFT_RST   35
#define TFT_BL    33
#define TOUCH_RST 31
#define TOUCH_INT 32
#endif

/**
   @brief Constructor  When the touch uses the gt series chip, you can call this constructor
*/
//If you are using the Firebeetle 2 ESP32-E (N16R2) board and connecting the screen through the GDI interface with the address set to 0x14, and other card addresses are set to 0x5D.
DFRobot_Touch_GT911_IPS touch(0X5D,TOUCH_RST,TOUCH_INT); 

/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is ST7365P, and the screen resolution is 320x480, this constructor can be called
   @param dc Command/data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
*/
DFRobot_ST7365P_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
/* M0 mainboard DMA transfer */
//DFRobot_ST7365P_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);


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
