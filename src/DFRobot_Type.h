/*!
 * @file DFRobot_Type.h
 * @brief DFRobot_GDL类型定义文件
 * @n 访问ROM内存的方法
 * @n 不同主控操控通过寄存器操控IO口的方式
 *
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [Arya](xue.peng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-23
 * @https://github.com/DFRobot/DFRobot_GDL
 */
/*!
 * @file DFRobot_Type.h
 * @brief DFRobot_GDL type definition file
 * @n How to access ROM memory
 * @n different main control methods of controlling IO port through registers
 *
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [Arya] (xue.peng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-23
 * @https: //github.com/DFRobot/DFRobot_GDL
 */
#ifndef __DFROBOT_TYPE_H
#define __DFROBOT_TYPE_H

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#ifndef ARDUINO_SAM_ZERO
#include "Interface/DFRobot_DMA.h"
#endif

/*访问flash内存方式*/  /* Access to flash memory */
#ifndef pgm_read_byte
 #define pgm_read_byte(addr) (*(const unsigned char *)(addr))
#endif
#ifndef pgm_read_word
 #define pgm_read_word(addr) (*(const unsigned short *)(addr))
#endif
#ifndef pgm_read_dword
 #define pgm_read_dword(addr) (*(const unsigned long *)(addr))
#endif

#define COLOR_RGB565_BLACK     0x0000   //  black    
#define COLOR_RGB565_NAVY      0x000F   //  Navy blue  
#define COLOR_RGB565_DGREEN    0x03E0   //  dark green  
#define COLOR_RGB565_DCYAN     0x03EF   //  Teal  
#define COLOR_RGB565_MAROON    0x7800   //  Deep red      
#define COLOR_RGB565_PURPLE    0x780F   //  purple  
#define COLOR_RGB565_OLIVE     0x7BE0   //  Olive green      
#define COLOR_RGB565_LGRAY     0xC618   //  light-white
#define COLOR_RGB565_DGRAY     0x7BEF   //  Dark gray      
#define COLOR_RGB565_BLUE     0x001F   //  blue    
#define COLOR_RGB565_GREEN     0x07E0   //  green          
#define COLOR_RGB565_CYAN      0x07FF   //  cyan-blue  
#define COLOR_RGB565_RED       0xF800   //  red       
#define COLOR_RGB565_MAGENTA    0xF81F   //  magenta    
#define COLOR_RGB565_YELLOW    0xFFE0   //  yellow
#define COLOR_RGB565_ORANGE    0xFD20   //  Orange        
#define COLOR_RGB565_WHITE     0xFFFF   //  white  
#define COLOR_RGB565_PINK     0xFE19    //Pink
#define COLOR_RGB565_SKYBLUE     0x867D    //Sky blue
#define SET_COLOR     1
#define CLR_COLOR     0

#endif