/*!
 * @file icon.ino
 * @brief 在屏幕上显示一些小图标
 * @n 本示例支持的主板有Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [YeHangYu](hangyu.ye@dfrobot.com)
 * @version  V0.1
 * @date  2020-01-07
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
/*!
 * @file icon.ino
 * @brief displays some small icons on the screen
 * @n The motherboards supported by this example are Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [YeHangYu] (hangyu.ye@dfrobot.com)
 * @version V0.1
 * @date 2020-01-07
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include "DFRobot_GDL.h"
#include "Icon.h"
//自定义通信引脚  Custom communication pins
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



void setup() {
  Serial.begin(115200);
  screen.begin();
}

void loop() {
  int w = screen.width();
  int h = screen.height();
  int a = millis()/1000;//获取时间  Get Time
     //0x00FF 是格式为RGB565的颜色数据  0x00FF Is color data in the format RGB565
  uint16_t color = 0x00FF;
  
  screen.fillScreen(COLOR_RGB565_WHITE);
  
  while(1) {
    for(int i = 0;i < 12; i++){
      //用字库显示时间，程序执行秒数  Display time with font, seconds of program execution
      screen.fillRect(16,16,w-16*2,35, COLOR_RGB565_WHITE);
      screen.setTextWrap(false);
	   //0x30FF 是格式为RGB565的颜色数据  0x30FF is the color data in the format of RGB565
      screen.setTextColor(0x30FF);
      screen.setTextSize(3);
      screen.setCursor(30, 30);
      screen.println("Time:");
	   //0x00FF 是格式为RGB565的颜色数据  0x00FF is color data in RGB565 format
      screen.setTextColor(0x00FF);
      screen.setTextSize(3);
      screen.setCursor(120, 30);
      a = millis()/1000;
      screen.println(a, 1);
      /**
       * @brief 填充圆角矩形
       * @param x      矩形的起始点坐标
       * @param y      矩形的起始点坐标
       * @param w      矩形宽度
       * @param h      矩形高度
       * @param r      圆角半径
       * @param color  矩形颜色
       */
      /**
       * @brief fills rounded rectangle
       * @param x starting point coordinates of the rectangle
       * @param y the starting point coordinates of the rectangle
       * @param w rectangle width
       * @param h rectangle height
       * @param r fillet radius
       * @param color rectangle color
       */
       screen.fillRoundRect(w/2-48-12, h/2-16-8, 32*3+12*2, 32+8*2, 20, 0x0000);
       /**
       * @brief 画水平线和竖线来绘制边框
       * @param x      画线起始点的x坐标
       * @param y      画线起始点的y坐标
       * @param h      线的长度
       * @param color  线的颜色
       */
      /**
       * @brief draw horizontal and vertical lines to draw borders
       * @param x The x coordinate of the starting point of the line drawing
       * @param y the y coordinate of the starting point of the line drawing
       * @param h line length
       * @param color line color
       */
      for(int x = 0; x<16 ;x++)
      screen.drawFastVLine(/*x=*/x,/*y=*/0,/*h=*/h,/*color=*/color);
      for(int y = 0; y<16 ;y++)
      screen.drawFastHLine(/*x=*/16,/*y=*/y,/*w=*/w-16*2,/*color=*/color);
      for(int x = w-1; x>=w-16 ;x--)
      screen.drawFastVLine(x,0,h, color);
      for(int y = h-1; y>=h-16 ;y--)
      screen.drawFastHLine(16,y,w-16*2,color);
      /**
       * @brief 画位顺序不同的单色图
       * @param x      0 设置画图起点是屏幕左上方，靠左
       * @param y      0 靠上
       * @param bitmap gImage_XBitmap 头文件XBitmap中的数组，数组元素是单字节
       * @param w      240 图片宽度
       * @param h      240 图片高度
       * @param color  为图片黑色部分设置为颜色
       */ 
      /**
       * @brief monochrome pictures with different drawing order
       * @param x 0 Set the starting point of drawing to be at the upper left of the screen, close to the left
       * @param y 0 up
       * @param bitmap gImage_XBitmap The array in the header file XBitmap, the array elements are single bytes
       * @param w 240 picture width
       * @param h 240 Picture height
       * @param color is the color of the black part of the picture
       */
      screen.drawXBitmap(/*x=*/w/2-48,/*y=*/h/2-16,/*bitmap gImage_Bitmap=*/gImage[i],/*w=*/32,/*h=*/32,color+=0x0700);
      //Delay 1 second
      delay(1000);
  
      screen.drawXBitmap(/*x=*/w/2-16,/*y=*/h/2-16,/*bitmap gImage_Bitmap=*/gImage[i+1],/*w=*/32,/*h=*/32,color+=0x0700);
      delay(1000);
      
      screen.drawXBitmap(/*x=*/w/2+16,/*y=*/h/2-16,/*bitmap gImage_Bitmap=*/gImage[i+2],/*w=*/32,/*h=*/32,color+=0x0700);
      delay(1000);

    }
  }
  

}
