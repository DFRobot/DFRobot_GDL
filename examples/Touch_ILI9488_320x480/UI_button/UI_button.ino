/*!
 * @file UI_button.ino
 * @brief 在屏幕上创建一个按钮控件，用户可以自定义按钮的参数
 * @n 示例里面创建了三个按钮on,off,clr,按下on,off按钮会在文本框内显示，按下clr按钮会删除文本框的一个字符
 * @n 需要文本框显示时，需要点击文本框以使光标移到文本框内
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
 * @file UI_button.ino
 * @brief creates a button control on the screen, the user can customize the parameters of the button
 * The @n example creates three buttons on, off, and clr. Pressing the on and off buttons will display in the text box. Pressing the clr button will delete a character in the text box.
 * @n When the text box needs to be displayed, you need to click the text box to move the cursor into the text box
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
//三个按钮的回调函数  Callback function for three buttons
void btnCallback(DFRobot_UI::sButton_t &btn,DFRobot_UI::sTextBox_t &obj) {
   String text((char *)btn.text);
   if(text == "ON"){
    obj.setText("you have touch button on");
    }
   else if(text == "OFF"){
    obj.setText("you have touch button off");
    }
   else if(text == "clr"){
    obj.deleteChar();
    }
    
}

void setup()
{

  Serial.begin(9600);
  //UI初始化  UI initialization
  ui.begin();
  // 设置UI的主题，有两种主题可供选择 1.CLASSIC ，2.MODERN。 Set the theme of the UI, there are two themes to choose from 1.CLASSIC, 2.MODERN.
  ui.setTheme(DFRobot_UI::MODERN);
  //创建一个文本框控件  Create a text box control
  DFRobot_UI::sTextBox_t & tb = ui.creatText();
  tb.bgColor = 0xe6B6;
  ui.draw(&tb,/**x=*/5,/**y=*/5,/*width*/screen.width()-10,/*height*/screen.height()/5);
  //在屏幕上创建一个按钮控件 Create a button control on the screen
  DFRobot_UI::sButton_t & btn1 = ui.creatButton();
  //设置按钮的名字  Set the name of the button
  btn1.setText("ON");
  btn1.bgColor = COLOR_RGB565_RED;
  btn1.setCallback(btnCallback);
  //每个按钮都有一个文本框的参数，需要自己设定 Each button has a text box parameter, you need to set it yourself
  btn1.setOutput(&tb);
  ui.draw(&btn1,/**x=*/screen.width()/10,/**y=*/screen.height()/2,/*width*/screen.width()/10*2,/*height*/screen.width()/10*2);
  
  DFRobot_UI::sButton_t & btn2 = ui.creatButton();
  btn2.setText("OFF");
  btn2.bgColor = COLOR_RGB565_GREEN;
  btn2.setCallback(btnCallback);
  //每个按钮都有一个文本框的参数，需要自己设定 Each button has a text box parameter, you need to set it yourself
  btn2.setOutput(&tb);
  ui.draw(&btn2,/**x=*/(screen.width()/10)*4,/**y=*/screen.height()/2,/*width*/screen.width()/10*2,/*height*/screen.width()/10*2);
 
  DFRobot_UI::sButton_t & btn3 = ui.creatButton();
  btn3.bgColor = COLOR_RGB565_BLUE;
  btn3.setText("clr");

  //Set the callback function of the button
  btn3.setCallback(btnCallback);
  //每个按钮都有一个文本框的参数，需要自己设定 Each button has a text box parameter, you need to set it yourself
  
  btn3.setOutput(&tb);
  ui.draw(&btn3,/**x=*/(screen.width()/10)*7,/**y=*/screen.height()/2,/*width*/screen.width()/10*2,/*height*/screen.width()/10*2);
}
void loop()
{
  //refresh
  ui.refresh();
}