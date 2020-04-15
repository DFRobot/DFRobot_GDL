/**
 * @brief Can display 16bit 24bit and 32bit BMP and JFIF format JPEG / JPG pictures
 */
#ifndef __PICDECODER_H__
#define __PICDECODER_H__


#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#if 0
#ifdef ARDUINO_SAM_ZERO
#define DBG(...) {SerialUSB.print("["); SerialUSB.print(__FUNCTION__); SerialUSB.print("(): "); SerialUSB.print(__LINE__); SerialUSB.print(" ] "); SerialUSB.println(__VA_ARGS__);}
#else
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#endif
#else
#define DBG(...)
#endif



/*M0*/
#if defined ARDUINO_SAM_ZERO
#define  JD_SZBUF    1024  //Buffer size for reading pictures
#define JPEG_WBUF_SIZE  4096  //Define the size of the jpg work area array, at least 3092 bytes
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define JD_SZBUF    1024  //Buffer size for reading pictures
#define JPEG_WBUF_SIZE  4096  //Define the size of the jpg work area array, at least 3092 bytes
/* AVR series motherboard */
#elif defined(ARDUINO_AVR_UNO)
#define JD_SZBUF    66  //Buffer size for reading pictures，To ensure normal file header reading, the size must be at least larger than the bmp file header
#define JPEG_WBUF_SIZE  0       //Unable to complete jpg decoding
#else
#define  JD_SZBUF    512  //Buffer size for reading pictures
#define JPEG_WBUF_SIZE  0       //Unable to complete jpg decoding
#endif


//Macro definition for jpg decoding
#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DRI   0xdd
#define M_APP0  0xe0

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

#define MAKEWORD(a, b)      ((uint16_t)(((uint8_t)(a)) | ((uint16_t)((uint8_t)(b))) << 8))
#define MAKELONG(a, b)      ((int32_t)(((uint16_t)(a)) | ((uint32_t)((uint16_t)(b))) << 16))
#define LOWORD(l)           ((uint16_t)(l))
#define HIWORD(l)           ((uint16_t)(((uint32_t)(l) >> 16) & 0xFFFF))
#define LOBYTE(w)           ((uint8_t)(w))
#define HIBYTE(w)           ((uint8_t)(((uint16_t)(w) >> 8) & 0xFF))


#define FUNC_Err 0
#define FUNC_OK 1
#define FUNC_FILE_ERROR 2
#define FUNC_FORMAT_ERROR 3

#define BMPTYPE 1
#define JPGTYPE 2

#define BI_RGB	 		0  //没有压缩.RGB 5,5,5. No compression. RGB 5, 5, 5.
#define BI_RLE8 		1  //每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)； 8-bit RLE compression coding for each pixel, the compression format consists of 2 bytes (repeated pixel count and color index);
#define BI_RLE4 		2  //每个象素4比特的RLE压缩编码，压缩格式由2字节组成 4-bit RLE compression coding for each pixel, the compression format consists of 2 bytes
#define BI_BITFIELDS 	3  //每个象素的比特由指定的掩码决定.  The bit of each pixel is determined by the specified mask.
 
//BMP信息头  BMP information header
typedef  struct
{
  uint32_t biSize ;       //说明BITMAPINFOHEADER结构所需要的字数。 Explain the number of words required by the BITMAPINFOHEADER structure.
  int32_t  biWidth ;              //说明图象的宽度，以象素为单位  Specifies the width of the image, in pixels
  int32_t  biHeight ;             //说明图象的高度，以象素为单位  State the height of the image in pixels
  uint16_t  biPlanes ;        //为目标设备说明位面数，其值将总是被设为1  Specify the number of planes for the target device, its value will always be set to 1
  uint16_t  biBitCount ;      //说明比特数/象素，其值为1、4、8、16、24、或32   Specifies the number of bits / pixel, the value is 1, 4, 8, 16, 24, or 32
  uint32_t biCompression ;//说明图象数据压缩的类型。其值可以是下述值之一：   Explain the type of image data compression. Its value can be one of the following values:
  uint32_t biSizeImage ;  //说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0  Specifies the size of the image, in bytes. When using BI_RGB format, it can be set to 0
  int32_t  biXPelsPerMeter ;      //说明水平分辨率，用象素/米表示  Description of horizontal resolution, expressed in pixels / meter
  int32_t  biYPelsPerMeter ;      //说明垂直分辨率，用象素/米表示  Description of vertical resolution, expressed in pixels / meter
  uint32_t biClrUsed ;    //说明位图实际使用的彩色表中的颜色索引数  Explain the number of color indexes in the color table actually used by the bitmap
  uint32_t biClrImportant ;//说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。Explain the number of color indexes that have an important influence on the image display. If it is 0, it means that all are important.
} __attribute__ ((packed)) BITMAPINFOHEADER ;

//BMP头文件 BMP header file
typedef  struct
{
  uint16_t  bfType ;        //文件标志.只对'BM',用来识别BMP位图类型  File logo. Only for 'BM', used to identify BMP bitmap type
  uint32_t bfSize ;     //文件大小,占四个字节  File size, takes up four bytes
  uint16_t  bfReserved1 ;   //reserve
  uint16_t  bfReserved2 ;   //reserve
  uint32_t bfOffBits ;  //从文件开始到位图数据(bitmap data)开始之间的的偏移量  Offset from the start of the file to the start of bitmap data
} __attribute__ ((packed)) BITMAPFILEHEADER ;

//彩色表  Color table
// typedef  struct
// {
  // uint8_t rgbBlue ;    //指定蓝色强度  Specify blue intensity
  // uint8_t rgbGreen ;   //指定绿色强度  Specify green intensity
  // uint8_t rgbRed ;     //指定红色强度  Specify red intensity
  // uint8_t rgbReserved ;//reserve，Set to 0
// } __attribute__ ((packed)) RGBQUAD ;
// typedef RGBQUAD * LPRGBQUAD;

//位图信息头  Bitmap information header
typedef  struct
{
  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER bmiHeader;
  //u32 RGB_MASK[3];     //调色板用于存放RGB掩码.  The palette is used to store RGB masks.
  //RGBQUAD bmiColors[256];
} __attribute__ ((packed)) BITMAPINFO;




bool bmpDecode( uint8_t *filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));


int32_t jpgDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));
int32_t  InitTag(void);
void InitTable(void);             
int32_t  Decode(void (*screenDrawPixel)(int16_t,int16_t,uint16_t));
int32_t  DecodeMCUBlock(void);
int32_t  HufBlock(uint8_t dchufindex,uint8_t achufindex);
int32_t  DecodeElement(void);           
void IQtIZzMCUComponent(int16_t flag);  
void IQtIZzBlock(int16_t  *s,int16_t * d,int16_t flag);
void GetYUV(int16_t flag);     
void StoreBuffer(void (*screenDrawPixel)(int16_t,int16_t,uint16_t));

uint8_t ReadByte(void);            
void Initialize_Fast_IDCT(void); 
void Fast_IDCT(int32_t * block);  
void idctrow(int32_t * blk);
void idctcol(int32_t * blk);
bool drawSDPicture(const char* filename,uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));//智能显示图片
uint8_t pictype(uint8_t* filename); 
uint8_t pictype(uint8_t* filename); 
#endif
