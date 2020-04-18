/*!
 * @file gdlFontDef.h
 * @brief The format of custom font structure 
 *
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [Arya] (xue.peng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-23
 * @https: //github.com/DFRobot/DFRobot_GDL
 */
#ifndef _GDLFONTDEF_H_
#define _GDLFONTDEF_H_
#include "Arduino.h"
#include <DFRobot_Type.h>
#define FONT_TYPE_BUILTIN  0//Built-in font GFXfont
#define FONT_TYPE_CUSTOM   1//Built-in font GFXfont custom font
#define FONT_TYPE_FLASH    2//gdl_Font_t

typedef struct{
  uint16_t unicode;  /**< Font unicode*/
  uint16_t length;  /**< Font effective data length*/
  uint8_t  width;   /**< Font outline width*/
  uint8_t  height;  /**< Font outline height*/
  int8_t xOffset;  /**< Origin point offset in x direction */
  int8_t yOffset;  /**< Origin point offset in y direction*/
  uint8_t xAdvance; /**< Total font width*/
} gdl_Glyph_t;

typedef struct{
  uint8_t  *bitmap;      /**< Glyph bitmaps, concatenated*/
  gdl_Glyph_t *glyph;       /**< Glyph array*/
  uint8_t type;  /**< Font type, indicating that it is a custom font*/
  uint8_t last;  /**< This value is always 0, by which to distinguish GFXfont*/
  uint8_t yAdvance; /**< Font height*/
}gdl_Font_t;

#include "DFRobot_Font.h"
#endif 
