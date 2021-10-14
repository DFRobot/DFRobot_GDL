/*!
 * @file ballCollision.ino
 * @brief Present the animation effect of balls moving and colliding in the screen  
 * @n The demo supports Arduino Uno, Leonardo, Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, FireBeetle-M0
 * @copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V0.1
 * @date  2021-04-13
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
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
/*AVR series board*/
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */

DFRobot_ST7735_80x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */

//DFRobot_ST7735_80x160_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

/*
 *User-selectable macro definition color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY     
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE   
 */

int xx0 = 39;int yy0 = 99;int xx1 = 199;int yy1 = 29;int xx2 = 99;int yy2 = 59;
int xx3 = 5;int yy3 = 5;int xx4 = 5;int yy4 = 5;int xx5 = 5;int yy5 = 5;
uint16_t color1 = COLOR_RGB565_YELLOW;
uint16_t color2 = COLOR_RGB565_RED;
uint16_t color3 = COLOR_RGB565_BLUE;
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    screen.begin();
    /*
     * @ brief Clear screen
     * @ param c Screen color
     */
    screen.fillScreen(COLOR_RGB565_BLACK);
    //Painted solid circle 1
    screen.fillCircle(xx0,yy0,10,color1);
    //Painted solid circle 2
    screen.fillCircle(xx1,yy1,10,color2);
    //Painted solid circle 3
    screen.fillCircle(xx2,yy2,10,color3);
}

void loop() {
    screen.fillCircle(xx0,yy0,10,COLOR_RGB565_BLACK);
    screen.fillCircle(xx1,yy1,10,COLOR_RGB565_BLACK);
    screen.fillCircle(xx2,yy2,10,COLOR_RGB565_BLACK);
    int r1 = (yy0-yy1)*(yy0-yy1) + (xx0-xx1)*(xx0-xx1);
    int r2 = (yy0-yy2)*(yy0-yy2) + (xx0-xx2)*(xx0-xx2);
    int r3 = (yy2-yy1)*(yy2-yy1) + (xx2-xx1)*(xx2-xx1);
    if(r1 <= 400){
        int t = 0;
        if(fabs(yy0-yy1)<fabs(xx0-xx1)){
            t = xx3;xx3 = xx4;xx4 =t;
        }
        else if(fabs(yy0-yy1)>fabs(xx0-xx1)){
            t = yy3;yy3 = yy4;yy4 =t;
        }
        else{
            t = yy3;yy3 = yy4;yy4 =t;
            t = xx3;xx3 = xx4;xx4 =t;
        }
    }
    if(r2 <= 400){
        int t;
        if(fabs(yy0-yy2)<fabs(xx0-xx2)){
            t = xx3;xx3 = xx5;xx5 =t;
        }
        else if(fabs(yy0-yy2)>fabs(xx0-xx2)){
            t = yy3;yy3 = yy5;yy5 =t;
        }
        else{
            t = yy3;yy3 = yy5;yy5 =t;
            t = xx3;xx3 = xx5;xx5 =t;
        }
    }
    if(r3 <= 400){
        int t;
        if(fabs(yy2-yy1)<fabs(xx2-xx1)){
            t = xx5;xx5 = xx4;xx4 =t;
        }
        else if(fabs(yy2-yy1)>fabs(xx2-xx1)){
            t = yy5;yy5 = yy4;yy4 =t;
        }
        else{
            t = yy5;yy5 = yy4;yy4 =t;
            t = xx5;xx5 = xx4;xx4 =t;
        }
    }
    if(yy0<=9)
        yy3 = 5;
    else if(yy0>=(screen.height()-10))
        yy3 = -5;
    if(xx0<=9)
        xx3 = 5;
    else if(xx0>=(screen.width()-10))
        xx3 = -5;
    if(yy1<=9)
        yy4 = 5;
    else if(yy1>=(screen.height()-10))
        yy4 = -5;
    if(xx1<=9)
        xx4 = 5;
    else if(xx1>=(screen.width()-10))
        xx4 = -5;
    if(yy2<=9)
        yy5 = 5;
    else if(yy2>=(screen.height()-10))
        yy5 = -5;
    if(xx2<=9)
        xx5 = 5;
    else if(xx2>=(screen.width()-10))
        xx5 = -5;
    yy0 += yy3;xx0 += xx3;
    yy1 += yy4;xx1 += xx4;
    yy2 += yy5;xx2 += xx5;
    
    screen.fillCircle(xx0,yy0,10,color1);
    screen.fillCircle(xx1,yy1,10,color2);
    screen.fillCircle(xx2,yy2,10,color3);
    delay(100);
}
