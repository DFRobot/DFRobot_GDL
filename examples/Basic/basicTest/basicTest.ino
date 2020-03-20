/*!
 * @file basicTest.ino
 * @brief 演示各种图形绘画效果 
 * @n 本示例支持的主板有Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [LuoYufeng](yufeng.luo@dfrobot.com)
 * @version  V0.1
 * @date  2020-01-07
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
 
#include "DFRobot_GDL.h"
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
/*AVR系列主板*/
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
DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/*M0主板下DMA传输*/
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


/*
 *可供用户选择的宏定义颜色
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */
 
void setup() {
  Serial.begin(115200);
  screen.begin();//生成了screen对象
}

void loop(){
    testDrawPixel();
    testLine();
    testFastLines(COLOR_RGB565_PURPLE,COLOR_RGB565_YELLOW);       
    testRects(COLOR_RGB565_BLACK,COLOR_RGB565_WHITE);
    testRoundRects();
    testCircles(24,COLOR_RGB565_BLUE);
    testTriangles(COLOR_RGB565_YELLOW);
    testPrint();

}

/*测试画像素点*/
void testDrawPixel() {
  /*
   *@brief 清屏
   *@param c 屏幕颜色
   */
  screen.fillScreen(COLOR_RGB565_BLACK);
  int x = 0;
  int y = screen.height();
  for(int i = 0; i <= screen.width()/2; i += 10){
    for (x = screen.width() - i; x >= i; x-=10 ){
      /*
       *@brief 画像素点
       *@param x 横坐标
       *       y 纵坐标
       *       c 像素点颜色
       */
      screen.drawPixel(x, y, COLOR_RGB565_ORANGE);
      delay(10);
    }
	
    for (y = screen.height() - i; y >= i; y-=10){
      screen.drawPixel(x, y, COLOR_RGB565_ORANGE);
      delay(10);
    }
	
    for (x = i; x <= screen.width() - i + 1; x+=10 ){
      screen.drawPixel(x, y, COLOR_RGB565_ORANGE);
      delay(10);
    }
	
    for (y = i; y <= screen.height() - i + 1; y+=10){
      screen.drawPixel(x, y, COLOR_RGB565_ORANGE);
      delay(10);
    }
  }
}

/*测试画线*/
void testLine(){
	 //0x00FF 是格式为RGB565的颜色数据
  uint16_t color = 0x00FF;
  screen.fillScreen(COLOR_RGB565_BLACK);
  for (int16_t x=0; x < screen.width(); x+=6) {
    /*
     *@brief 画线段
     *@param x0 第一个顶点横坐标
     *       y0 第一个顶点纵坐标
     *       x1 第二个顶点横坐标
     *       y1 第二个顶点纵坐标
     *       c 线段颜色
     */
    screen.drawLine(/*x0=*/screen.width()/*屏幕宽度*//2, /*y0=*/screen.height()/*屏幕高度*//2, /*x1=*/x, /*y1=*/0, /*c=*/color+=0x0700);
  }
  for (int16_t y=0; y < screen.height(); y+=6) {
    screen.drawLine(screen.width()/2, screen.height()/2, screen.width(), y, color+=0x0700);
  }
 
  for (int16_t x = screen.width(); x >= 0; x-=6) {
    screen.drawLine(screen.width()/2, screen.height()/2, x,screen.height(), color+=0x0700);
  }
  
  for (int16_t y = screen.height(); y >= 0; y-=6) {
    screen.drawLine(screen.width()/2, screen.height()/2, 0, y, color+=0x0700);
  }
}

/*测试快速画线(需设置延时)，只有横线和纵线*/
void testFastLines(uint16_t color1, uint16_t color2) {
  for (int16_t y=0; y < screen.height(); y+=4) {
    /*
     *@brief 画线段
     *@param x 第一个顶点横坐标
     *       y 第一个顶点纵坐标
     *       w 线段的长度
     *       c 线段颜色
     */
    screen.drawFastHLine(/*x=*/0, /*y=*/y, /*w=*/screen.width(),/*c=*/color2);
    delay(10);
  }
  
  for(int16_t x=0; x < screen.width(); x+=3) {
    /*
     *@brief 画线段
     *@param x 第一个顶点横坐标
     *       y 第一个顶点纵坐标
     *       h 线段的长度
     *       c 线段颜色
     */
    screen.drawFastVLine(/*x=*/x, /*y=*/0, /*h=*/screen.height(), /*c=*/color1);
    delay(10);
  }
}

/*测试画矩形*/
void testRects(uint16_t color1, uint16_t color2) { 
    screen.fillScreen(COLOR_RGB565_BLACK);
    int16_t x=screen.width()-12;
    for (; x > 100; x-=screen.width()/40) {
      /*
       *@brief 画空心矩形
       *@param x 顶点横坐标
       *@param y 顶点纵坐标
       *@param w 横向边长
       *@param h 纵向边长
       *@param color 填充颜色，565结构的RGB色
       */
      screen.drawRect(/*x=*/screen.width()/2 -x/2, /*y=*/screen.height()/2 -x/2 , /*w=*/x, /*h=*/x, /*color=*/color2+=0x0F00);
      delay(100);
    }
	
    /*
     *@brief 画填充矩形
     *@param x 顶点横坐标
     *@param y 顶点纵坐标
     *@param w 横向边长
     *@param h 纵向边长
     *@param color 填充颜色，565结构的RGB色
    */
    screen.fillRect(/*x=*/screen.width()/2 -x/2, /*y=*/screen.height()/2 -x/2 , /*w=*/x, /*h=*/x, /*color=*/color2);
    delay(100);
    for(; x > 6; x-=screen.width()/40){
      screen.drawRect(screen.width()/2 -x/2, screen.height()/2 -x/2 , x, x, color1);
      delay(100);
    }
}

/*测试画圆角矩形*/
void testRoundRects() {
  screen.fillScreen(COLOR_RGB565_BLACK);
   //0xF00F 是格式为RGB565的颜色数据
  int color = 0xF00F;
  int i;
  int x = 0;
  int y = 0;
  int w = screen.width()-3;
  int h = screen.height()-3;
  for(i = 0 ; i <= 16; i+=2) {
    /*
     *@brief 画空心圆角矩形
     *@param x0 起始顶点横坐标
     *@param y0 起始顶点纵坐标
     *@param w 横向边长
     *@param h 纵向边长
     *@param radius 圆角半径
     *@param color 边框颜色，565结构的RGB色
     */
    screen.drawRoundRect(/*x0=*/x, /*y0=*/y, /*w=*/w, /*h=*/h, /*radius=*/20, /*color=*/color);
    x+=5;
    y+=5;
    w-=10;
    h-=10;
    color+=0x0100;
    delay(50);
  }
  for(i = 0 ; i <= 16; i+=2) {
    /*
     *@brief 画填充圆角矩形
     *@param x0 起始顶点横坐标
     *@param y0 起始顶点纵坐标
     *@param w 横向边长
     *@param h 纵向边长
     *@param radius 圆角半径
     *@param color 填充颜色，565结构的RGB色
     */
    screen.fillRoundRect(/*x0=*/x, /*y0=*/y, /*w=*/w, /*h=*/h, /*radius=*/10, /*color=*/color);
    x+=5;
    y+=5;
    w-=10;
    h-=10;
    color+=0x0500;
    delay(50);
  }
}

/*测试画圆*/
void testCircles(uint8_t radius, uint16_t color) {
  screen.fillScreen(COLOR_RGB565_BLACK);
  for (int16_t x=radius; x <=screen.width()-radius; x+=radius*2) {
    for (int16_t y=radius; y <=screen.height()-radius; y+=radius*2) {
      /*
       *@brief 画空心圆
       *@param x0 圆心横坐标
       *@param y0 圆心纵坐标
       *@param r 半径
       *@param color 圆周颜色，565结构的RGB色
       */
      screen.drawCircle(/*x0=*/x, /*y0=*/y, /*r=*/radius, /*color=*/color);
        if(x == y ||x == -y ||x == y + 2*radius)
          /*
           *@brief 画填充圆
           *@param x0 圆心横坐标
           *@param y0 圆心纵坐标
           *@param r 半径
           *@param color 填充颜色，565结构的RGB色
           */
          screen.fillCircle(/*x0=*/x, /*y0=*/y, /*r=*/radius, /*color=*/color);
       color += 800;
       delay(100);
    }
  }
}

/*测试画三角形*/
void testTriangles(uint16_t color){
  screen.fillScreen(COLOR_RGB565_BLACK);
  
  for (int16_t i=0; i <=screen.width(); i+=24)
    /*
     *@brief 画空心三角形
     *@param x0 起始顶点横坐标
     *@param y0 起始顶点纵坐标
     *@param x1 第二个顶点横坐标
     *@param y1 第二个顶点纵坐标
     *@param x2 第三个顶点横坐标
     *@param y2 第三个顶点纵坐标
     *@param color 边框颜色，565结构的RGB色
     */
    screen.drawTriangle(/*x0=*/i,/*y0=*/0,/*x1=*/0,/*y1=*/screen.height()-i,/*x2=*/screen.width()-i,/*y2=*/screen.height(), /*color=*/color);
	
  for (int16_t i=0; i <screen.width(); i+=24)
    screen.drawTriangle(screen.width(),i*4/3,0,screen.height()-i*4/3,i,0, color);

  for (int16_t i=0; i <screen.width(); i+=24)
    screen.drawTriangle(screen.width(),i*4/3,i,0,screen.width()-i,screen.height(), color);

  color = COLOR_RGB565_RED;
  for (int16_t i=0; i <=screen.width(); i+=24)
    /*
     *@brief 画填充三角形
     *@param x0 起始顶点横坐标
     *@param y0 起始顶点纵坐标
     *@param x1 第二个顶点横坐标
     *@param y1 第二个顶点纵坐标
     *@param x2 第三个顶点横坐标
     *@param y2 第三个顶点纵坐标
     *@param color 填充颜色，565结构的RGB色
     */
    screen.fillTriangle(/*x0=*/i,/*y0=*/0,/*x1=*/0,/*y1=*/screen.height()-i,/*x2=*/screen.width()-i,/*y2=*/screen.height(), /*color=*/color+=100);
	
  for (int16_t i=0; i <screen.width(); i+=24)
    screen.fillTriangle(screen.width(),i*4/3,0,screen.height()-i*4/3,i,0, color+=100);

  for (int16_t i=0; i <screen.width(); i+=24)
    screen.fillTriangle(screen.width(),i*4/3,i,0,screen.width()-i,screen.height(), color+=100);
}

void testPrint() {
 //0x00FF 是格式为RGB565的颜色数据
  int16_t color = 0x00FF;
  //设置文本自动换行模式
  //true=文本自动换行，false=不自动换行
  screen.setTextWrap(false);
  //填充颜色，565结构的RGB色
  screen.fillScreen(COLOR_RGB565_BLACK);
  
  //设置坐标位置x=0,y=50
  screen.setCursor(0, 50);
  //设置文本颜色;这是变化的值
  screen.setTextColor(color+=0x3000);
  //设置文本大小为0
  screen.setTextSize(0);
  //输出文本
  screen.println("Hello World!");
  
  screen.setTextColor(color+=0x3000);
  //设置文本大小为1
  screen.setTextSize(1);
  screen.println("Hello World!");
  
  screen.setTextColor(color+=0x3000);
  //设置文本大小为2
  screen.setTextSize(2);
  screen.println("Hello World!");
  
  screen.setTextColor(color+=0x3000);
  //设置文本大小为3
  screen.setTextSize(3);
  screen.println("Hello World!");
  
  screen.setTextColor(color+=0x3000);
  //设置文本大小为4
  screen.setTextSize(4);
  screen.println("Hello!");
  //设置文本大小为5
  screen.setTextSize(5);
  screen.print("Hello!");
  delay(2000);
  
  //设置坐标位置x=0,y=0
  screen.setCursor(0, 0);
  //填充颜色，565结构的RGB色
  screen.fillScreen(COLOR_RGB565_BLACK);
  screen.setTextSize(2);
  screen.setTextColor(color+=0x3000);
  screen.print("a = ");
  
  screen.setTextColor(color+=0x3000);
  int a = 1234;
  screen.println(a, 1);
  screen.setTextColor(color+=0x3000);
  screen.print(8675309, HEX);
  screen.println("this is HEX!");
  screen.println("");
  
  screen.setTextColor(color+=0x0F00);
  screen.println("running for: ");
  screen.setTextColor(color+=0x0F00);
  //输出毫秒时间
  screen.print(millis());
  screen.setTextColor(color+=0x0F00);
  screen.println("/1000 seconds.");
  
  char *text = "Hi DFRobot!";
  screen.setTextColor(color+=0x0F00);
  screen.setTextWrap(true);
  screen.setTextSize(3);
  screen.println(text);
  //screen.setFonts((const gdl_Font_t *)SIMKAIFont18ptBitmaps);
  screen.println(text);
  delay(2000);
}
