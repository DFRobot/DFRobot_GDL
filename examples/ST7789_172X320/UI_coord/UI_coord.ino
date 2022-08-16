/*!
 * @file UI_coord.ino
 * @brief Coordinate system control, users can customize the coordinate data in the coordinate system to draw polyline graph
 * @n The demo supports  FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [tangjie] (jie.tang@dfrobot.com)
 * @version V1.0
 * @date 2022-05-16
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
#define TFT_BL  9
/*ESP32*/
#elif defined(ESP32)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
#define TFT_BL  D4n
/*ESP8266*/
#elif defined(ESP8266)
#define TFT_DC  D8
#define TFT_CS  D6
#define TFT_RST D3
#define TFT_BL  D4
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TFT_BL  5
#endif

/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is st7789, and the screen resolution is 240x204, this constructor can be called
   @param dc Command/data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
   @param bl Rscreen backlight
*/
DFRobot_ST7789_172x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
/* M0 mainboard DMA transfer */
//DFRobot_ST7789_172x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);

/*
 *User-selectable macro definition color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */

/**
   @brief Constructor
   @param gdl Screen object
   @param touch Touch object
*/
DFRobot_UI ui(&screen, NULL);

int16_t point3[7][2] ={{0,50},{10,55},{15,65},{20,70},{63,75},{70,80},{80,90}};
void setup()
{
  
  Serial.begin(9600);
  //Initialize UI
  ui.begin();
  //Create a coordinate 
  DFRobot_UI::sCoordinate_t &coord = ui.creatCoordinate();

  coord.setPoint(point3,7,COLOR_RGB565_RED);
  ui.draw(&coord);

}


void loop()
{
  //Refresh 
  ui.refresh();
}
