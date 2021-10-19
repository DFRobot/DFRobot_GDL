/*!
 * @file UI_control.ino
 * @brief Create a switch, text box and slider control on the screen, users can directly use the controls' parameters here or use the default parameters.
 * @n When using a control, your current operation will be displayed in the text box.
 * @n The demo supports Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-SP8266, FireBeetle-M0
 * @n Click the text box to move the cursor into it when we need a text box to display.
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
/*M0*/
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
//Callback function of slider control
void changeColor(DFRobot_UI::sSlider_t &slider, DFRobot_UI::sTextBox_t &textBox) {
  String s(slider.value);
  textBox.setText("slider's value is " + s);
}

//Callback function of switch control
void switchCallBack(DFRobot_UI::sSwitch_t &sw, DFRobot_UI::sTextBox_t &textBox) {
  if (sw.state == 0) {
    textBox.setText("you have turn off the Switch!");

  }
  else {
    textBox.setText("you have turn on the Switch!");

  }

}
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
  //Initialize the slider control, initialize and assign the parameters of the slider
  DFRobot_UI::sSlider_t &slider = ui.creatSlider();
  slider.setCallback(changeColor);
  //  slider.bgColor = COLOR_RGB565_GREEN;
  //Set the output text box of the slider
  slider.setOutput(&tb);
  //Draw a slider at a specified position
  ui.draw(&slider,/*x = */(screen.width() - slider.width) / 2,/*y = */(screen.height() - tb.posy - tb.height) / 3 * 1 + tb.posy + tb.height);

  DFRobot_UI::sSwitch_t &swh = ui.creatSwitch();
  swh.bgColor = COLOR_RGB565_RED;
  swh.setCallback(switchCallBack);
  //Set the output text box of the switch
  swh.setOutput(&tb);
  //Draw switch at specified position
  ui.draw(&swh,/*x = */(screen.width() - swh.width - 40) / 2,/*y = */(screen.height() - tb.posy - tb.height) / 3 * 2 + tb.posy + tb.height,/*width=*/55,/*height=*/20);


}


void loop()
{
  ui.refresh();
}
