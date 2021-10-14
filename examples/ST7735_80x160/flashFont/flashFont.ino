/*!
 * @file flashFont.ino
 * @brief Demonstrate the built-in font library in MO UD
 * @n The demo supports mainboard FireBeetle-M0
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V0.1
 * @date  2021-04-13
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include "DFRobot_GDL.h"
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
/* AVR series mainboard*/
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst Reset pin of the screen
 */
DFRobot_ST7735_80x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ST7735_80x160_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);



/*
 *User-selectable macro-defined color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  screen.begin();
  /* Set screen color */
  screen.fillScreen(COLOR_RGB565_BLACK);
}

void loop() {
  //Set the font size to 4, font size range 1-4
  screen.setTextSize(2);
  screen.setFont(&flashFont);//Read font from spi flash
  
  /*
   * @ brief Set text position
   * @ param x The x-coordinate of the first word of the text
   * @ param y The y-coordinate of the first word of the text 
   */
  screen.setCursor(/*x=*/10,/*y=*/0);
// Set the text color
   // The optional color list is the same as the color list used in the fillScreen function
  screen.setTextColor(COLOR_RGB565_LGRAY);
  screen.setTextWrap(true);
  // Output text
  screen.print("DFRobot于2010年2月在上海成立，2013年网站正式上线。\
  致力于为青少年和创客爱好者提供开源硬件产品,机器人及零配件产品");
  
  delay(5000);
  

}