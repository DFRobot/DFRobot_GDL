/**
 * @brief Display 16bit, 24bit and 32bit BMP and JPEG/JPG pictures of JFIF format.
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

//Only M0 can open this macro. After opening, M0 reads the pictures in the built-in U disk
#define ENABLE_UD
#ifdef ENABLE_UD
#define useUD
#endif

#define	JD_SZBUF		1024	
#define JPEG_WBUF_SIZE  4096 	


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

#define FUNC_FALSE 0
#define FUNC_OK 1
#define FUNC_FILE_ERROR 2
#define FUNC_FORMAT_ERROR 3

#define BMPTYPE 1
#define JPGTYPE 2

#define BI_RGB	 		0  //No compression. RGB 5, 5, 5.
#define BI_RLE8 		1  //8-bit RLE compression for each pixel, the compression format consists of 2 bytes (repeated pixel count and color index);
#define BI_RLE4 		2  //4-bit RLE compression for each pixel, the compression format consists of 2 bytes
#define BI_BITFIELDS 	3  //The bit of each pixel is determined by the specified mask.
 
//BMP information header
typedef  struct
{
  uint32_t biSize ;       //The number of words required by the BITMAPINFOHEADER structure.
  int32_t  biWidth ;              //The width of the image, in pixels
  int32_t  biHeight ;             //The height of the image in pixels
  uint16_t  biPlanes ;        //The number of planes for the target device, its value will always be set to 1
  uint16_t  biBitCount ;      //The number of bits / pixel, could be 1, 4, 8, 16, 24, or 32
  uint32_t biCompression ;//The compression type of image data. Its value can be one of the followings:
  uint32_t biSizeImage ;  //The size of the image, in bytes. When using BI_RGB format, it can be set to 0
  int32_t  biXPelsPerMeter ;      //Thorizontal resolution, in pixels/meter
  int32_t  biYPelsPerMeter ;      //The vertical resolution, in pixels/meter
  uint32_t biClrUsed ;    //The number of color indexes in the color table actually used by the bitmap
  uint32_t biClrImportant ;//The number of color indexes that has an important influence on the image display. If it is 0, it means that all are important.
} __attribute__ ((packed)) BITMAPINFOHEADER ;

//BMP header file
typedef  struct
{
  uint16_t  bfType ;        //File logo. Only for 'BM', used to identify BMP bitmap type
  uint32_t bfSize ;     //File size, takes up four bytes
  uint16_t  bfReserved1 ;   //reserve
  uint16_t  bfReserved2 ;   //reserve
  uint32_t bfOffBits ;  //Offset from the start of the file to the start of bitmap data
} __attribute__ ((packed)) BITMAPFILEHEADER ;

//Color table
// typedef  struct
// {
  // uint8_t rgbBlue ;    //Specify blue intensity
  // uint8_t rgbGreen ;   //Specify green intensity
  // uint8_t rgbRed ;     //Specify red intensity
  // uint8_t rgbReserved ;//reserve，Set to 0
// } __attribute__ ((packed)) RGBQUAD ;
// typedef RGBQUAD * LPRGBQUAD;

//Bitmap information header
typedef  struct
{
  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER bmiHeader;
  //u32 RGB_MASK[3];     //The palette is used to store RGB masks.
  //RGBQUAD bmiColors[256];
} __attribute__ ((packed)) BITMAPINFO;




bool bmpDecode( uint8_t *filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));


int32_t  jpgDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));
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
bool drawUDPicture(const char* filename,uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));//智能显示图片
uint8_t pictype(uint8_t* filename); 
#endif
