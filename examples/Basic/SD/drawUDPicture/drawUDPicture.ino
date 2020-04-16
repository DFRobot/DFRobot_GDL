/*!
 * @file UD.ino
 * @brief Read pictures in bmp / jpg / jpeg format from the SD card and display them on the screen. bmp supports 16-bit / 24-bit / 32-bit.
 * @n jpg only supports JFIF format, you can open with windows drawing and save it as this format.
 * @n The pictures of this demo are stored under example-> SD-> picture; just copy the picture folder to M0 flash S
 * @n The demo supports FireBeetle-M0
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [YeHangYu] (hangyu.ye@dfrobot.com)
 * @version V0.1
 * @date 2020-03-13
 * @url https://github.com/DFRobot/DFRobot_GDL
 */
#include <UD.h>
#include <SPI.h>
#include "DFRobot_GDL.h"
#include "picdecoder.h"

//Custom communication pins
/*M0*/
#define TFT_DC  7
#define TFT_CS  5
#define TFT_RST 6

/**
 * @brief Constructor Constructor of hardware SPI communication
 * @param dc Command/data line pin for SPI communication
 * @param cs Chip select pin for SPI communication
 * @param rst Reset pin of the screen
 */
//DFRobot_ST7789_240x240_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_HW_SPI  screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_HW_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
/* M0 motherboard DMA transfer */
//DFRobot_ST7789_240x240_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ST7789_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9341_240x320_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);
//DFRobot_ILI9488_320x480_DMA_SPI screen(/*dc=*/TFT_DC,/*cs=*/TFT_CS,/*rst=*/TFT_RST);

/*
 *User-selectable macro definition color
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
  //Wait for serial connection
  delay(1000);
  //Initialize screen
  screen.begin();
  //Initialize the SD card, wait until the initialization is successful
  while(1)
  {
    if (UD.begin())
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
  drawUDPicture(/*filename=*/"picture/219x220.jpg",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);
  screen.fillScreen(COLOR_RGB565_WHITE);
  drawUDPicture(/*filename=*/"picture/RGB565.bmp",/*sx=*/0,/*sy=*/0,/*ex=*/screen.width(),/*ey=*/screen.height(),/*screenDrawPixel=*/screenDrawPixel);
 /*
  * Mode parameter that can be set by UD.open function
  * FILE_READ: open the file for reading, starting from the beginning of the file
  * FILE_WRITE: open the file for reading and writing, starting from the end of the file
  */
   //Batch display icons
  UDFile myDir = UD.open(/*directory name=*/"picture/Icon/",/*mode=*/FILE_READ);
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
        UDFile entry = myDir.openNextFile();
        if (! entry)
        {
          //There are no more pictures to show, jump out of the loop
          goto quit;
        }
        //Splice file name
        strcpy(str,"picture/Icon/");
        strcat(str,entry.getName());
        //Show an icon
        drawUDPicture(/*filename=*/str,/*sx=*/x,/*sy=*/y,/*ex=*/x+32,/*ey=*/y+32,/*screenDrawPixel=*/screenDrawPixel);
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
