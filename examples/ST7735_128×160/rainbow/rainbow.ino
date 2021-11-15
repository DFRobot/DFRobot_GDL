/*!
 * @file basicTest.ino
 * @brief Demonstrate various graphic painting effects
 * @n This demo supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, and FireBeetle-M0.
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fary] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2021-11-04
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include "DFRobot_GDL.h"
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 ESP8266*/
#elif defined(ESP32)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
/*ESP8266*/
#elif defined(ESP8266)
#define TFT_DC  D4
#define TFT_CS  D6
#define TFT_RST D5
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */
//DFRobot_ST7735_128x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ST7735_128x160_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

byte red = 29; // Red is the top 5 bits of a 16 bit colour value
byte green = 0;// Green is the middle 6 bits
byte blue = 0; // Blue is the bottom 5 bits
byte state = 0;
/*
 *User-selectable macro definition color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */
 boolean initial = 1;
void setup() {
  Serial.begin(115200);
  screen.begin();
  screen.fillScreen(COLOR_RGB565_LGRAY);
  for(uint16_t i=0;i<screen.height();i++){
    screen.drawFastHLine(0,i,screen.width(),rainbow());
    
   }

}


void loop() {
  screen.setTextSize(1);
  /* Set screen color */
  /*
   *The currently available fonts are as follows, you can add other font files in gfxfont.h (font files are stored in the src/Frame/Fonts folder)
   *FreeMono9pt7b, FreeMono12pt7b, FreeMonoBold12pt7b,
   *FreeMonoBoldOblique12pt7b, FreeMonoOblique12pt7b,
   *FreeSans12pt7b,FreeSansBold12pt7b, FreeSansBoldOblique12pt7b,
   *FreeSansOblique12pt7b, FreeSerif12pt7b, FreeSerifBold12pt7b,
   *FreeSerifBoldItalic12pt7b, FreeSerifItalic12pt7b, FreeMono24pt7b
   */
  screen.setFont(&FreeMono9pt7b);//Set the font to FreeMono12pt7b
  
  /*
   * @ brief Set text position
   * @ param x The x-coordinate of the first word of the text
   * @ param y The y-coordinate of the first word of the text 
   */
  screen.setCursor(/*x=*/10,/*y=*/screen.height()-108);
   // Set the text color
   // The optional color list is the same as the color list used in the fillScreen function
  screen.setTextColor(COLOR_RGB565_BLACK);
  //Set to text auto-wrapping mode
  //true = Auto-wrap, false =No auto-warp
  screen.setTextWrap(true);
  //Output text
  screen.print("DFRobot");
  delay(500);
  
  //Use FreeMonoBold12pt7b font

  screen.setFont(&FreeMonoBoldOblique9pt7b);
  screen.setCursor(10,screen.height()-72);
  screen.setTextColor(COLOR_RGB565_RED);
  screen.setTextWrap(true);
  screen.print("GDL");
  delay(500);
   
  //Use FreeMonoBoldOblique12pt7b font

  screen.setFont(&FreeSansBold9pt7b);
  screen.setCursor(10,screen.height()-38);
  screen.setTextColor(COLOR_RGB565_YELLOW);
  screen.setTextWrap(true);
  screen.print("fonts test");
  delay(500);
  
  //Use FreeMonoOblique12pt7b font
  screen.setFont(&FreeSerif9pt7b);
  screen.setCursor(10,screen.height()-6);
  screen.setTextColor(COLOR_RGB565_BLUE);
  screen.setTextWrap(true);
  screen.print("hello,world!");
  delay(500);
}


unsigned int rainbow()
{
    switch (state) {
      case 0:
        green ++;
        if (green == 64) {
          green = 63;
          state = 1;
        }
        break;
      case 1:
        red--;
        if (red == 255) {
          red = 0;
          state = 2;
        }
        break;
      case 2:
        blue ++;
        if (blue == 32) {
          blue = 31;
          state = 3;
        }
        break;
      case 3:
        green --;
        if (green == 255) {
          green = 0;
          state = 4;
        }
        break;
      case 4:
        red ++;
        if (red == 32) {
          red = 31;
          state = 5;
        }
        break;
      case 5:
        blue --;
        if (blue == 255) {
          blue = 0;
          state = 0;
        }
        break;
    }
    return red << 11 | green << 5 | blue;
}
