/*!
 * @file UI_login.ino
 * @brief A login interface
 * @n The example supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
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
 * @param dc  Command pin or data line pin of SPI communication
 * @param cs  Chip select pin for SPI communication
 * @param rst  Reset pin of the screen
 * @param bl  Screen backlight pin
 */
DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
 * @brief Construct a function
 * @param gdl Screen object
 * @param touch Touch object
 */
DFRobot_UI ui(&screen, NULL);


void setup()
{

  Serial.begin(9600);
  ui.begin();
  //Set the UI theme, there are two themes to choose from 1.CLASSIC, 2.MODERN.
  ui.setTheme(DFRobot_UI::CLASSIC);
  //Create a text box activex
  DFRobot_UI::sTextBox_t& tb1 = ui.creatText();
   ui.draw(&tb1,/**x=*/30,/**y=*/30,/*width*/180,/*height=*/30);
  //Create a text box activex
  DFRobot_UI::sTextBox_t& tb2 = ui.creatText();
  ui.draw(&tb2,/**x=*/30,/**y=*/80,/*width*/180,/*height=*/30);

  
  //Create a button activex in the screen
  DFRobot_UI::sButton_t & btn1 = ui.creatButton();
  //Set the button name
  btn1.setText("login");
  ui.draw(&btn1,/**x=*/30,/**y=*/120,/*width*/180,/*height*/30);
  //Create a numeric keyboard activex
  DFRobot_UI::sKeyPad_t & kp = ui.creatKeyPad();
  //Set the numeric keyboard mode: NOCHOICE-bring the text box itself; CHOICE-the text box needed to be defined externally.
  kp.setMode(DFRobot_UI::CHOICE);
  ui.draw(&kp);
  ui.drawString(/*x=*/30, /*y=*/10, "login interface", COLOR_RGB565_CYAN, ui.bgColor,/*字体大小=*/ 2, /*反显*/0);
}


void loop()
{
  //Refresh
  ui.refresh();

}
