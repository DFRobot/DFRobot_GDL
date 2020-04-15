/*!
 * @file UI_tableview.ino
 * @brief 在屏幕上创建一个tableview控件，用户可以自定义在屏幕上创建一个tableview控件的参数
 * @n 用户可以选择不同的页来显示不同的内容
 * @n 本示例支持的主板有Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * 
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
*/
/*!
 * @file UI_tableview.ino
 * @brief Create a tableview control on the screen, the user can customize the parameters of creating a tableview control on the screen
 * @n users can choose different pages to display different content
 * @n The motherboards supported by this example are Arduino Uno, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 *
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
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D3
#define TFT_CS  D4
#define TFT_RST D5
/*AVR系列主板*/  /* AVR series motherboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
   @brief Constructor  当触摸采用gt系列芯片时，可以调用此构造函数 When the touch uses the gt series chip, you can call this constructor
*/
DFRobot_Touch_GT911 touch;

/**
   @brief Constructor  当屏采用硬件SPI通信，驱动IC是st7789，屏幕分辨率是240x320时，可以调用此构造函数
   @param dc  SPI通信的命令/数据线引脚
   @param cs  SPI通信的片选引脚
   @param rst  屏的复位引脚
*/
/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is st7789, and the screen resolution is 240x320, this constructor can be called
   @param dc Command / data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
*/
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/ /* M0 motherboard DMA transfer */
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/**
   @brief 构造函数
   @param gdl 屏幕对象
   @param touch 触摸对象
*/
/**
   @brief constructor
   @param gdl screen object
   @param touch touch object
*/
DFRobot_UI ui(&screen, &touch);
//   tableview 回调函数  tableview callback function
void tbCallback(void * highLightPage){
  uint8_t * hl = (uint8_t *)highLightPage;
  switch (*hl) {
     case 1:{
    //在屏幕上显示字符串  Display a string on the screen
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
  ui.setTheme(DFRobot_UI::MODERN);

  
 //创建一个tableview控件  Create a tableview control
 DFRobot_UI::sTableview_t &tb = ui.creatTableview();
  //设置tableview的个数 和名字 最大页数为4 Set the number and name of tableview, the maximum number of pages is 4
 tb.setName(/*page=*/4,/*page1 name=*/"tab1",/*page2 name=*/"tab2",/*page3 name=*/"tab3",/*page4 name=*/"tab4");
  //设置回调函数  Set callback function
 tb.setCallback(tbCallback);
 ui.draw(&tb);


}
void loop()
{  
   //refresh
   ui.refresh();

}