#!/usr/bin/env python
# -*- coding: utf-8 -*-
# -*- coding: cp936 -*- 
import numpy as np
from freetype import * 
import math
import os
import codecs
import copy
import chardet

def setup():
  if not os.path.exists('ttf'):
    os.makedirs('ttf')
  if not os.path.exists('font'):
    os.makedirs('font')
  if not os.path.exists('config.txt'):
    fp = open('config.txt', 'w+')
    s = "FontFileNamePre = default\n"
    s += "FontSizeList = [12,18,24,36,48,72]\n"
    fp.write(s)
    fp.close()
  if not os.path.exists('text.txt'):
    fp = open('text.txt', 'w+')
    s = "你好，世界！"
    fp.write(s)
    fp.close()
  if not os.path.getsize('text.txt'):
    fp = open('text.txt', 'w+')
    s = "你好，世界！"
    fp.write(s)
    fp.close()

def readText():
  if os.path.exists('text.txt'):
    fp = open('text.txt', 'rb')
    r = fp.read()
    fp.close();
    f_charInfo = chardet.detect(r)
    fp = open('text.txt', 'r', encoding=f_charInfo['encoding'])
    #print(f_charInfo)
    charList = fp.readlines()
    fp.close()
    #charList = ["nihao"]
    return charList

def getFontDict():
  if os.path.exists('config.txt'):
    fontDict ={'FontFileNamePre':'', 'FontSizeList':[], 'charList':[]}
    fp = open('config.txt', 'r')
    configList = fp.readlines()
    fp.close()
    for s in configList:
      s1 = s.replace(' ','').replace('\n','')
      index = s1.find('#')
      if index == 0:
        continue
      else:
        index = s1.find('=')
        if index != -1:
          key = s1[:index]
          val = s1[index+1:]
          index = val.find('#')
          if index != -1:
            val = val[:index]
          if key in fontDict:
            fontDict[key] = val
    fontDict['FontSizeList'] = eval(fontDict['FontSizeList'])
    if fontDict['FontFileNamePre'] == "default":
      fontDict['FontFileNamePre'] = ""
    if len(fontDict['FontSizeList']) == 0:
      fontDict['FontSizeList'] = [12,18,24,36,48,72]
    fontDict['charList'] = readText()
    return fontDict

def parseFont(dstfilename, filename, lis, size = 12, angle = 0):
  face = freetype.Face(filename)
  face.set_char_size(size*64)
  Font_height = face.size.height//72
  Font_totallen = 0
  
  angle = (angle/180.0)*math.pi
  matrix = FT_Matrix((int)( math.cos( angle ) * 0x10000),
                     (int)(-math.sin( angle ) * 0x10000),
                     (int)( math.sin( angle ) * 0x10000),
                     (int)( math.cos( angle ) * 0x10000))
  flag = FT_LOAD_RENDER
  pen = FT_Vector(0,0)
  FT_Set_Transform( face._FT_Face, byref(matrix), byref(pen))
  previous = 0
  xmin, xmax = 0, 0
  ymin, ymax = 0, 0
  L = []
  unicodeList = []
  start = '{'
  unicode = 0
  length = 0
  width = 0
  rows = 0
  xadvance = 0
  xoffset = 0
  yoffset = 0
  end = '},\n'
  maxAadvancex = 0
  TotalList = []
  dict1 = {"unicode":0, "length":0, "width":0, "rows":0, "xadvance":0, "xoffset":0, "top":0}
  glyphDitct = {"lengthH":0, 'lengthL':0, 'maxwidth':0, 'maxrows':0,'minXoffset':255, "minYoffset":255,'maxAadvancex':0}
  baseline = 0
  for s in lis:
    s = s.replace('\n','')
    for c in s:
      dict1["unicode"] = getUnicode(c)[0]#hex will convert integer into string 
      if dict1["unicode"] in unicodeList:
        continue
      unicodeList.append(dict1["unicode"])
      face.load_char(c, flag)
      kerning = face.get_kerning(previous, c)
      previous = c
      dict1["xadvance"] = face.glyph.metrics.horiAdvance//64
      if glyphDitct['maxAadvancex'] < dict1["xadvance"]:
          glyphDitct['maxAadvancex'] = dict1["xadvance"]
      yadvance = face.glyph.metrics.vertAdvance//64
      if yadvance > Font_height:
          Font_height = yadvance
      bitmap1 = face.glyph.bitmap
      dict1["width"]  = face.glyph.bitmap.width
      dict1["rows"]   = face.glyph.bitmap.rows
      dict1["top"] = face.glyph.bitmap_top
      dict1["xoffset"] = face.glyph.bitmap_left
      if glyphDitct['minXoffset'] > dict1["xoffset"]:
          glyphDitct['minXoffset'] = dict1["xoffset"]
      if glyphDitct['maxwidth'] < dict1["width"]:
          glyphDitct['maxwidth'] = dict1["width"]
      if glyphDitct['maxrows'] < dict1["rows"]:
          glyphDitct['maxrows'] = dict1["rows"]
      if dict1["top"] > baseline:
          baseline = dict1["top"]
      
      bitmap = getBitmap(bitmap1.buffer)
      dict1["length"] = len(bitmap)#the number of the pixel data 
      if dict1["length"]:
          writeFileArray(dstfilename, bitmap)#Write in the pixel data
      TotalList.append(copy.deepcopy(dict1))
      Font_totallen +=1
  glyphDitct['lengthH'] = Font_totallen >> 0xFFFF
  glyphDitct['lengthL'] = Font_totallen & 0xFFFF
  for d in TotalList:
      yoffset = baseline - d['top']
      if glyphDitct['minYoffset'] > yoffset:
          glyphDitct['minYoffset'] = yoffset
      ll = []
      ll.append(d['unicode'])
      ll.append(hex(d['length']))
      ll.append(hex(d['width']))
      ll.append(hex(d['rows']))
      ll.append(hex(d['xoffset']))
      ll.append(hex(yoffset))
      ll.append(hex(d['xadvance']))
      ls = start + listToString(ll) + end
      L.append(ls)
  ls = '{0}\n'
  L.append(ls)
  glyphLL = []
  glyphLL.append(hex(glyphDitct['lengthH']))
  glyphLL.append(hex(glyphDitct['lengthL']))
  glyphLL.append(hex(glyphDitct['maxwidth']))
  glyphLL.append(hex(glyphDitct['maxrows']))
  glyphLL.append(hex(glyphDitct['minXoffset']))
  glyphLL.append(hex(glyphDitct['minYoffset']))
  glyphLL.append(hex(glyphDitct['maxAadvancex']))
  ls = start + listToString(glyphLL) + end
  L.insert(0,ls)
  L.append(Font_height)
  L.append(Font_totallen)
  return L

def listToString(List,c = ''):#Convert list to string, c indicates whether there is a comma added
  s = str(List).replace('[',']').strip(']').replace("'",'') + c
  return s
def getUnicode(text):#return a List
  L = []
  cl = []
  for i in text:
      cl.append(ord(i))#ord
  for i in cl:
      L.append(hex(i))
  return L

def getBitmap(bitmap):
  L = []
  n = 0
  highByte = 0
  flag = True  #if flag = 0 indicate that highByte_start  flag = 1 indicate that highByte_end  
  #flag = 2 indicate that lowByte_start  flag = 3 indicate that lowByte_end
  lowByte  = 0
  oneByte = 1
  halfByte = 0
  for i in range(len(bitmap)):
      n = n + 1
      if i + 1 == len(bitmap):# the last effective data 
          if bool(bitmap[i]):
              halfByte = 8
          else:
              halfByte = 0
          if flag:
              highByte = halfByte | n
              lowByte = 0
          else:
              lowByte = halfByte | n
          oneByte = (highByte << 4) | lowByte
          L.append(hex(oneByte))
          L.append(hex(0))
          return L
      if (bool(bitmap[i]) != bool(bitmap[i+1]) or n + 1 > 7):
          if bool(bitmap[i]):
              halfByte = 8
          else:
              halfByte = 0
          if flag:
              flag = False
              highByte = halfByte | n
              n = 0
          else:
              flag = True
              lowByte = halfByte | n
              oneByte = (highByte << 4) | lowByte
              L.append(hex(oneByte))
              n = 0
  return L
def loadList(List, num,remains, fix = 0):#Load data into list, and return the invalid length of the last byte of data in list 
  s = str(bin(num)).lstrip('0').strip('b')
  if fix != 0:
      lis = [0]*(fix-(s))
      s = listToString(lis).replace(',','').replace(' ','') + s
  tail = 0
  index = len(List) - 1
  while len(s):
      if len(s) > remains:
          tail = remains
      else:
          tail = len(s)
      value = int(s[:tail],2)
      if remains < BYTEBITS_MAX:
          List[index] = List[index] | (value << (remains -  tail))
      else:
          List.append((value << (remains - tail)))
      if remains - tail:
          remains -= tail
      else:
          remains = BYTEBITS_MAX
          index += 1
      s = s[tail:]
  
  return remains
def ModifySuffixName(filename, suffix):# Revise the file name suffix
  portion = os.path.splitext(filename)
  if portion[1] != suffix:
      newname = portion[0]+suffix
      if os.path.exists(newname):
          return
      os.rename(filename, newname)

def writeFile(filename, text):
  fp = open(filename, 'a+')
  fp.write(text)
  fp.close()


def writeFileArray(filename, List):#10 numbers in each row 
  #writeFile(filename, listToString(List[0:5], ',\n'))
  num = (len(List))//10;
  m = (len(List))%10
  for i in range(num):
      writeFile(filename, listToString(List[(i*10):(i*10)+10], ',\n'))
  if m:
      writeFile(filename, listToString(List[(num*10):], ',\n'))

if __name__ == '__main__':
  setup()
  pwdPath = os.getcwd()
  ttfSourcePath = pwdPath + '\\ttf'
  fontDestPath = pwdPath + '\\font'
  ttfFileList = os.listdir(ttfSourcePath)
  if len(ttfFileList) == 0:
    print("Failed! Please add truetype file with the extension .ttf in the ttf folder.")
    sys.exit()
  fontDict ={'FontFileNamePre':'', 'FontSizeList':[], 'charList':[]}
  fontDict = getFontDict()
  fontDict['ttfFileList'] = ttfFileList
  lis_1 = ["const uint8_t ", "Bitmaps[] PROGMEM = {\n"]
  lis_2 = ["const gdl_Glyph_t ","Glyphs[] PROGMEM = {\n"]
  lis_3 = ["const gdl_Font_t "," PROGMEM = {\n"]
  fpd = open('font.txt', 'w+')
  for ttf in fontDict['ttfFileList']:
    (filename,suffix) = os.path.splitext(ttf)
    if suffix.upper() != '.TTF':
      continue
    for size in fontDict['FontSizeList']:
      newfilename = fontDict['FontFileNamePre'] + filename + "Font"+ str(size) +'pt'
      filename_path = fontDestPath + '\\' +newfilename
      filename_h = filename_path +'.h'
      if os.path.exists(filename_h):
          os.remove(filename_h)
      filename_txt = filename_path +'.txt'
      fp = open(filename_txt, 'w+')
      text = lis_1[0] + newfilename + lis_1[1]
      fp.write(text)
      fp.close()
      src = ttfSourcePath + '\\' + ttf
      Glyph_list = parseFont(filename_txt, src, fontDict['charList'], size, 0)
      text = '};\n\n' + lis_2[0] + newfilename + lis_2[1]
      writeFile(filename_txt, text)
      for i in Glyph_list[:len(Glyph_list) - 2]:
          writeFile(filename_txt, i)
      text = '};\n\n' + lis_3[0] + newfilename + lis_3[1]
      text += "(uint8_t *)"+ newfilename +"Bitmaps,\n"
      text += "(gdl_Glyph_t *)" + newfilename + "Glyphs,\n" + '1, 0, '+ hex(Glyph_list[len(Glyph_list)-2])+',\n};\n\n'
      writeFile(filename_txt, text)
      ModifySuffixName(filename_txt, '.h')
      text = "#include \"Fonts/"+newfilename+'.h"\n'
      fpd.write(text)
  fpd.close()
  os.system(pwdPath+'\\font.txt')
