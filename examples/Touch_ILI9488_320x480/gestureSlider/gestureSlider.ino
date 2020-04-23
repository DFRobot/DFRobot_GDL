/*!
 * @file gestureSlider.ino
 * @brief Gesture recognition example. Make the following gestures on the screen and the graphics of the 
 * @n corresponding gestures will be drawn on the screen: one finger slides up, down, left and right; two fingers slide up and down, 
 * @n left and right; three fingers slide up and down, left and right, and right. 
 * @n The demo supports Mega2560, FireBeetle-ESP32, FireBeetle-ESP8266, and FireBeetle-M0.
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
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#endif

/**
   @brief Constructor  When the touch uses the gt series chip, you can call this constructor
*/
DFRobot_Touch_GT911 touch;


/**
   @brief Constructor When the screen uses hardware SPI communication, the driver IC is st7789, and the screen resolution is 240x320, this constructor can be called
   @param dc Command/data line pin for SPI communication
   @param cs Chip select pin for SPI communication
   @param rst Reset pin of the screen
*/
DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);



/**
   @brief Constructor
   @param gdl Screen object
   @param touch Touch object
*/
DFRobot_UI ui(&screen, &touch);

char font[] = "gesture";

void setup()
{

  Serial.begin(9600);
  //UI initialization
  ui.begin();
  //Draw string
  ui.drawString((screen.width()-strlen(font)*2*6)/2, (screen.height()-8)/2, font, COLOR_RGB565_LGRAY,  ui.bgColor, 2, 1);
}


void loop()
{
//refresh
  refresh();
}



void  refresh() {
  //Function to detect gestures on the screen, return the gesture detected by the touch screen
  DFRobot_UI:: eGesture_t gesture = ui.getGestures();
  
  switch (gesture) {
    case ui.DUPGLIDE : {
       /**
        * @brief Gesture drawing function
        * @param dir Gesture direction
        * The number of fingers in @param obj pointnum
        */
        drawT(/*Gesture direction=*/0, /*The number of fingers in the gesture=*/2);
      } break;
    case ui.DRIGHTGLIDE : {
        drawT(1, 2);
      } break;
    case ui.DDOWNGLIDE : {
        drawT(2, 2);
      } break;
    case ui.DLEFTGLIDE : {
        drawT(3, 2);
      } break;
    case ui.SUPGLIDE : {
        drawT(0, 1);
      } break;
    case ui.SRIGHTGLIDE : {
        drawT(1, 1);
      } break;
    case ui.SDOWNGLIDE : {
        drawT(2, 1);
      } break;
    case ui.SLEFTGLIDE : {
        drawT(3, 1);
      } break;
    case ui.TUPGLIDE : {
        drawT(0, 3);
      } break;
    case ui.TRIGHTGLIDE : {
        drawT(1, 3);
      } break;
    case ui.TDOWNGLIDE : {
        drawT(2, 3);
      } break;
    case ui.TLEFTGLIDE : {
        drawT(3, 3);
      } break;
    case ui.DWROTATE : {
        drawT(10, 0);
      } break;
    case ui.DCWROTATE : {
        drawT(10, 1);
      } break;
    case ui.NONE: {
        return;
      }
  }

}



/**
 * @brief Gesture drawing function
 * @param dir Gesture direction
 * The number of fingers in @param obj pointnum
 */
void drawT(uint8_t dir , uint8_t pointnum) {
  if (dir == 0 && pointnum == 1) {
    screen.fillTriangle(screen.width() / 2, screen.height() / 20, screen.width() / 2 - 15, screen.height() / 20 + 24, screen.width() / 2 + 15, screen.height() / 20 + 24, COLOR_RGB565_RED);
    screen.fillRect(screen.width() / 2 - 8, screen.height() / 20 + 24, 16, 70, COLOR_RGB565_RED);
    delay(500);
    screen.fillTriangle(screen.width() / 2, screen.height() / 20, screen.width() / 2 - 15, screen.height() / 20 + 24, screen.width() / 2 + 15, screen.height() / 20 + 24, ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8, screen.height() / 20 + 24, 16, 70, ui.bgColor);
  }
  else if (dir == 1 && pointnum == 1) {
    screen.fillTriangle(screen.width() - 20, screen.height() / 2, screen.width() - 50, screen.height() / 2 - 15, screen.width() - 50, screen.height() / 2 + 15, COLOR_RGB565_RED);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8, 70, 16, COLOR_RGB565_RED);
    delay(500);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2, screen.width() - 50, screen.height() / 2 - 15, screen.width() - 50, screen.height() / 2 + 15, ui.bgColor);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8, 70, 16, ui.bgColor);

  }
  else if (dir == 2 && pointnum == 1) {
    screen.fillTriangle(screen.width() / 2, screen.height() - 50, screen.width() / 2 - 15, screen.height() - 50 - 30, screen.width() / 2 + 15, screen.height() - 50 - 30, COLOR_RGB565_RED);
    screen.fillRect(screen.width() / 2 - 8, screen.height() - 50 - 30 - 70, 16, 70, COLOR_RGB565_RED);
    delay(500);
    screen.fillTriangle(screen.width() / 2, screen.height() - 50, screen.width() / 2 - 15, screen.height() - 50 - 30, screen.width() / 2 + 15, screen.height() - 50 - 30, ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8, screen.height() - 50 - 30 - 70, 16, 70, ui.bgColor);
  }
  else if (dir == 3 && pointnum == 1) {
    screen.fillTriangle(20, screen.height() / 2, 20 + 30, screen.height() / 2 - 15, 20 + 30, screen.height() / 2 + 15, COLOR_RGB565_RED);
    screen.fillRect(20 + 30, screen.height() / 2 - 8, 70, 16, COLOR_RGB565_RED);
    delay(500);
    screen.fillTriangle(20, screen.height() / 2, 20 + 30, screen.height() / 2 - 15, 20 + 30, screen.height() / 2 + 15, ui.bgColor);
    screen.fillRect(20 + 30, screen.height() / 2 - 8, 70, 16, ui.bgColor);
  }
  else if (dir == 0 && pointnum == 2) {
    screen.fillTriangle(screen.width() / 2 - 15, screen.height() / 20, screen.width() / 2 - 15 - 15, screen.height() / 20 + 24, screen.width() / 2 + 15 - 15, screen.height() / 20 + 24, COLOR_RGB565_GREEN);
    screen.fillRect(screen.width() / 2 - 8 - 15, screen.height() / 20 + 24, 16, 70, COLOR_RGB565_GREEN);

    screen.fillTriangle(screen.width() / 2 + 15, screen.height() / 20, screen.width() / 2 - 15 + 15, screen.height() / 20 + 24, screen.width() / 2 + 15 + 15, screen.height() / 20 + 24, COLOR_RGB565_GREEN);
    screen.fillRect(screen.width() / 2 - 8 + 15, screen.height() / 20 + 24, 16, 70, COLOR_RGB565_GREEN);
    delay(500);
    screen.fillTriangle(screen.width() / 2 - 15, screen.height() / 20, screen.width() / 2 - 15 - 15, screen.height() / 20 + 24, screen.width() / 2 + 15 - 15, screen.height() / 20 + 24, ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 - 15, screen.height() / 20 + 24, 16, 70, ui.bgColor);

    screen.fillTriangle(screen.width() / 2 + 15, screen.height() / 20, screen.width() / 2 - 15 + 15, screen.height() / 20 + 24, screen.width() / 2 + 15 + 15, screen.height() / 20 + 24, ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 + 15, screen.height() / 20 + 24, 16, 70, ui.bgColor);

  }
  else if (dir == 1 && pointnum == 2) {
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 - 15, screen.width() - 50, screen.height() / 2 - 15 - 15, screen.width() - 50, screen.height() / 2 + 15 - 15, COLOR_RGB565_GREEN);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 - 15, 70, 16, COLOR_RGB565_GREEN);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 + 15, screen.width() - 50, screen.height() / 2 - 15 + 15, screen.width() - 50, screen.height() / 2 + 15 + 15, COLOR_RGB565_GREEN);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 + 15, 70, 16, COLOR_RGB565_GREEN);
    delay(500);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 - 15, screen.width() - 50, screen.height() / 2 - 15 - 15, screen.width() - 50, screen.height() / 2 + 15 - 15,  ui.bgColor);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 - 15, 70, 16,  ui.bgColor);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 + 15, screen.width() - 50, screen.height() / 2 - 15 + 15, screen.width() - 50, screen.height() / 2 + 15 + 15,  ui.bgColor);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 + 15, 70, 16,  ui.bgColor);
  }
  else if (dir == 2 && pointnum == 2) {
    screen.fillTriangle(screen.width() / 2 - 15, screen.height() - 50, screen.width() / 2 - 15 - 15, screen.height() - 50 - 30, screen.width() / 2 + 15 - 15, screen.height() - 50 - 30, COLOR_RGB565_GREEN);
    screen.fillRect(screen.width() / 2 - 8 - 15, screen.height() - 50 - 30 - 70, 16, 70, COLOR_RGB565_GREEN);
    screen.fillTriangle(screen.width() / 2 + 15, screen.height() - 50, screen.width() / 2 - 15 + 15, screen.height() - 50 - 30, screen.width() / 2 + 15 + 15, screen.height() - 50 - 30, COLOR_RGB565_GREEN);
    screen.fillRect(screen.width() / 2 - 8 + 15, screen.height() - 50 - 30 - 70, 16, 70, COLOR_RGB565_GREEN);
    delay(500);
    screen.fillTriangle(screen.width() / 2 - 15, screen.height() - 50, screen.width() / 2 - 15 - 15, screen.height() - 50 - 30, screen.width() / 2 + 15 - 15, screen.height() - 50 - 30, ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 - 15, screen.height() - 50 - 30 - 70, 16, 70, ui.bgColor);
    screen.fillTriangle(screen.width() / 2 + 15, screen.height() - 50, screen.width() / 2 - 15 + 15, screen.height() - 50 - 30, screen.width() / 2 + 15 + 15, screen.height() - 50 - 30, ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 + 15, screen.height() - 50 - 30 - 70, 16, 70, ui.bgColor);
  }
  else if (dir == 3 && pointnum == 2) {
    screen.fillTriangle(20, screen.height() / 2 - 15, 20 + 30, screen.height() / 2 - 15 - 15, 20 + 30, screen.height() / 2 + 15 - 15, COLOR_RGB565_GREEN);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 - 15, 70, 16, COLOR_RGB565_GREEN);
    screen.fillTriangle(20, screen.height() / 2 + 15, 20 + 30, screen.height() / 2 - 15 + 15, 20 + 30, screen.height() / 2 + 15 + 15, COLOR_RGB565_GREEN);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 + 15, 70, 16, COLOR_RGB565_GREEN);
    delay(500);
    screen.fillTriangle(20, screen.height() / 2 - 15, 20 + 30, screen.height() / 2 - 15 - 15, 20 + 30, screen.height() / 2 + 15 - 15, ui.bgColor);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 - 15, 70, 16, ui.bgColor);
    screen.fillTriangle(20, screen.height() / 2 + 15, 20 + 30, screen.height() / 2 - 15 + 15, 20 + 30, screen.height() / 2 + 15 + 15, ui.bgColor);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 + 15, 70, 16, ui.bgColor);
  }
  else if (dir == 10 && pointnum == 0) {
    screen.drawCircleHelper(screen.width() / 2, screen.height() / 2, 100, 10, COLOR_RGB565_GREEN);
    screen.fillTriangle(screen.width() / 2 - 100, screen.height() / 2 - 24, screen.width() / 2 - 100 - 14, screen.height() / 2, screen.width() / 2 - 100 + 14, screen.height() / 2, COLOR_RGB565_GREEN);
    screen.fillTriangle(screen.width() / 2 + 100, screen.height() / 2 + 24, screen.width() / 2 + 100 - 14, screen.height() / 2, screen.width() / 2 + 100 + 14, screen.height() / 2, COLOR_RGB565_GREEN);
    delay(500);
    screen.drawCircleHelper(screen.width() / 2, screen.height() / 2, 100, 10, ui.bgColor);
    screen.fillTriangle(screen.width() / 2 - 100, screen.height() / 2 - 24, screen.width() / 2 - 100 - 14, screen.height() / 2, screen.width() / 2 - 100 + 14, screen.height() / 2,  ui.bgColor);
    screen.fillTriangle(screen.width() / 2 + 100, screen.height() / 2 + 24, screen.width() / 2 + 100 - 14, screen.height() / 2, screen.width() / 2 + 100 + 14, screen.height() / 2,  ui.bgColor);

  }
  else if (dir == 10 && pointnum == 1) {
    screen.drawCircleHelper(screen.width() / 2, screen.height() / 2, 100, 5, COLOR_RGB565_GREEN);
    screen.fillTriangle(screen.width() / 2 - 100, screen.height() / 2 + 24, screen.width() / 2 - 100 - 14, screen.height() / 2, screen.width() / 2 - 100 + 14, screen.height() / 2, COLOR_RGB565_GREEN);
    screen.fillTriangle(screen.width() / 2 + 100, screen.height() / 2 - 24, screen.width() / 2 + 100 - 14, screen.height() / 2, screen.width() / 2 + 100 + 14, screen.height() / 2, COLOR_RGB565_GREEN);
    delay(500);
    screen.drawCircleHelper(screen.width() / 2, screen.height() / 2, 100, 5, ui.bgColor);
    screen.fillTriangle(screen.width() / 2 - 100, screen.height() / 2 + 24, screen.width() / 2 - 100 - 14, screen.height() / 2, screen.width() / 2 - 100 + 14, screen.height() / 2, ui.bgColor);
    screen.fillTriangle(screen.width() / 2 + 100, screen.height() / 2 - 24, screen.width() / 2 + 100 - 14, screen.height() / 2, screen.width() / 2 + 100 + 14, screen.height() / 2, ui.bgColor);

  }
  else if (dir == 0 && pointnum == 3) {
    screen.fillTriangle(screen.width() / 2 - 30, screen.height() / 20, screen.width() / 2 - 15 - 30, screen.height() / 20 + 24, screen.width() / 2 + 15 - 30, screen.height() / 20 + 24, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() / 2 - 8 - 30, screen.height() / 20 + 24, 16, 70, COLOR_RGB565_BLUE);

    screen.fillTriangle(screen.width() / 2 + 30, screen.height() / 20, screen.width() / 2 - 15 + 30, screen.height() / 20 + 24, screen.width() / 2 + 15 + 30, screen.height() / 20 + 24, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() / 2 - 8 + 30, screen.height() / 20 + 24, 16, 70, COLOR_RGB565_BLUE);

    screen.fillTriangle(screen.width() / 2 , screen.height() / 20, screen.width() / 2 - 15 , screen.height() / 20 + 24, screen.width() / 2 + 15 , screen.height() / 20 + 24, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() / 2 - 8, screen.height() / 20 + 24, 16, 70, COLOR_RGB565_BLUE);

    delay(500);
    screen.fillTriangle(screen.width() / 2 - 30, screen.height() / 20, screen.width() / 2 - 15 - 30, screen.height() / 20 + 24, screen.width() / 2 + 15 - 30, screen.height() / 20 + 24,  ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 - 30, screen.height() / 20 + 24, 16, 70,  ui.bgColor);

    screen.fillTriangle(screen.width() / 2 + 30, screen.height() / 20, screen.width() / 2 - 15 + 30, screen.height() / 20 + 24, screen.width() / 2 + 15 + 30, screen.height() / 20 + 24,  ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 + 30, screen.height() / 20 + 24, 16, 70,  ui.bgColor);

    screen.fillTriangle(screen.width() / 2 , screen.height() / 20, screen.width() / 2 - 15 , screen.height() / 20 + 24, screen.width() / 2 + 15 , screen.height() / 20 + 24,  ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8, screen.height() / 20 + 24, 16, 70,  ui.bgColor);

  }
  else if (dir == 1 && pointnum == 3) {
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 - 30, screen.width() - 50, screen.height() / 2 - 15 - 30, screen.width() - 50, screen.height() / 2 + 15 - 30, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 - 30, 70, 16, COLOR_RGB565_BLUE);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 + 30, screen.width() - 50, screen.height() / 2 - 15 + 30, screen.width() - 50, screen.height() / 2 + 15 + 30, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 + 30, 70, 16, COLOR_RGB565_BLUE);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 , screen.width() - 50, screen.height() / 2 - 15 , screen.width() - 50, screen.height() / 2 + 15 , COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 , 70, 16, COLOR_RGB565_BLUE);

    delay(500);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 - 30, screen.width() - 50, screen.height() / 2 - 15 - 30, screen.width() - 50, screen.height() / 2 + 15 - 30, ui.bgColor);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 - 30, 70, 16, ui.bgColor);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 + 30, screen.width() - 50, screen.height() / 2 - 15 + 30, screen.width() - 50, screen.height() / 2 + 15 + 30, ui.bgColor);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 + 30, 70, 16, ui.bgColor);
    screen.fillTriangle(screen.width() - 20, screen.height() / 2 , screen.width() - 50, screen.height() / 2 - 15 , screen.width() - 50, screen.height() / 2 + 15 , ui.bgColor);
    screen.fillRect(screen.width() - 50 - 70, screen.height() / 2 - 8 , 70, 16, ui.bgColor);
  }
  else if (dir == 2 && pointnum == 3) {
    screen.fillTriangle(screen.width() / 2 - 30, screen.height() - 50, screen.width() / 2 - 15 - 30, screen.height() - 50 - 30, screen.width() / 2 + 15 - 30, screen.height() - 50 - 30, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() / 2 - 8 - 30, screen.height() - 50 - 30 - 70, 16, 70, COLOR_RGB565_BLUE);
    screen.fillTriangle(screen.width() / 2 + 30, screen.height() - 50, screen.width() / 2 - 15 + 30, screen.height() - 50 - 30, screen.width() / 2 + 15 + 30, screen.height() - 50 - 30, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() / 2 - 8 + 30, screen.height() - 50 - 30 - 70, 16, 70, COLOR_RGB565_BLUE);
    screen.fillTriangle(screen.width() / 2 , screen.height() - 50, screen.width() / 2 - 15 , screen.height() - 50 - 30, screen.width() / 2 + 15, screen.height() - 50 - 30, COLOR_RGB565_BLUE);
    screen.fillRect(screen.width() / 2 - 8, screen.height() - 50 - 70- 30, 16, 70, COLOR_RGB565_BLUE);
    delay(500);
    screen.fillTriangle(screen.width() / 2 - 30, screen.height() - 50, screen.width() / 2 - 15 - 30, screen.height() - 50 - 30, screen.width() / 2 + 15 - 30, screen.height() - 50 - 30,  ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 - 30, screen.height() - 50 - 30 - 70, 16, 70,  ui.bgColor);
    screen.fillTriangle(screen.width() / 2 + 30, screen.height() - 50, screen.width() / 2 - 15 + 30, screen.height() - 50 - 30, screen.width() / 2 + 15 + 30, screen.height() - 50 - 30,  ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8 + 30, screen.height() - 50 - 30 - 70, 16, 70,  ui.bgColor);
    screen.fillTriangle(screen.width() / 2 , screen.height() - 50, screen.width() / 2 - 15 , screen.height() - 50 - 30, screen.width() / 2 + 15, screen.height() - 50 - 30,  ui.bgColor);
    screen.fillRect(screen.width() / 2 - 8, screen.height() - 50 - 70- 30, 16, 70,  ui.bgColor);
  }
  else if (dir == 3 && pointnum == 3) {
    screen.fillTriangle(20, screen.height() / 2 - 30, 20 + 30, screen.height() / 2 - 15 - 30, 20 + 30, screen.height() / 2 + 15 - 30, COLOR_RGB565_BLUE);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 - 30, 70, 16, COLOR_RGB565_BLUE);
    screen.fillTriangle(20, screen.height() / 2 + 30, 20 + 30, screen.height() / 2 - 15 + 30, 20 + 30, screen.height() / 2 + 15 + 30, COLOR_RGB565_BLUE);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 + 30, 70, 16, COLOR_RGB565_BLUE);
    screen.fillTriangle(20, screen.height() / 2 , 20 + 30, screen.height() / 2 - 15, 20 + 30, screen.height() / 2 + 15 , COLOR_RGB565_BLUE);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 , 70, 16, COLOR_RGB565_BLUE);
 
    delay(500);
    screen.fillTriangle(20, screen.height() / 2 - 30, 20 + 30, screen.height() / 2 - 15 - 30, 20 + 30, screen.height() / 2 + 15 - 30, ui.bgColor);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 - 30, 70, 16, ui.bgColor);
    screen.fillTriangle(20, screen.height() / 2 + 30, 20 + 30, screen.height() / 2 - 15 + 30, 20 + 30, screen.height() / 2 + 15 + 30, ui.bgColor);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 + 30, 70, 16, ui.bgColor);
    screen.fillTriangle(20, screen.height() / 2 , 20 + 30, screen.height() / 2 - 15, 20 + 30, screen.height() / 2 + 15 , ui.bgColor);
    screen.fillRect(20 + 30, screen.height() / 2 - 8 , 70, 16, ui.bgColor);
  }
}
