/*!
 * @file flashFont.ino
 * @brief Demonstrate the built-in font library in MO UD
 * @n The demo supports mainboard FireBeetle-M0
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
#define TFT_DC  D5
#define TFT_CS  D7
#define TFT_RST D6
#define TFT_BL  D3
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
  /* Set screen color */
  screen.fillScreen(COLOR_RGB565_WHITE);
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
  screen.setCursor(/*x=*/20,/*y=*/20);
// Set the text color
   // The optional color list is the same as the color list used in the fillScreen function
  screen.setTextColor(COLOR_RGB565_LGRAY);
  screen.setTextWrap(true);
  // Output text
  screen.print("DFRobot是上海智位机器人股份有限公司旗下注册商标，商标注册号\
  为19098139，国际分类号为42。DFRobot于2010年2月在上海成立，2013年网站正式上线。\
  致力于为青少年和创客爱好者提供开源硬件产品,机器人及零配件产品，拥有知识型创客社区、造物记、\
  蘑菇云创客空间，为专业和入门级创客提供全方位的软硬件支持。DFRobot总部位于上海浦东，是一家国际领先的从事开源硬件、\
  机器人产品、STEM教育、创客教育和人工智能教育的高科技企业。自成立以来，DFRobot推出自主研发产品上千件，并向世界各地\
  的教育和培训机构提供创客教育产品和解决方案，目前已进入全球8000多所学校.");
  
  delay(5000);
  

}
