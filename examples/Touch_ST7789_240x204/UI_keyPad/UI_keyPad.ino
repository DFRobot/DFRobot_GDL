/*!
 * @file UI_keypad.ino
 * @brief Create a numeric keyboard control on the screen
 * @n Users can click number on the keyboard and then the output will be displayed in the text box.
 * @n Click the text box to move the cursor into it when we need a text box to display.
 * @n The demo supports Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, and FireBeetle-M0.
 *
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @maintainer [Fary] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2021-10-19
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL
*/

#include "DFRobot_UI.h"
#include "Arduino.h"
#include "DFRobot_GDL.h"
#include "DFRobot_Touch.h"
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
#define TFT_BL  9
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
#define TFT_BL  D13
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TFT_BL  5
#endif

/**
   @brief Constructor  When the touch uses the gt series chip, you can call this constructor
*/

DFRobot_Touch_FT3267 touch;

/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is st7789, and the screen resolution is 240x204, this constructor can be called
   @param dc Command/data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
*/
DFRobot_ST7789_240x204_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
/* M0 mainboard DMA transfer */
//DFRobot_ST7789_240x204_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);


/**
   @brief Constructor
   @param gdl Screen object
   @param touch Touch object
*/
DFRobot_UI ui(&screen, &touch);




void setup()
{

  Serial.begin(9600);

  ui.begin();
  // Set the UI theme, there are two themes to choose from: CLASSIC and MODERN.
  ui.setTheme(DFRobot_UI::CLASSIC);
  
   //Create a numeric keypad
  DFRobot_UI::sObject_t &kp = ui.creatKeyPad();
  ui.draw(&kp);
}

void loop()
{
  // refresh
  ui.refresh();
}
