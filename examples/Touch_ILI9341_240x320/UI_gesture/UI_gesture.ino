/*!
 * @file UI_gesture.ino
 * @brief Recognize user's gesture in the specified area of the screen, and display the name of the gesture in the text box.
 * @n The demo supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @n Click the text box to move the cursor into it when we need a text box to display.
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
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
 * @brief Constructor When the screen uses hardware SPI communication, the driver IC is ILI9341, and the screen resolution is 240x320, this constructor can be called
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst Reset pin of the screen
 */
DFRobot_ILI9341_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ILI9341_240x320_DMA_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

/**
 * @brief Constructor
 * @param gdl Screen object
 * @param touch Touch object
 */
DFRobot_UI ui(&screen, &touch);



void setup()
{
  
  Serial.begin(9600);
  ui.begin();
  // Set the UI theme, there are two themes to choose from: CLASSIC and MODERN.
  ui.setTheme(DFRobot_UI::MODERN);
  
  //Create a text box control
  DFRobot_UI::sTextBox_t &tb = ui.creatText();
  //Create a text box control on the screen and draw the text box according to the customized or initialized parameters
  ui.draw(&tb);
  /**
   * @brief  Set the touch gesture recognition area
   */
  ui.setGestureArea(/*x=*/screen.width()/2-75,/*y=*/150,/*width=*/150,/*height=*/150);
  while(true){
     //Refresh
    ui.refresh();
    // getGestures()： Get gesture
    switch(ui.getGestures()){
      //setText：let the text box display a string
      case ui.SUPGLIDE : tb.setText("upwards slide"); break;
      case ui.SDOWNGLIDE : tb.setText("down slide"); break;
      case ui.SLEFTGLIDE : tb.setText("left slide"); break;
      case ui.SRIGHTGLIDE : tb.setText("right slide"); break;
      case ui.DLONGPRESSED : tb.setText("long press"); break;
      case ui.SCLICK : tb.setText("click"); break;
      case ui.DDOUBLECLICK : tb.setText("double click"); break;
      default  :  break;
      }
  }
}


void loop()
{


}
