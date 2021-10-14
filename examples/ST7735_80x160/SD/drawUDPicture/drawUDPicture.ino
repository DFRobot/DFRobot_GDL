/*!
 * @file UD.ino
 * @brief Read pictures in bmp / jpg / jpeg format from the SD card and display them on the screen. bmp supports 16-bit / 24-bit / 32-bit.
 * @n jpg only supports JFIF format, you can open with windows drawing and save it as this format.
 * @n The pictures of this demo are stored under example-> SD-> picture; just copy the picture folder to M0 flash S
 * @n The demo supports FireBeetle-M0
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V0.1
 * @date  2021-04-13
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include <SD.h>
#include <SPI.h>
#include "DFRobot_GDL.h"
#include "DFRobot_Picdecoder_SD.h"

DFRobot_Picdecoder_SD decoder;

//Custom communication pins
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
/*AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 6
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
 *User-selectable macro-defined color
 *COLOR_RGB565_BLACK   COLOR_RGB565_NAVY    COLOR_RGB565_DGREEN   COLOR_RGB565_DCYAN 
 *COLOR_RGB565_MAROON  COLOR_RGB565_PURPLE  COLOR_RGB565_OLIVE    COLOR_RGB565_LGRAY
 *COLOR_RGB565_DGRAY   COLOR_RGB565_BLUE    COLOR_RGB565_GREEN    COLOR_RGB565_CYAN  
 *COLOR_RGB565_RED     COLOR_RGB565_MAGENTA COLOR_RGB565_YELLOW   COLOR_RGB565_ORANGE           
 *COLOR_RGB565_WHITE
 */
void setup()
{
  //Initialize the serial port
  SerialUSB.begin(115200);
  //Initialize screen
  screen.begin();
  //Initialize the SD card, wait until the initialization is successful
  while(1)
  {
    if (SD.begin())
    {
      SerialUSB.println("initialization done.");
      break;
    }
    SerialUSB.println("initialization failed!");
  }
}


//Store file name of the icon in the read directory
char str[32];

void loop()
{
  /* Set the screen color to white */
  screen.fillScreen(COLOR_RGB565_WHITE);
  /**
   * @brief decoding function, call point-drawing function to display bmp pictures in sd card
   * @param filename The file name and path of the SD card to be opened (the name cannot be too long due to the limitation of SD.open).
   * @param sx The x coordinate to start displayine
   * @param sy The y coordinate to start displaying
   * @param ex The x coordinate to end displayig
   * @param ey The y coordinate to end displaying
   * @param screenDrawPixel point-drawing function name
   */
  decoder.drawPicture(/*filename=*/"picture/219x220.jpg",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);
  screen.fillScreen(COLOR_RGB565_WHITE);
  decoder.drawPicture(/*filename=*/"picture/RGB565.bmp",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);
 /*
  * Mode parameter that can be set by UD.open function
  * FILE_READ: open the file for reading, starting from the beginning of the file
  * FILE_WRITE: open the file for reading and writing, starting from the end of the file
  */
   //Batch display icons
  SDFile myDir = SD.open(/*directory name=*/"picture/Icon/",/*mode=*/FILE_READ);
  if(myDir)
  {
    /* Set the screen color to white */
    screen.fillScreen(COLOR_RGB565_WHITE);
    //Display an icon every 32 pixels until the screen is full or all icons are displayed
    for(uint16_t y = 10; y<screen.height()-32; y+=60)//y coordinate
    {
      for(uint16_t x = 10; x<screen.width()-32; x+=60)//x coordinate
      {
        //Read the next file information in the directory
        SDFile entry = myDir.openNextFile();
        if (! entry)
        {
          //There are no more pictures to show, jump out of the loop
          goto quit;
        }
        //Splice file name
        strcpy(str,"picture/Icon/");
        strcat(str,entry.name());
        //Show an icon
        decoder.drawPicture(/*filename=*/str,/*sx=*/x,/*sy=*/y,/*ex=*/x+32,/*ey=*/y+32,/*screenDrawPixel=*/screenDrawPixel);
      }
    }
quit:
    myDir.close();
  }
  else
  {
    SerialUSB.println("dir open fail");
  }

  delay(1000);
}

//For decoding function calling, this function is used to draw a pixel on the screen
void screenDrawPixel(int16_t x, int16_t y, uint16_t color)
{
  //Draw a point on the screen
  screen.writePixel(x,y,color);
}