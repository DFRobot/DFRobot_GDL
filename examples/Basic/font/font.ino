/*!
 * @file font.ino
 * @brief Demonstrate different English fonts
 * @n The demo supports mainboard Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, and FireBeetle-M0
 * @n Leonado doesn't have enough space to run this demo
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [LuoYufeng] (yufeng.luo@dfrobot.com)
 * @version V0.1
 * @date 2020-01-07
 * @url https://github.com/DFRobot/DFRobot_GDL
 */

#include "DFRobot_GDL.h"
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
#define TFT_BL  9
#define TFT_SD  3
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
#define TFT_BL  D13
#define TFT_SD  D7
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TFT_BL  5
#define TFT_SD  6
#endif

/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */
//DFRobot_ST7789_240x204_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
//DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7365P_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
/* M0 mainboard DMA transfer */
//DFRobot_ST7789_240x204_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7365P_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);


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
}

void loop() {
  //Set the font size to 4, font size range 1-4
  screen.setTextSize(2);
  /* Set screen color */
  screen.fillScreen(COLOR_RGB565_BLACK);
  /*
   *The currently available fonts are as follows, you can add other font files in gfxfont.h (font files are stored in the src/Frame/Fonts folder)
   *FreeMono9pt7b, FreeMono12pt7b, FreeMonoBold12pt7b,
   *FreeMonoBoldOblique12pt7b, FreeMonoOblique12pt7b,
   *FreeSans12pt7b,FreeSansBold12pt7b, FreeSansBoldOblique12pt7b,
   *FreeSansOblique12pt7b, FreeSerif12pt7b, FreeSerifBold12pt7b,
   *FreeSerifBoldItalic12pt7b, FreeSerifItalic12pt7b, FreeMono24pt7b
   */
  screen.setFont(&FreeMono12pt7b);//Set the font to FreeMono12pt7b
  
  /*
   * @ brief Set text position
   * @ param x The x-coordinate of the first word of the text
   * @ param y The y-coordinate of the first word of the text 
   */
  screen.setCursor(/*x=*/10,/*y=*/120);
   // Set the text color
   // The optional color list is the same as the color list used in the fillScreen function
  screen.setTextColor(COLOR_RGB565_LGRAY);
  //Set to text auto-wrapping mode
  //true = Auto-wrap, false =No auto-warp
  screen.setTextWrap(true);
  //Output text
  screen.print("DFRobot");
  delay(500);
  
  //Use FreeMonoBold12pt7b font
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&FreeMonoBold12pt7b);
  screen.setCursor(10,120);
  screen.setTextColor(COLOR_RGB565_GREEN);
  screen.setTextWrap(true);
  screen.print("GDL");
  delay(500);
   
  //Use FreeMonoBoldOblique12pt7b font
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&FreeMonoBoldOblique12pt7b);
  screen.setCursor(10,160);
  screen.setTextColor(COLOR_RGB565_RED);
  screen.setTextWrap(true);
  screen.print("fonts test");
  delay(500);
  
  //Use FreeMonoOblique12pt7b font
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&FreeMonoOblique12pt7b);
  screen.setCursor(80,160);
  screen.setTextColor(COLOR_RGB565_BLUE);
  screen.setTextWrap(true);
  screen.print("hello,world!");
  delay(500);
}
