/*!
 * @file ChineseFont.ino
 * @brief Present the effects of different Chinese font libraries 
 * @n This demo supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2020-01-07
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
 *User-selectable macro-defined colors
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
  //set font size to 4, range 1 to 4
  screen.setTextSize(4);
  /*set screen color */
  screen.fillScreen(COLOR_RGB565_BLACK);
  //set font file
  screen.setFont(&SIMKAIFont12pt);
  
  /*
   *@brief set text position 
   *@param x the x-axis of the first word of text
   *@param y the y -coordinate of the first word of text
   */
  screen.setCursor(/*x=*/10,/*y=*/120);
  //set text color 
  //selectable color list is the same as the color list in fillscreen function
  screen.setTextColor(COLOR_RGB565_BLUE); 
  //set the text to word wrap mode 
  //true=word wrap，false=not 
  screen.setTextWrap(true);
  // output the text   
  screen.print("你好");
  delay(2000);
  
  //use SIMKAIFont18pt font
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&SIMKAIFont18pt);
  screen.setCursor(10,120);
  screen.setTextColor(COLOR_RGB565_RED);
  screen.setTextWrap(true);
  screen.print("你好");
  delay(2000);
  
  //use SIMKAIFont24pt font 
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&SIMKAIFont24pt);
  screen.setCursor(10,120);
  screen.setTextColor(COLOR_RGB565_BLUE);
  screen.setTextWrap(true);
  screen.print("你好");
  delay(2000);
  
  //use SIMKAIFont36pt font
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&SIMKAIFont36pt);
  screen.setCursor(10,120);
  screen.setTextColor(COLOR_RGB565_WHITE);
  screen.setTextWrap(true);
  screen.print("你好");
  delay(2000);
  
  //use SIMKAIFont48pt font 
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&SIMKAIFont48pt);
  screen.setCursor(10,120);
  screen.setTextColor(COLOR_RGB565_YELLOW);
  screen.setTextWrap(true);
  screen.print("你好");
  delay(2000);
  
  //use SIMKAIFont72pt font
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setFont(&SIMKAIFont72pt);
  screen.setCursor(10,120);
  screen.setTextColor(COLOR_RGB565_ORANGE);
  screen.setTextWrap(true);
  screen.print("你好");
  delay(2000);
}
