/*!
 * @file SD.ino
 * @brief 从SD卡上读取bmp/jpg/jpeg格式图片并显示在屏幕上，bmp支持16位/24位/32位，jpg只支持JFIF格式用windows的画图打开然后另存为这个格式就行了。
 * @n 本demo的演示图片存放在example->SD->picture下；拷贝picture文件夹到SD卡即可
 * @n 本示例支持的主板有ESP8266、FireBeetle-M0,MAGE2560，UNO
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [YeHangYu](hangyu.ye@dfrobot.com)
 * @version  V0.1
 * @date  2020-03-20
 * @url https://github.com/DFRobot/DFRobot_GDL
 */

/*!
 * @file SD.ino
 * @brief reads pictures in bmp / jpg / jpeg format from the SD card and displays them on the screen. bmp supports 16-bit / 24-bit / 32-bit, jpg only supports JFIF format, open with windows drawing and save as this format .
 * @n The demo picture of this demo is stored under example-> SD-> picture; just copy the picture folder to the SD card
 * @n The motherboards supported by this example are ESP8266, FireBeetle-M0, MAGE2560, UNO
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [YeHangYu] (hangyu.ye@dfrobot.com)
 * @version V0.1
 * @date 2020-03-20
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include <SD.h>
#include <SPI.h>
#include "DFRobot_GDL.h"
#include "picdecoder.h"

//自定义通信引脚  Custom communication pins
/*FireBeetle-M0*/
#if defined ARDUINO_SAM_ZERO
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6
#define TFT_SD  3
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define TFT_DC  D3
#define TFT_CS  D4
#define TFT_RST D5
#define TFT_SD  D6
/*AVR系列主板*/  /* AVR series motherboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TFT_SD  6
#endif
/**
 * @brief Constructor  硬件SPI通信的构造函数
 * @param dc  SPI通信的命令/数据线引脚
 * @param cs  SPI通信的片选引脚
 * @param rst  屏的复位引脚
 */
/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command / data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */
//DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/  /* M0 motherboard DMA transfer */
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

/*
 *可供用户选择的宏定义颜色 User-selectable macro definition color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE
 */
void setup()
{
  //初始化串口 Initialize the serial port
  Serial.begin(115200);
  while (!Serial)
  {
    ; //等待串口连接 Wait for serial connection
  }
  //初始化屏幕  Initialization screen
  screen.begin();
  //初始化SD卡，直到初始化成功  Initialize the SD card until the initialization is successful
  while(1)
  {
    if (SD.begin(/*sdcs=*/TFT_SD))
    {
      Serial.println("initialization done.");
      break;
    }
    Serial.println("initialization failed!");
  }
}



void loop()
{
  /*设置屏幕颜色为白色*/  /* Set the screen color to white */
  screen.fillScreen(COLOR_RGB565_WHITE);

  /**
   * @brief 解码函数，通过调用画点函数显示sd卡上的bmp图片，FireBeetle-M0，ESP32和ESP8266可以显示jpg图片
   * @param filename 要打开的SD卡文件名及路径（由于SD.open的限制，名称不能太长）
   * @param sx 开始显示的x坐标
   * @param sy 开始显示的y坐标
   * @param ex 结束显示的x坐标
   * @param ey 结束显示的y坐标
   * @param screenDrawPixel 画点函数名
   */
  /**
   * @brief decoding function, display bmp pictures on sd card by calling draw point function, FireBeetle-M0, ESP32 and ESP8266 can display jpg pictures
   * @param filename The file name and path of the SD card to be opened (due to the limitation of SD.open, the name cannot be too long)
   * @param sx start to display the x coordinate
   * @param sy the y coordinate to start displaying
   * @param ex End the displayed x coordinate
   * @param ey End the displayed y coordinate
   * @param screenDrawPixel draw point function name
   */
  drawSDPicture(/*filename=*/"picture/219x220.jpg",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);
  screen.fillScreen(COLOR_RGB565_WHITE);
  drawSDPicture(/*filename=*/"picture/RGB565.bmp",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);

  /*设置屏幕颜色为白色*/ /* Set the screen color to white */
  screen.fillScreen(COLOR_RGB565_WHITE);

  //批量显示图标，大容量主板显示任意数量的图标  Batch display icons, large-capacity motherboard displays any number of icons
  /*FireBeetle-M0，ESP32和ESP8266*/
#if defined ARDUINO_SAM_ZERO || defined(ESP8266)
  /*
  *SD.open函数可设置的mode参数
  *FILE_READ: 打开文件进行读取，从文件的开头开始
  *FILE_WRITE: 打开文件进行读写，从文件末尾开始
  */
 /*
  * Mode parameter that can be set by SD.open function
  * FILE_READ: Open the file for reading, starting from the beginning of the file
  * FILE_WRITE: open the file for reading and writing, starting from the end of the file
  */
  File myDir = SD.open(/*directory name=*/"picture/Icon/",/*mode=*/FILE_READ);
  if(myDir)
  {
    char str[32];//存放读取的目录下图标的文件名  File name of the icon in the read directory
    //每隔32个像素显示一个图标，直到铺满屏幕或显示完所有图标  Display an icon every 32 pixels until the screen is full or all icons are displayed
    for(uint16_t y = 10; y<screen.height()-32; y+=60)//y coordinate
    {
      for(uint16_t x = 10; x<screen.width()-32; x+=60)//X coordinate
      {
        //读取目录中下一个文件信息  Read the next file information in the directory
        File entry = myDir.openNextFile();
        if (! entry)
        {
          //没有可以显示的图片了，跳出循环 There are no more pictures to show, jump out of the loop
          goto quit;
        }
        //拼接文件名 Splice file name
        strcpy(str,"picture/Icon/");
        strcat(str,entry.name());
        //显示一个图标  Show an icon
        drawSDPicture(/*filename=*/str,/*sx=*/x,/*sy=*/y,/*ex=*/x+32,/*ey=*/y+32,/*screenDrawPixel=*/screenDrawPixel);
      }
    }
quit:
    myDir.close();
  }
  else
  {
    Serial.println("dir open fail");
  }
  /*AVR系列主板*/  /* AVR series motherboard */
#else
  drawSDPicture("picture/Icon/1.bmp",0,0,32,32,screenDrawPixel);
  drawSDPicture("picture/Icon/2.bmp",32,32,64,64,screenDrawPixel);
  drawSDPicture("picture/Icon/3.bmp",64,64,96,96,screenDrawPixel);
  drawSDPicture("picture/Icon/4.bmp",96,96,128,128,screenDrawPixel);
  drawSDPicture("picture/Icon/5.bmp",128,128,160,160,screenDrawPixel);
  drawSDPicture("picture/Icon/6.bmp",160,160,192,192,screenDrawPixel);
  drawSDPicture("picture/Icon/7.bmp",192,192,224,224,screenDrawPixel);
  //drawSDPicture("picture/Icon/8.bmp",224,224,250,250,screenDrawPixel);
#endif
  delay(1000);
}

//供解码函数调用，该函数的功能是在屏幕上画一个像素点  For decoding function call, the function of this function is to draw a pixel on the screen
void screenDrawPixel(int16_t x, int16_t y, uint16_t color)
{
  //屏幕画点  Screen drawing
  screen.writePixel(x,y,color);
}
