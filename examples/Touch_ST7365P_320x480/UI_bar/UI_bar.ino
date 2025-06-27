/*!
 * @file UI_bar.ino
 * @brief Create a progress bar control on the screen.
 * @n Users can customize the parameters of the progress bar, or use the default parameters.
 * @n Users can control the value of the progress bar through the callback function of the progress bar.
 * @n The demo supports Mega2560, FireBeetle-ESP32, and FireBeetle-M0.
 *
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
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
#define TOUCH_RST 2
#define TOUCH_INT 3
/*ESP32 and ESP8266*/
#elif defined(ESP32)
#define TFT_DC    26    
#define TFT_CS    14     
#define TFT_RST   0    
#define TFT_BL    25   
#define TOUCH_RST 13
#define TOUCH_INT 2 
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

uint8_t value1 = 0;
uint8_t value2 = 0;
uint8_t value3 = 0;
//Callback function of progress bar1
void barCallback1(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 in each time automatically.
   delay(50);
    obj.setValue(value1);
	if(value1 < 100) value1++;
}
//Callback function of progress bar2
void barCallback2(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 in each time automatically.
    delay(50);
    obj.setValue(value2);
	if(value2 < 100) value2++;
	
}
//Callback function of progress bar3
void barCallback3(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 in each time automatically.
    delay(50);
    obj.setValue(value3);
	if(value3 < 100) value3++;
}
void setup()
{
  
  Serial.begin(9600);
  //Initialize UI
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
  /** User-defined progress bar parameters */
  bar2.setStyle(DFRobot_UI::CIRCULAR);
  bar2.setCallback(barCallback2);
  ui.draw(&bar2,/*x=*/screen.width()/2,/*y=*/screen.height()/5*2);

  DFRobot_UI::sBar_t &bar3 = ui.creatBar();
  /**User-defined progress bar parameters */
  bar3.fgColor = COLOR_RGB565_BLUE;
  bar3.setStyle(DFRobot_UI::BAR);
  bar3.setCallback(barCallback3);
  ui.draw(&bar3,/*x=*/(screen.width()-bar3.width)/2,/*y=*/screen.height()/5*1);
  // Display a string on the screen
  ui.drawString(/*x=*/33,/*y=*/screen.height()/5*4,"Page of loading",COLOR_RGB565_WHITE,ui.bgColor,/*fontsize =*/2,/*Invert=*/0);
}


void loop()
{
  //refresh
  ui.refresh();
}
