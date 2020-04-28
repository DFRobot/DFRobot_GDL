/*!
 * @file DFRobot_GDL.h
 * @brief Declare the DFRobot_GDL class and declare multiple screen subclasses
 * @n DFRobot_GDL Display framework inherited from Adafruit's GFX framework, link: https://github.com/adafruit/Adafruit-GFX-Library
 * @n Support UNO, Mega2560, Leonardo, ESP32, ESP8266, M0 and other master controls
 * @n Support hardware I2C, SPI, SPI_DMA interface
 * @n Support color screen and black-and-white screen
 * @n Refer to the naming method of u8glib to name screens, and distinguish different screens by driver IC/resolution/communication interface
 *
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [Arya] (xue.peng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-23
 * @https: //github.com/DFRobot/DFRobot_GDL
 */
#ifndef __DFROBOT_GDL_H
#define __DFROBOT_GDL_H

#include "DFRobot_Type.h"
#include <Arduino.h>
#include "Print.h"
#include "Frame/Adafruit_GFX.h"
#include "Interface/DFRobot_IF.h"
#include "Drivers/DFRobot_LCDType.h"
#include "Fonts/gdlFontDef.h"
#ifdef ARDUINO_SAM_ZERO
#include <DFRobot_Flash_Font.h>
#include <W25Q.h>
#endif
#define COLOR_MODE_SINGLE  0
#define COLOR_MODE_RGB111  1
#define COLOR_MODE_RGB565  2
#define COLOR_MODE_RGB666  3
#define COLOR_MODE_RGB888  4

#if 0
#ifdef ARDUINO_SAM_ZERO//The serial port of M0 board is different from other serial ports
#define DBG(...) {SerialUSB.print("["); SerialUSB.print(__FUNCTION__); SerialUSB.print("(): "); SerialUSB.print(__LINE__); SerialUSB.print(" ] "); SerialUSB.println(__VA_ARGS__);}
#else
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#endif
#else
#define DBG(...)
#endif


/*
 MADCTL of 0x36 register:
   * -------------------------------------------------------------------------
   * |   D7   |   D6   |   D5   |   D4   |   D3   |   D2   |   D1   |   D0   |
   * -------------------------------------------------------------------------
   * |   MY   |   MX   |   MV   |   ML   |   RGB  |   MH   |      reserve    |
   * -------------------------------------------------------------------------
   *Default value:0x00, display mode: top to bottom, left to right, RGB order
*/
typedef union{
  struct{
      uint8_t rsrv: 2; /**< reserve */
      uint8_t mh: 1;   /**< Display Data Latch Order */
      uint8_t rgb: 1;  /**< RGB/BGR Order 0: RGB, 1: BGR*/
      uint8_t ml: 1;   /**< Line Address Order */
      uint8_t mv: 1;   /**< Page/Column Order */
      uint8_t mx: 1;   /**< Column Address Order */
      uint8_t my: 1;   /**< Page Address Order */
  };
  uint8_t value; /**<The value of the register MADCTL*/
}uMadctlArgs_t;

typedef struct{
  uint8_t madctl; /**<Register MADCTL command*/
  uMadctlArgs_t args; /**<The value of the register MADCTL*/
}sMadctlCmd_t;

typedef struct{
  uint8_t rotation;/**<Rotation angle: 0, No rotation; 1, 90 ° rotation; 2, 180 ° rotation; 3, 270 ° rotation*/
  uint8_t cMode; /**<Screen color format, such as RGB565, RGB666, RGB888*/
  uint8_t *buffer;/**<Generally used in black/white or grayscale display, and one byte controls several pixels or one pixel is controlled by non-integer bytes, and cannot support read mode*/
}sGdlLcdDev_t;

class DFRobot_GDL : public Adafruit_GFX, public DFRobot_IF{
public:
  /**
    * @brief Constructor When the screen uses hardware SPI communication, call this constructor
    * @param dev Pointer to communication interface structure. The struct holds the screen's communication interface type,
    * @n communication frequency, and related IO pins. For different main-controllers, the maximum number of bytes processed in
    * @n one communication and the screen initialization array and Interface function pointer are different.
    * @param w Wide resolution of the screen
    * @param h High resolution of the screen
    * @param dc Command/data line pin for SPI communication
    * @param cs Chip select pin for SPI communication
    * @param rst Reset pin of the screen
    * @param bl Backlight control pin of the screen
    */
  DFRobot_GDL(sGdlIFDev_t *dev, int16_t w, int16_t h, uint8_t dc, uint8_t cs, uint8_t rst, uint8_t bl);
  /**
    * @brief Constructor When the screen uses hardware IIC communication, call this constructor
    * @param dev Pointer to communication interface structure. The scruct holds the screen's communication interface type, communication
    * @n frequency, and related IO pins. For different masters, the maximum number of bytes processed in one communication and the screen
    * @n initialization array and Interface function pointer are different.
    * @param w Wide resolution of the screen
    * @param h High resolution of the screen
    * @param addr IIC communication address
    * @param rst Reset pin of the screen
    * @param bl Backlight control pin of the screen
    */
  DFRobot_GDL(sGdlIFDev_t *dev, int16_t w, int16_t h, uint8_t addr, uint8_t rst, uint8_t bl);
  ~DFRobot_GDL();
  virtual void begin(uint32_t freq = 0){};
  /**
    * @brief Function to draw a pixel 
    * @param x X-coordinate position of the pixel
    * @param y Y-coordinate position of the pixel
    * @param color The color of pixels, RGB565 format
    */
  void drawPixel(int16_t x, int16_t y, uint16_t color);
  /**
    * @brief Function to refresh screen
    * @param color color to fill screen, RGB565 format
    */
  void fillScreen(uint16_t color);
  /**
    * @brief Draw vertical line fast
    * @param x X-coordinate position of the starting point of the vertical line
    * @param y Y-coordinate position of the starting point of the vertical line
    * @param h Height of the vertical line
    * @param color The color of the vertical line, RGB565 format
    */
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  /**
    * @brief Draw horizontal line fast
    * @param x X-coordinate position of the starting point of the horizontal line
    * @param y Y-coordinate position of the starting point of the horizontal line
    * @param w Length of horizontal line
    * @param color The color of the vertical line, RGB565 format
    */
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  /**
    * @brief Fill rectangle 
    * @param x X-coordinate position of the starting point of the rectangle
    * @param y Y-coordinate position of the starting point of the rectangle
    * @param w Width of the rectangle
    * @param h Height of the rectangle
    * @param color The color of the rectangle, RGB565 format
    */
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
  /**
    * @brief Set font
    * @param font Font structure pointer, point to GFXfont or gdl_Font_t
    */
  void setFont(const void *font = NULL);
  /**
    * @brief Set the rotation mode
    * @param r Effective rotation mode 0, 1, 2, 3  
    */
  void setRotation(uint8_t r);
  
  /**
   * @brief  Invert the colors of the display (if supported by hardware).
   * @n        Self-contained, no transaction setup required.
   * @param  i  true = reversed display, false = normal display.
   */
  void invertDisplay(bool i);
  /**
    * @brief Covert color RGB565 data to color RGB666 data
    * @param rgb666 Array of RGB666 color data pointers
    * @param color raw RGB565 format color data
    */
  uint8_t rgb565ToRGB666(uint8_t *rgb666, uint16_t color);
  /**
   * @brief Covert color RGB565 data to color RGB888 data, 24-bit true color
   * @param r  Red component of RGB888
   * @param g  Green component of RGB888
   * @param b  Blue component of RGB888
   * @param color  Raw RGB565 color data
   */
  void rgb565ToRGB888(uint8_t *r, uint8_t *g, uint8_t *b, uint16_t color);
  /**
    * @brief Compress RGB888 format color data into color data in RGB666 format
    * @param r Red component of original RGB888 color data
    * @param g Green component of original RGB888 color data 
    * @param b Blue component of original RGB888 color data
    * @return RGB565 format color data
    */
  uint16_t rgb888ToRGB565(uint8_t r, uint8_t g, uint8_t b);
  /**
   * @brief Write bytes into screen.
   * @n The following are the overload functions of the byte of different data type. 
   * @param buffer  Buffer for saving or transmitting data 
   * @param size  The number of bytes to send
   * @return Return the actual number of bytes written
   */
  virtual size_t write(const uint8_t *buffer, size_t size);
  /**
    * @brief Convert utf-8 encoding to unicode encoding
    * @param num The number of bytes of UTF-8 encode
    * @param buf The encode of UTF-8 data
    * @return 32-bit Unicode encode data
    */
  uint32_t utf8ToUnicode(uint8_t num, uint8_t *buf);
  /**
    * @brief Get the number of bytes of a UTF-8 encode date 
    * @param b The first data of UTF-8 encode
    * @return The number of bytes of UTF-8 encode data
    */
  uint8_t getUtf_8Bytes(uint8_t b);
  /**
    * @brief  Drawing bitmap method of custom font
    * @param x X-coordinate of the character displayed on the screen
    * @param y Y-starting coordinate of the character displayed on the screen
    * @param gdlFont custom font structure pointer, store character data information
    * @param fg Foreground color, text color
    * @param bg Background color, text background color
    */
  void drawCharBitmaps(uint16_t x, uint16_t y, gdl_Font_t *gdlFont, uint16_t fg, uint16_t bg);
  /**
    * @brief When using the OLED screen, the update function must be called to take effect after the operation is completed
    */
  void update();
  
protected:
  /**
    * @brief Display initialization function, the screen will work after initialization done
    * @param freq frequency
    */
  void initDisplay();
  /**
    * @brief Display initialization function
    * @param freq frequency
    */
  void gdlInit(uint32_t freq = 0);
  /**
    * @brief Pure virtual function, set rectangular display area, this function should be implemented in subclasses
    * @param x The position of the start row
    * @param y The position of the start column
    * @param w Display area width
    * @param h Display area height
    * @param color The color of pixels, RGB565 format
    */
  virtual void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color) = 0;
  /**
    * @brief Write command
    * @param cmd The data is a command
    */
  void sendCommand(uint8_t cmd);
  /**
    * @brief Write command and data
    * @param cmd The data is a command
    * @param args const pointer to store data
    * @param len Length of pointer data
    * @param isRamData const pointer points to ROM or RAM, default point to ROM. A special method is required to read the data args points to 
    */
  void sendCommand(uint8_t cmd, void *args, uint32_t len, bool isRamData = false);
  /**
    * @brief write one byte of data
    * @param data The parameter is the screen data
    */
  void sendData(uint8_t data);
  /**
    * @brief write 2 bytes of data
    * @param data The parameter is the screen data
    */
  void sendData16(uint16_t data);
  /**
    * @brief If the screen is in RGB565 format, you can call this function directly
    * @param color RGB565 format color data
    * @param len The number of pixels to be operated
    */
  void sendColor(uint16_t color, uint32_t len);
  /**
    * @brief Set color for pixels
    * @param c uint8_t pointer to color data, the color format could be RGB565, RGB666, etc., data in different color formats
    * @param cBytes Means how many bytes are used to represent one pixel
    * @param isRamData Pointer c points to ROM or RAM, default point to ROM. A special method is required to read the data args points to.
    */
  void sendColor(uint8_t *c, uint8_t cBytes, uint32_t len, bool isRamData = true);
  /**
    * @brief Set the resolution of the display driver IC
    * @param w x resolution of driver IC, when rotation = 0
    * @param h y resolution of driver IC, when rotation = 0
    */
  void setDriverICResolution(int16_t w, int16_t h);
  /**
    * @brief Switch 16-bit RGB565 color format to other color formats, such as RGB888, black and white format, etc.
    * @param pBuf Store the switched color data
    * @param len Length of the color data in pBuf, which means how many bytes are required to represent the color data 
    * @param pixel The number of pixels that can be controlled by the color data in pBuf 
    * @param color Raw color data, RGB565
    */  
  void getColorFormat(uint8_t *pBuf, uint8_t &len, uint8_t &pixel, uint16_t color);

protected:
  uint8_t _xStart;
  uint8_t _yStart;
  int16_t _icWidth;
  int16_t _icHeight;
  uint8_t invertOnCmd;
  uint8_t invertOffCmd;
  sGdlLcdDev_t _lcd;
  sMadctlCmd_t madctlReg;
  
private:
  gdl_Font_t *_gdlFont;
  uint8_t _fontType;
  
};

/**
  * @brief 1.54 inches SPI color screen
  * Driver IC: ST7789
  * Resolution: 240x240 (width x height)
  * Communication Interface: hardware SPI
  */
class DFRobot_ST7789_240x240_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x240_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x240_HW_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};


/**
  * @brief 1.54 inches SPI color screen
  * Driver IC: ST7789
  * Resolution: 240x320 (width x height)
  * Communication Interface: hardware SPI
  */
class DFRobot_ST7789_240x320_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x320_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x320_HW_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

class DFRobot_ST7735S_80x160_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7735S_80x160_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7735S_80x160_HW_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ILI9488_320x480_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9488_320x480_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9488_320x480_HW_SPI();
  void begin(uint32_t freq = 0);
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ILI9341_240x320_HW_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9341_240x320_HW_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9341_240x320_HW_SPI();
  void begin(uint32_t freq = 0);
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

/**
  * @brief 0.91 inch oled screen (black and white)
  * Driver IC: SSD1306
  * Resolution: 128x32 (width x height)
  * Communication Interface: hardware IIC
  * IIC communication Address: default 0x3C
  */
class DFRobot_SSD1306_128x32_HW_IIC: public DFRobot_GDL{
public:
  DFRobot_SSD1306_128x32_HW_IIC(uint8_t addr = 0x3C, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_SSD1306_128x32_HW_IIC();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

#ifdef ARDUINO_SAM_ZERO
class DFRobot_ST7789_240x240_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x240_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x240_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ST7735S_80x160_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7735S_80x160_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7735S_80x160_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ST7789_240x320_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ST7789_240x320_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ST7789_240x320_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
class DFRobot_ILI9488_320x480_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9488_320x480_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9488_320x480_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};

class DFRobot_ILI9341_240x320_DMA_SPI: public DFRobot_GDL{
public:
  DFRobot_ILI9341_240x320_DMA_SPI(uint8_t dc, uint8_t cs = GDL_PIN_NC, uint8_t rst = GDL_PIN_NC, uint8_t bl = GDL_PIN_NC);
  ~DFRobot_ILI9341_240x320_DMA_SPI();
  void begin(uint32_t freq = 0);
protected:
  void setDisplayArea(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
};
#endif
#endif
