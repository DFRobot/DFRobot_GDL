/*!
 * @file basicTest.ino
 * @brief Demonstrate various graphic painting effects
 * @n This demo supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, and FireBeetle-M0.
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [LuoYufeng] (yufeng.luo@dfrobot.com)
 * @version V0.1
 * @date 2020-01-07
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include "DFRobot_GDL.h"
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
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */
//DFRobot_ST7789_240x204_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
//DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ST7789_240x204_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST,/*bl=*/TFT_BL);
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

unsigned long total = 0;
unsigned long tn = 0;
/*
 *User-selectable macro definition color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */
 boolean initial = 1;
void setup() {
  Serial.begin(115200);
  screen.begin();
  screen.fillScreen(COLOR_RGB565_LGRAY);
  screen.setTextSize(2);
  screen.println(""); screen.println("");
  screen.println(" gdl library Test!"); 
  delay(1000);
}





void loop(void){
  
  uint32_t usecFillScreen = testFillScreen();
  uint32_t usecText = testText();
  uint32_t usecPixels = testPixels();
  uint32_t usecLines = testLines(COLOR_RGB565_BLUE);
  uint32_t usecFastLines = testFastLines(COLOR_RGB565_RED, COLOR_RGB565_BLUE);
  uint32_t usecRects = testRects(COLOR_RGB565_GREEN);
  uint32_t usecFilledRects = testFilledRects(COLOR_RGB565_YELLOW, COLOR_RGB565_MAGENTA);
  uint32_t usecFilledCircles = testFilledCircles(10, COLOR_RGB565_MAGENTA);
  uint32_t usecCircles = testCircles(10, COLOR_RGB565_WHITE);
  uint32_t usecTriangles = testTriangles();
  uint32_t usecFilledTrangles = testFilledTriangles();
  uint32_t usecRoundRects = testRoundRects();
  uint32_t usedFilledRoundRects = testFilledRoundRects();
  
  /*
  uint32_t usecFillScreen ;
  uint32_t usecText ;
  uint32_t usecPixels ;
  uint32_t usecLines ;
  uint32_t usecFastLines ;
  uint32_t usecRects ;
  uint32_t usecFilledRects;
  uint32_t usecFilledCircles ;
  uint32_t usecCircles ;
  uint32_t usecTriangles ;
  uint32_t usecFilledTrangles ;
  uint32_t usecRoundRects ;
  uint32_t usedFilledRoundRects;
  */
  
  screen.setTextColor(COLOR_RGB565_MAGENTA);
  screen.setTextSize(2);
  screen.setCursor(30, 60);
  screen.println(F("GDL test"));

  screen.setTextSize(1);
  screen.setTextColor(COLOR_RGB565_WHITE);
  screen.println(F(""));
  screen.setTextSize(1);
  screen.println(F(""));
  screen.setTextColor(screen.rgb888ToRGB565(0x80, 0x80, 0x80));

  screen.print(F(""));


  screen.setTextColor(COLOR_RGB565_GREEN);
  screen.println(F(""));
 

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Screen fill          "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecFillScreen);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  
  screen.print(F("Text                 "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecText);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Pixels                  "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecPixels);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Lines                   "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecLines);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Horiz/Vert Lines     "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecFastLines);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Rectangles           "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecRects);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Rectangles-filled      "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecFilledRects);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Circles                "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecCircles);


  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Circles-filled         "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecFilledCircles);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Triangles              "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecTriangles);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Triangles-filled       "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecFilledTrangles);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Rounded rects        "));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  printnice(usecRoundRects);

  screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.print(F("Rounded rects-fill     ")); 
  screen.setTextColor(COLOR_RGB565_YELLOW); 
  printnice(usedFilledRoundRects);

  screen.setTextSize(1);
   screen.setTextColor(COLOR_RGB565_CYAN); screen.setTextSize(1);
  screen.println(F(""));
  screen.setTextColor(COLOR_RGB565_YELLOW);
  screen.print(F("              Complete!"));

  delay(60 * 1000L);
  while(1);
  
}




void printnice(int32_t v)
{
  char  str[32] = { 0 };
  sprintf(str, "%lu", v);
  for (char *p = (str+strlen(str))-3; p > str; p -= 3)
  {
    memmove(p+1, p, strlen(p)+1);
    *p = ',';
    
  }
  while (strlen(str) < 10)
  {
    memmove(str+1, str, strlen(str)+1);
    *str = ' ';
  }
  screen.println(str);
}

static inline uint32_t micros_start() __attribute__ ((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms)
    ;
  return micros();
}


uint32_t testFillScreen()
{
  uint32_t start = micros_start();
    // Shortened this tedious test!
    screen.fillScreen(COLOR_RGB565_WHITE);
    screen.fillScreen(COLOR_RGB565_RED);
    screen.fillScreen(COLOR_RGB565_GREEN);
    screen.fillScreen(COLOR_RGB565_BLUE);
    screen.fillScreen(COLOR_RGB565_BLACK);

  return (micros() - start)/5;
}

uint32_t testText()
{
  screen.fillScreen(COLOR_RGB565_BLACK);
  uint32_t start = micros_start();
  screen.setCursor(0, 0);
  screen.setTextColor(COLOR_RGB565_WHITE,COLOR_RGB565_BLACK);  screen.setTextSize(1);
  screen.println(F("Hello World!"));
  screen.setTextSize(2);
  screen.setTextColor(screen.rgb888ToRGB565(0xff, 0x00, 0x00));
  screen.print(F("RED "));
  screen.setTextColor(screen.rgb888ToRGB565(0x00, 0xff, 0x00));
  screen.print(F("GREEN "));
  screen.setTextColor(screen.rgb888ToRGB565(0x00, 0x00, 0xff));
  screen.println(F("BLUE"));
  screen.setTextColor(COLOR_RGB565_YELLOW); screen.setTextSize(2);
  screen.println(1234.56);
  screen.setTextColor(COLOR_RGB565_RED);    screen.setTextSize(3);
  screen.println(0xDEADBEEF, HEX);
  screen.println();
  screen.setTextColor(COLOR_RGB565_GREEN);
  screen.setTextSize(5);
  screen.println(F("Groop"));
  screen.setTextSize(2);
  screen.println(F("I implore thee,"));
  screen.setTextColor(COLOR_RGB565_GREEN);
  screen.setTextSize(1);
  screen.println(F("my foonting turlingdromes."));
  screen.println(F("And hooptiously drangle me"));
  screen.println(F("with crinkly bindlewurdles,"));
  screen.println(F("Or I will rend thee"));
  screen.println(F("in the gobberwarts"));
  screen.println(F("with my blurglecruncheon,"));
  screen.println(F("see if I don't!"));
  screen.println(F(""));
  screen.println(F(""));
  screen.setTextColor(COLOR_RGB565_MAGENTA);
  screen.setTextSize(6);
  screen.println(F("Woot!"));
  uint32_t t = micros() - start;
  delay(1000);
  return t;
}

uint32_t testPixels()
{
  int32_t w = screen.width();
  int32_t h = screen.height();

  uint32_t start = micros_start();
  screen.startWrite();
  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w; x++)
    {
      screen.drawPixel(x, y, screen.rgb888ToRGB565(x<<3, y<<3, x*y));
    }
  }
  screen.endWrite();
  return micros() - start;
}


uint32_t testLines(uint16_t color)
{
  uint32_t start, t;
  int32_t x1, y1, x2, y2;
  int32_t w = screen.width();
  int32_t h = screen.height();

  screen.fillScreen(COLOR_RGB565_BLACK);

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }

  t = micros() - start; // fillScreen doesn't count against timing

  screen.fillScreen(COLOR_RGB565_BLACK);

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  screen.fillScreen(COLOR_RGB565_BLACK);

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }
  t += micros() - start;

  screen.fillScreen(COLOR_RGB565_BLACK);

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    screen.drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
  uint32_t start;
  int32_t x, y;
  int32_t w = screen.width();
  int32_t h = screen.height();

  screen.fillScreen(COLOR_RGB565_BLACK);

  start = micros_start();

  for (y = 0; y < h; y += 5)
    screen.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5)
    screen.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

uint32_t testRects(uint16_t color)
{
  uint32_t start;
  int32_t n, i, i2;
  int32_t cx = screen.width() / 2;
  int32_t cy = screen.height() / 2;

  screen.fillScreen(COLOR_RGB565_BLACK);
  n = min(screen.width(), screen.height());
  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    screen.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}

uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
  uint32_t start, t = 0;
  int32_t n, i, i2;
  int32_t cx = screen.width() / 2 - 1;
  int32_t cy = screen.height() / 2 - 1;

  screen.fillScreen(COLOR_RGB565_BLACK);
  n = min(screen.width(), screen.height());
  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    start = micros_start();

    screen.fillRect(cx-i2, cy-i2, i, i, color1);

    t += micros() - start;

    // Outlines are not included in timing results
    screen.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, w = screen.width(), h = screen.height(), r2 = radius * 2;

  screen.fillScreen(COLOR_RGB565_BLACK);

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      screen.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w = screen.width() + radius;
  int32_t h = screen.height() + radius;

  // Screen is not cleared for this one -- this is
  // intentional and does not affect the reported time.
  start = micros_start();

  for (x = 0; x < w; x += r2)
  {
    for (y = 0; y < h; y += r2)
    {
      screen.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testTriangles()
{
  uint32_t start;
  int32_t n, i;
  int32_t cx = screen.width()/ 2 - 1;
  int32_t cy = screen.height() / 2 - 1;

  screen.fillScreen(COLOR_RGB565_BLACK);
  n = min(cx, cy);

  start = micros_start();

  for (i = 0; i < n; i += 5)
  {
    screen.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      screen.rgb888ToRGB565(0, 0, i));
  }

  return micros() - start;
}

uint32_t testFilledTriangles()
{
  uint32_t start, t = 0;
  int32_t i;
  int32_t cx = screen.width() / 2 - 1;
  int32_t cy = screen.height() / 2 - 1;

  screen.fillScreen(COLOR_RGB565_BLACK);

  start = micros_start();

  for (i = min(cx,cy); i > 10; i -= 5) {
    start = micros_start();
    screen.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      screen.rgb888ToRGB565(0, i, i));
    t += micros() - start;
    screen.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      screen.rgb888ToRGB565(i, i, 0));
  }

  return t;
}

uint32_t testRoundRects()
 {
  uint32_t start;
  int32_t w, i, i2;
  int32_t cx = screen.width() / 2 - 1;
  int32_t cy = screen.height() / 2 - 1;

  screen.fillScreen(COLOR_RGB565_BLACK);
  
  w = min(screen.width(), screen.height());
  
  start = micros_start();

  for (i = 0; i < w; i += 6)
  {
    i2 = i / 2;
    screen.drawRoundRect(cx-i2, cy-i2, i, i, i/8, screen.rgb888ToRGB565(i, 0, 0));
  }

  return micros() - start;
}

uint32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;
  int32_t cx = screen.width() / 2 - 1;
  int32_t cy = screen.height() / 2 - 1;

  screen.fillScreen(COLOR_RGB565_BLACK);

  start = micros_start();

  for (i = min(screen.width(), screen.height()); i > 20; i -= 6)
  {
    i2 = i / 2;
    screen.fillRoundRect(cx-i2, cy-i2, i, i, i/8, screen.rgb888ToRGB565(0, i, 0));
  }

  return micros() - start;
}
