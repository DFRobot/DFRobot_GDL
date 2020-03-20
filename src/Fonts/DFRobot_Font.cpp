/*!
 * @file DFRobot_Font.cpp
 * @brief 定义DFRobot_GDL显示库, 字体显示细节实现
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @https://github.com/DFRobot/DFRobot_GDL
 */

#include "DFRobot_GDL.h"
#include "gdlFontDef.h"
#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

#define FONT_MAX_OFFSET  0xFFFFFFFF

void DFRobot_GDL::setFont(const void *font){
  uint8_t basline, xMaxAdavanceNew, yMaxAdavance, lastFlag = 0;//lastFlag非0代表内置字体，GFXfont生效，0代表自定义字体，gdl_Font_t生效
  font ? lastFlag = (uint8_t)pgm_read_byte(&((gdl_Font_t *)font)->last) : lastFlag = 1;
  if(lastFlag && _fontType == FONT_TYPE_BUILTIN){
      Adafruit_GFX::setFont((const GFXfont *)font);
  }else if(lastFlag && _fontType == FONT_TYPE_CUSTOM){
      getGFXFontBaselineAndMaxXAdvance((const GFXfont *)font, &basline,&xMaxAdavanceNew);
      if(cursor_x + xMaxAdavanceNew*textsize_x > _width){
          cursor_x = 0;
          cursor_y += pgm_read_byte(&_gdlFont->yAdvance);
      }
      font ? yMaxAdavance = (uint8_t)pgm_read_byte(&(((GFXfont *)font)->yAdvance)) : yMaxAdavance = 10;
      if(cursor_x && (uint8_t)pgm_read_byte(&_gdlFont->yAdvance) > yMaxAdavance*textsize_y)
          cursor_y += ((uint8_t)pgm_read_byte(&_gdlFont->yAdvance) - yMaxAdavance*textsize_y);
      cursor_y += basline;
      gfxFont = (GFXfont *)font;
  }else if(!lastFlag && _fontType == FONT_TYPE_BUILTIN){
      getGFXFontBaselineAndMaxXAdvance(gfxFont, &basline);
      if(cursor_y) cursor_y -= basline;
      gfxFont ? yMaxAdavance = (uint8_t)pgm_read_byte(&(gfxFont->yAdvance)) : yMaxAdavance = 10;
      if(cursor_x + pgm_read_byte(&((gdl_Glyph_t *)pgm_read_dword(&((gdl_Font_t *)font)->glyph))->xAdvance) > _width){
          cursor_x = 0;
          cursor_y += yMaxAdavance;
      }
      if(cursor_x && yMaxAdavance*textsize_y > (uint8_t)pgm_read_byte(&(((gdl_Font_t *)font)->yAdvance)))
          cursor_y += (yMaxAdavance*textsize_y - (uint8_t)pgm_read_byte(&(((gdl_Font_t *)font)->yAdvance)));
  }else if(!lastFlag && _fontType == FONT_TYPE_CUSTOM){
      if(cursor_x + pgm_read_byte(&((gdl_Glyph_t *)pgm_read_dword(&((gdl_Font_t *)font)->glyph))->xAdvance)){
          cursor_x = 0;
          cursor_y += pgm_read_byte(&_gdlFont->yAdvance);
      }
      if(cursor_x && pgm_read_byte(&_gdlFont->yAdvance) > (uint8_t)pgm_read_byte(&(((gdl_Font_t *)font)->yAdvance)))
          cursor_y += (pgm_read_byte(&_gdlFont->yAdvance) - (uint8_t)pgm_read_byte(&(((gdl_Font_t *)font)->yAdvance)));
  }
  if(lastFlag){
      _fontType = FONT_TYPE_BUILTIN;
  }else{
      _fontType = FONT_TYPE_CUSTOM;
      _gdlFont = (gdl_Font_t *)font;
  }
}
size_t DFRobot_GDL::write(const uint8_t *buffer, size_t size){//默认为uft8，这里不处理gbk
  size_t n = 0;
  uint8_t c = 0;
  uint8_t num = 0;
  uint32_t unicode = 0;
  uint32_t length = 0;
  if(_fontType == FONT_TYPE_BUILTIN){
      while(size--) {
          n += Adafruit_GFX::write(*buffer++);
      }
  }else if(_fontType == FONT_TYPE_CUSTOM && _gdlFont){
      while(size){
          c = *buffer++;
          gdl_Font_t gdlFont;
          gdlFont.bitmap = (uint8_t *)pgm_read_dword(&_gdlFont->bitmap);
          gdlFont.glyph = (gdl_Glyph_t *)pgm_read_dword(&_gdlFont->glyph);//取出glyph的地址
          gdlFont.glyph += 1;
          gdlFont.yAdvance = pgm_read_byte(&_gdlFont->yAdvance);
          if(c == '\n'){
              cursor_x  = 0;
              cursor_y += gdlFont.yAdvance;
              size -= 1;
          }else if(c == '\r'){
              size -= 1;
          }else{
              length = 0;
              num = getUtf_8Bytes(c);
              unicode = utf8ToUnicode(num, (uint8_t *)(buffer-1));
              while(pgm_read_word(&gdlFont.glyph->unicode)){
                  if(unicode == (uint32_t)pgm_read_word(&gdlFont.glyph->unicode)){
                      gdlFont.bitmap += length;
                      if(wrap && ((cursor_x + (uint16_t)pgm_read_byte(&(gdlFont.glyph->xAdvance))) > _width)){
                          cursor_x = 0;
                          cursor_y += gdlFont.yAdvance;
                      }
                      drawCharBitmaps(cursor_x, cursor_y, &gdlFont, textcolor, textbgcolor);
                      cursor_x += pgm_read_byte(&(gdlFont.glyph->xAdvance));
                      break;
                  }
                  if(length + pgm_read_word(&(gdlFont.glyph->length)) > FONT_MAX_OFFSET){//当字的偏移超过FONT_MAX_OFFSET时
                      gdlFont.bitmap += length;
                      length = 0;
                  }
                  length += (uint32_t)pgm_read_word(&(gdlFont.glyph->length));
                  gdlFont.glyph++;
              }
              buffer += num - 1;
              if(size > num)
                 size -= num;
              else
                  size = 0;
              n += num;
          }
      }
  }
  return n;
}

void DFRobot_GDL::drawCharBitmaps(uint16_t x, uint16_t y, gdl_Font_t *gdlFont, uint16_t fg, uint16_t bg){
  uint16_t length = pgm_read_word(&(gdlFont->glyph->length));
  uint8_t width = pgm_read_byte(&(gdlFont->glyph->width));
  uint8_t xStep = pgm_read_byte(&(gdlFont->glyph->xAdvance));
  uint8_t yStep = gdlFont->yAdvance;
  if((x >= _width) || (y+yStep >= _height)) return;
  if(fg != bg)
      fillRect(x, y, xStep, yStep, bg);
  uint16_t x_pos = x + pgm_read_byte(&gdlFont->glyph->xOffset);
  uint16_t y_pos = y + pgm_read_byte(&gdlFont->glyph->yOffset);
  x = x_pos;
  y = y_pos;
  uint8_t w = 0;
  uint8_t total = 0;//记录每行的总宽度
  for(uint16_t i = 0; i < length; i++){
      for(uint8_t j = 0; j < 2; j++){
          uint8_t val = (pgm_read_byte(&gdlFont->bitmap[i]))>>(4*(1-j));
          w = val&0x07;
          total += w;
          while(total > width){//代表一行结束,y的值增加
              uint8_t c = width - (total - w);
              if(c != 0){
                  if(val & 0x08){
                      drawFastHLine(x, y,c,fg);
                   }//else{
                      // if(fg != bg) drawFastHLine(x, y,c,bg);
                  // }
              }
              x = x_pos;
              y += 1;
              w = w - c;
              total = w;
          }
          if(val & 0x08){
              drawFastHLine(x, y,w,fg);
          }
          x += w;
      }
  }
}

uint32_t DFRobot_GDL::utf8ToUnicode(uint8_t num, uint8_t *buf){
  uint32_t unicode = 0;
  uint8_t Byte[num];
  memset(Byte, 0, sizeof(Byte));
  DBG(buf[0]);
  if(num > 1 && num < 7){//大概率为uft-8编码
      for(uint8_t i = 0; i < num; i++){
          Byte[0] |= 1 <<(7-i);
          if (i > 0){
              Byte[i] = 0xC0;
          }
      }
      Byte[0] |= 1<<(7 - num);
      for(uint8_t i = 0; i < num; i++){
          Byte[i] = ~Byte[i] & *buf++;
      }
      uint8_t index = 0;
      uint8_t n = num;
      while((n--)){
          unicode |= uint32_t(Byte[n]) <<index;
          index += 6;
      }
  }else if(num == 1){
      unicode = buf[0];
  }
  return unicode;
}

uint8_t DFRobot_GDL::getUtf_8Bytes(uint8_t b){
   uint8_t num = 0, i = 0;
   for(i = 0; i < 7; i++){
       if(b&(0x80>>i)){
          num +=1;
        }else{
            if (num == 0)
               num = 1; 
            break;
          }
    }
    if(i==1)
      num = 255;
    return num;
}

