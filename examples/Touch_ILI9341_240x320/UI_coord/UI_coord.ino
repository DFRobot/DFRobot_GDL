/*!
 * @file UI_coord.ino
 * @brief Coordinate system control, users can customize the coordinate data in the coordinate system to draw polyline graph
 * @n The demo supports Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
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
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
#define TOUCH_CS D12
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TOUCH_CS 5
#endif

/**
 * @brief Constructor When the touch uses XPT2046 chip, you can call this constructor
 * @param cs SPI chip select signal
 * @param rst Reset signal
 * @param irq Interrupt signal
 */
DFRobot_Touch_XPT2046 touch(/*cs=*/TOUCH_CS);

/**
 * @brief Constructor When the screen uses hardware SPI communication, the driver IC is ILI9341, and the screen resolution is 240x320, this constructor can be called
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst Reset pin of the screen
 */
 
DFRobot_ILI9341_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ILI9341_240x320_DMA_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
 * @brief Constructor
 * @param gdl Screen object
 * @param touch Touch object
 */
DFRobot_UI ui(&screen, &touch);

/*
 *User-selectable macro-defined color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */

int16_t point3[15][2] ={{0,50},{10,55},{15,65},{20,70},{63,75},{70,80},{80,90},{100,100},{110,110},{112,112},{150,120},{210,130},{230,130},{240,150},{300,190}};
void setup()
{
  
  Serial.begin(9600);
  //Initialize UI
  ui.begin();
  //Create a coordinate 
  DFRobot_UI::sCoordinate_t &coord = ui.creatCoordinate();

  coord.setPoint(point3,15,COLOR_RGB565_RED);
  ui.draw(&coord);

}


void loop()
{
  //Refresh 
  ui.refresh();
}