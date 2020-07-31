/**
 * @brief Display 16bit, 24bit and 32bit BMP and JPEG/JPG pictures of JFIF format.
 */ 
#ifndef __DFROBOT_PICDECODER_SD_H__
#define __DFROBOT_PICDECODER_SD_H__

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

class DFRobot_Picdecoder_SD{
public:
/*M0*/
#if defined ARDUINO_SAM_ZERO
#define	JD_SZBUF		1024
#define JPEG_WBUF_SIZE  4096 	
/*ESP32 and ESP8266*/
#elif defined(ESP32) || defined(ESP8266)
#define	JD_SZBUF		1024
#define JPEG_WBUF_SIZE  4096 
/*AVR*/
#else
#define	JD_SZBUF		512 
#define JPEG_WBUF_SIZE  0 
#endif


//jpg解码使用的宏定义
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

#define BI_RGB	 		0  //No compression. RGB 5, 5, 5.
#define BI_RLE8 		1  //8-bit RLE compression for each pixel, the compression format consists of 2 bytes (repeated pixel count and color index);
#define BI_RLE4 		2  //4-bit RLE compression for each pixel, the compression format consists of 2 bytes
#define BI_BITFIELDS 	3  //The bit of each pixel is determined by the specified mask.


//Image information
typedef struct {
  uint32_t ImgWidth; //The actual width and height of the image
  uint32_t ImgHeight;

  uint32_t Div_Fac;  //Zoom factor (expanded by 10,000 times)

  uint32_t S_Height; //Set height and width
  uint32_t S_Width;

  uint32_t S_XOFF;   //x-axis and y-axis offset
  uint32_t S_YOFF;

  uint32_t staticx;  //The currently displayed xy coordinates
  uint32_t staticy;
} PIC_POS;

//BMP information header
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



//BMP
bool bmpDecode( uint8_t *filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));

//JPEG
int32_t jpgDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));
inline uint8_t IsElementOk(uint16_t x, uint16_t y, uint8_t chg);
int32_t  InitTag(void);
void InitTable(void);          
int32_t  Decode(void (*screenDrawPixel)(int16_t,int16_t,uint16_t));
int32_t  DecodeMCUBlock(void);
int32_t  HufBlock(uint8_t dchufindex,uint8_t achufindex);
int32_t  DecodeElement(void);         
void IQtIZzMCUComponent(int16_t flag);       
void IQtIZzBlock(int16_t  *s,int16_t * d,int16_t flag);
void AI_Drow_Init(void);
void GetYUV(int16_t flag);   
void StoreBuffer(void (*screenDrawPixel)(int16_t,int16_t,uint16_t));
void jpg_seek(uint8_t* pbase, uint8_t** pnow);
uint8_t ReadByte(void);          
void Initialize_Fast_IDCT(void);
void Fast_IDCT(int32_t * block);  
void idctrow(int32_t * blk);
void idctcol(int32_t * blk);
bool drawPicture(const char* filename,uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));//智能显示图片
uint8_t pictype(uint8_t* filename);

private:

int32_t screenWidth = 0, screenHeight = 0;
uint8_t jpg_buffer[JD_SZBUF];

#if defined ARDUINO_SAM_ZERO || defined(ESP32) || defined(ESP8266)
int16_t     SampRate_Y_H, SampRate_Y_V;
int16_t     SampRate_U_H, SampRate_U_V;
int16_t     SampRate_V_H, SampRate_V_V;
int16_t     H_YtoU, V_YtoU, H_YtoV, V_YtoV;
int16_t     Y_in_MCU, U_in_MCU, V_in_MCU;
uint8_t* lp;
int16_t     qt_table[3][64];
int16_t     comp_num;
uint8_t      comp_index[3];
uint8_t        YDcIndex, YAcIndex, UVDcIndex, UVAcIndex;
uint8_t      HufTabIndex;
int16_t* YQtTable, * UQtTable, * VQtTable;
int16_t       code_pos_table[4][16], code_len_table[4][16];
uint16_t  code_value_table[4][256];
uint16_t  huf_max_value[4][16], huf_min_value[4][16];
int16_t     BitPos, CurByte;
int16_t     rrun, vvalue;
int16_t     MCUBuffer[10 * 64];
int16_t     QtZzMCUBuffer[10 * 64];
int16_t     BlockBuffer[64];
int16_t     ycoef, ucoef, vcoef;
bool      IntervalFlag;
int16_t     interval = 0;
int16_t     Y[4 * 64], U[4 * 64], V[4 * 64];
uint32_t       sizei, sizej;
int16_t       restart;

long* iclp;
const int32_t Zig_Zag[8][8] = { {0,1,5,6,14,15,27,28},
  {2,4,7,13,16,26,29,42},
  {3,8,12,17,25,30,41,43},
  {9,11,18,24,31,40,44,53},
  {10,19,23,32,39,45,52,54},
  {20,22,33,38,46,51,55,60},
  {21,34,37,47,50,56,59,61},
  {35,36,48,49,57,58,62,63}
};
const uint8_t And[9] = { 0,1,3,7,0xf,0x1f,0x3f,0x7f,0xff };
#endif
};

#endif
