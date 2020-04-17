/*!
 * @file UI_tableview.ino
 * @brief Create a tableview control on the screen, and the control parameters can be defined by users.
 * @n Users can select different pages to display different contents.
 * @n The example supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, and FireBeetle-M0.
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
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
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D3
#define TFT_CS  D4
#define TFT_RST D5
/*AVR series*/
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
 * @brief Constructor  Constructors for hardware SPI communication
 * @param dc  Command/data line pin of SPI communication
 * @param cs  Chip select pin for SPI communication
 * @param rst  Reset pin of the screen
 * @param bl  Screen backlight pin
 */
//DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
 * @brief Constructor
 * @param gdl Screen object
 * @param touch Touch object
 */
DFRobot_UI ui(&screen, NULL);

void tbCallback(void * highLightPage){
  uint8_t * hl = (uint8_t *)highLightPage;
  switch (*hl) {
     case 1:{
    //Display a string on the screen
       ui.drawString(10, 200, "this is tab1", COLOR_RGB565_YELLOW, ui.bgColor, 3, 0);break;
       }
    case 2: {
       ui.drawString(10, 200, "this is tab2", COLOR_RGB565_YELLOW, ui.bgColor, 3, 0);break;
       }
    case 3:  {
      
       ui.drawString(10, 200, "this is tab3", COLOR_RGB565_YELLOW, ui.bgColor, 3, 0);break;
      }
    case 4:  {
       ui.drawString(10, 200, "this is tab4", COLOR_RGB565_YELLOW, ui.bgColor, 3, 0);break;
  }
  }
}
void setup()
{
  
 Serial.begin(9600);

 ui.begin();
  //Set the theme of the UI, there are two themes to choose from: CLASSIC, MODERN.
 ui.setTheme(DFRobot_UI::MODERN);
 //Create a tableview control
 DFRobot_UI::sTableview_t &tv = ui.creatTableview();
 //Set the number and name of tableview, at most 4 pages.
 tv.setName(/*page=*/4,/*page1 name=*/"tab1",/*page2 name=*/"tab2",/*page3 name=*/"tab3",/*page4 name=*/"tab4");
 //Set callback function
 tv.setCallback(tbCallback);
 ui.draw(&tv);
 while(true){
 for(uint8_t i=1 ; i<5 ;i++ ){
  //Refresh
  ui.refresh();
  delay(500);
  tv.changeTableview(i);
  }
 }
}
void loop()
{

}
