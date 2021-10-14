/*!
 * @file UI_bar.ino
 * @brief Create a progress bar control on the screen that allows the user to customize the parameters of the progress bar,
 * @n or they can use the default parameters.
 * @n The user can control the value of the progress bar through the callback function of the progress bar.
 * @n The example supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
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
 * @brief Constructor When the screen adopts hardware SPI communication, the driver IC is ILI9341, and the screen resolution is 240x320, this constructor can be called
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst Reset pin of the screen
 */
DFRobot_ILI9341_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
 /* M0 mainboard DMA transfer */
//DFRobot_ILI9341_240x320_DMA_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
 * @brief constructor
 * @param gdl screen object
 * @param touch touch object
 */
DFRobot_UI ui(&screen, &touch);

uint8_t value1 = 0;
uint8_t value2 = 0;
uint8_t value3 = 0;
//Callback function of progress bar1
void barCallback1(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 automatically in each time
   delay(50);
    obj.setValue(value1);
	if(value1 < 100) value1++;
}
// Callback function of progress bar2
void barCallback2(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 automatically in each time
    delay(50);
    obj.setValue(value2);
	if(value2 < 100) value2++;
	
}
// Callback function of progress bar3
void barCallback3(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 automatically in each time
    delay(50);
    obj.setValue(value3);
	if(value3 < 100) value3++;
}
void setup()
{
  
  Serial.begin(9600);
  //UI initialization
  ui.begin();
  ui.setTheme(DFRobot_UI::MODERN);

  
  //Create a progress bar control
  DFRobot_UI::sBar_t &bar1 = ui.creatBar();
  /** User-defined progress bar parameters */
  bar1.setStyle(DFRobot_UI::COLUMN);
  bar1.fgColor = COLOR_RGB565_GREEN;
  bar1.setCallback(barCallback1);
  ui.draw(&bar1,/*x=*/33,/*y=*/screen.height()/5*3);
  
  DFRobot_UI::sBar_t &bar2 = ui.creatBar();
  /* User-defined progress bar parameters */
  bar2.setStyle(DFRobot_UI::CIRCULAR);
  bar2.setCallback(barCallback2);
  ui.draw(&bar2,/*x=*/120,/*y=*/screen.height()/5*2);

  DFRobot_UI::sBar_t &bar3 = ui.creatBar();
  /* User-defined progress bar parameters */
  bar3.fgColor = COLOR_RGB565_BLUE;
  bar3.setStyle(DFRobot_UI::BAR);
  bar3.setCallback(barCallback3);
  ui.draw(&bar3,/*x=*/(screen.width()-bar3.width)/2,/*y=*/screen.height()/5*0.5);
  //Display a string on the screen
  ui.drawString(/*x=*/33,/*y=*/screen.height()/5*4,"Page of loading",COLOR_RGB565_WHITE,ui.bgColor,/*fontsize =*/2,/*Invert=*/0);
}


void loop()
{
  //Refresh all controls
  ui.refresh();
}
