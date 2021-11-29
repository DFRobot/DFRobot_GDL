/*!
 * @file drawSDPicture.ino
 * @brief Read pictures in bmp / jpg / jpeg format from the SD card and display them on the screen. bmp supports 16-bit / 24-bit / 32-bit.
 * @n jpg only supports JFIF format, you can open with windows drawing and save it as this format.
 * @n The pictures of this demo are stored under example-> SD-> picture; just copy the picture folder to the SD card
 * @n This demo supports mainboard ESP8266, FireBeetle-M0, MAGE2560, and UNO.
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author [fary] (feng.yang@dfrobot.com)
 * @version V1.0
 * @date 2021-11-04
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
#define TFT_SD  3
/*ESP32 ESP8266*/
#elif defined(ESP32)
#define TFT_DC  D2
#define TFT_CS  D6
#define TFT_RST D3
#define TFT_SD  D7
/*ESP8266*/
#elif defined(ESP8266)
#define TFT_DC  D4
#define TFT_CS  D6
#define TFT_RST D5
#define TFT_SD  D7
/* AVR series mainboard */
#else
#define TFT_DC  2
#define TFT_CS  3
#define TFT_RST 4
#define TFT_SD  6
#endif
/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst reset pin of the screen
 */
//DFRobot_ST7735_128x160_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 mainboard DMA transfer */
//DFRobot_ST7735_128x160_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);


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
  Serial.begin(115200);
  //Initialize screen
  screen.begin();
  //Initialize the SD card, wait until the initialization is successful
  while(1)
  {
    #if defined ARDUINO_SAM_ZERO
    if (SD.begin(/*sdcs=*/TFT_SD,/*type = */TYPE_NONBOARD_SD_MOUDLE)){
    #else 
    if (SD.begin(/*sdcs=*/TFT_SD)){
    #endif
      Serial.println("initialization done.");
      break;
    }
    Serial.println("initialization failed!");
  }
}



void loop()
{
  /* Set the screen color to white */
  screen.fillScreen(COLOR_RGB565_WHITE);

 
  /**
   * @brief decoding function, call point-drawing function to display bmp pictures in sd card. jpg pictures can be displayed via FireBeetle-M0, ESP32 and ESP8266. 
   * @n     The pictures of this demo are stored under example-> SD-> picture; just copy the picture folder to the SD card
   * @param filename The file name and path of the SD card to be opened (the name cannot be too long due to the limitation of SD.open).
   * @param sx The x coordinate to start displaying
   * @param sy The y coordinate to start displaying
   * @param ex The x coordinate to end displaying
   * @param ey The y coordinate to end displaying
   * @param screenDrawPixel point-drawing function name
   */
#if defined ARDUINO_SAM_ZERO || defined(ESP8266) ||defined(ESP32)
  //JPG images need to be decoded internally. AVR series master controls do not support JPG images due to lack of memory
  decoder.drawPicture(/*filename=*/"/picture/219x220.jpg",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);
  screen.fillScreen(COLOR_RGB565_WHITE);
#endif 
  decoder.drawPicture(/*filename=*/"/picture/RGB565.bmp",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);

 /* Set the screen color to white */ 
  screen.fillScreen(COLOR_RGB565_WHITE);

  //Display batch icons, display any number of icons on large-capacity mainboard.
  /*FireBeetle-M0，ESP32 and ESP8266*/
#if defined ARDUINO_SAM_ZERO || defined(ESP8266)
 /*
  * Mode parameter that can be set by SD.open function
  * FILE_READ: open the file for reading, starting from the beginning of the file
  * FILE_WRITE: open the file for reading and writing, starting from the end of the file
  */
  File myDir = SD.open(/*directory name=*/"/picture/Icon/",/*mode=*/FILE_READ);
  if(myDir)
  {
    char str[32];//Store file name of the icon in the read directory
    //Display an icon every 32 pixels until the screen is full or all icons are displayed.
    for(uint16_t y = 10; y<screen.height()-32; y+=60)//y coordinate
    {
      for(uint16_t x = 10; x<screen.width()-32; x+=60)//X coordinate
      {
        //Read the next file information in the directory
        File entry = myDir.openNextFile();
        if (! entry)
        {
          //There are no more pictures to show, jump out of the loop
          goto quit;
        }
        //Splice file name
        strcpy(str,"/picture/Icon/");
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
    Serial.println("dir open fail");
  }
  /* AVR series motherboard */
#else
  decoder.drawPicture("/picture/Icon/1.bmp",0,0,32,32,screenDrawPixel);
  decoder.drawPicture("/picture/Icon/2.bmp",32,32,64,64,screenDrawPixel);
  decoder.drawPicture("/picture/Icon/3.bmp",64,64,96,96,screenDrawPixel);
  decoder.drawPicture("/picture/Icon/4.bmp",96,96,128,128,screenDrawPixel);
#endif
  delay(1000);
}

//For decoding function calling, the function of this function is to draw a pixel on the screen
void screenDrawPixel(int16_t x, int16_t y, uint16_t color)
{
  //Draw a point on the screen
  screen.writePixel(x,y,color);
}
