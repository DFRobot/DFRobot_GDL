/*!
 * @file gdlFontDef.h
 * @brief 自定义字体结构的格式
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @https://github.com/DFRobot/DFRobot_GDL
 */
/*!
 * @file gdlFontDef.h
 * @brief custom font structure format
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
#define FONT_TYPE_BUILTIN  0//内置字体GFXfont  Built-in font GFXfont
#define FONT_TYPE_CUSTOM   1//自定义字体  Built-in font GFXfont custom font
#define FONT_TYPE_FLASH    2//gdl_Font_t

typedef struct{
  uint16_t unicode;  /**< 字体Unicode码*/  /**< Font unicode*/
  uint16_t length;  /**< 字体有效数据长度*/  /**< Font effective data length*/
  uint8_t  width;   /**< 字体轮廓宽度*/  /**< Font outline width*/
  uint8_t  height;  /**< 字体轮廓高度*/  /**< Font outline height*/
  int8_t xOffset;  /**< 原点x方向偏移*/  /**< Origin x direction offset*/
  int8_t yOffset;  /**< 原点y方向偏移*/  /**< Origin offset in y direction*/
  uint8_t xAdvance; /**< 字体总宽度*/  /**< Total font width*/
} gdl_Glyph_t;

typedef struct{
  uint8_t  *bitmap;      /**< Glyph bitmaps, concatenated*/
  gdl_Glyph_t *glyph;       /**< Glyph array*/
  uint8_t type;  /**<字体类型，表示是自定义字体*/  /**< Font type, indicating that it is a custom font*/
  uint8_t last;  /**<该值一直为0，以此区分GFXfont*/  /**< This value is always 0, to distinguish GFXfont*/
  uint8_t yAdvance; /**< 字体高度*/  /**< Font height*/
}gdl_Font_t;

#include "DFRobot_Font.h"
#endif 