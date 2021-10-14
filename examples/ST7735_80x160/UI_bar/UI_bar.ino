/*!
 * @file UI_bar.ino
 * @brief Create a progress bar control on the screen.
 * @n Users can customize the parameters of the progress bar or use the default parameters.
 * @n Users can control the value of the progress bar through the callback function of the progress bar.
 * @n The example supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0.
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V0.1
 * @date  2021-04-13
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
*/


#include "DFRobot_UI.h"
 #include "DFRobot_GDL.h"  
 
/*M0*/ 
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
/*AVR series mainboard*/
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
 * @brief Constructor  Constructors for hardware SPI communication
 * @param dc  Command pin or data line pin of SPI communication 
 * @param cs  Chip select pin for SPI communication
 * @param rst Reset pin of the screen
 * @param bl  Screen backlight pin
 */
DFRobot_ST7735_80x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ST7735_80x160_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
 * @brief Construct a function
 * @param gdl Screen object
 * @param touch Touch object
 */
DFRobot_UI ui(&screen, NULL);


uint8_t value1 = 0;
uint8_t value2 = 0;
uint8_t value3 = 0;
//Callback function of progress bar1
void barCallback1(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 in each time, it enters the callback function.
   delay(50);
    obj.setValue(value1);
	if(value1 < 100) value1++;
}
//Callback function of progress bar2
void barCallback2(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 in each time, it enters the callback function.
   delay(50);
    delay(50);
    obj.setValue(value2);
	if(value2 < 100) value2++;
	
}
//Callback function of progress bar3
void barCallback3(DFRobot_UI:: sBar_t &obj){
    //Enable the progress bar plus 1 in each time, it enters the callback function.
   delay(50);
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

  ui.drawString(/*x=*/4,/*y=*/30,"Page of load",COLOR_RGB565_WHITE,ui.bgColor,/*fontsize =*/2,/*Invert=*/0);
  //Create a progress bar control
  delay(3000);
  DFRobot_UI::sBar_t &bar3 = ui.creatBar();
  /**用户自定义进度条参数*/
  bar3.fgColor = COLOR_RGB565_BLUE;
  bar3.setStyle(DFRobot_UI::BAR);
  bar3.setCallback(barCallback3);
  ui.draw(&bar3,/*x=*/5,/*y=*/20,/*width =*/140,/*height =*/8);
    //Display a string on the screen
  

}


void loop()
{
  //Refresh 
  ui.refresh();
}