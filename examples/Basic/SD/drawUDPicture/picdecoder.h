/**
 * @brief 可以显示16bit 24bit 和32bit的BMP和JFIF格式的JPEG/JPG图片
 */
#ifndef __PICDECODER_H__
#define __PICDECODER_H__

//根据arduino版本确定
#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

//打开这个宏，可以看到程序的详细运行过程
#if 0
#ifdef ARDUINO_SAM_ZERO//M0板子的串口与其他串口使用方式有区别
#define DBG(...) {SerialUSB.print("["); SerialUSB.print(__FUNCTION__); SerialUSB.print("(): "); SerialUSB.print(__LINE__); SerialUSB.print(" ] "); SerialUSB.println(__VA_ARGS__);}
#else
#define DBG(...) {Serial.print("["); Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#endif
#else
#define DBG(...)
#endif

//使用M0才可打开这个宏，打开后M0读内置U盘的图片
#define ENABLE_UD
#ifdef ENABLE_UD
#define useUD
#endif

#define	JD_SZBUF		1024	//读图片的缓冲区大小
#define JPEG_WBUF_SIZE  4096 	//定义工作区数组大小,最少应不小于3092字节

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

//函数返回值定义
#define FUNC_FALSE 0
#define FUNC_OK 1
#define FUNC_FILE_ERROR 2
#define FUNC_FORMAT_ERROR 3
//图片类型定义
#define BMPTYPE 1
#define JPGTYPE 2
//图象数据压缩的类型
#define BI_RGB	 		0  //没有压缩.RGB 5,5,5.
#define BI_RLE8 		1  //每个象素8比特的RLE压缩编码，压缩格式由2字节组成(重复象素计数和颜色索引)；
#define BI_RLE4 		2  //每个象素4比特的RLE压缩编码，压缩格式由2字节组成
#define BI_BITFIELDS 	3  //每个象素的比特由指定的掩码决定.

//BMP信息头
typedef  struct
{
  uint32_t biSize ;       //说明BITMAPINFOHEADER结构所需要的字数。
  int32_t  biWidth ;         //说明图象的宽度，以象素为单位
  int32_t  biHeight ;        //说明图象的高度，以象素为单位
  uint16_t biPlanes ;        //为目标设备说明位面数，其值将总是被设为1
  uint16_t biBitCount ;      //说明比特数/象素，其值为1、4、8、16、24、或32
  uint32_t biCompression ;//说明图象数据压缩的类型。其值可以是下述值之一：
  uint32_t biSizeImage ;  //说明图象的大小，以字节为单位。当用BI_RGB格式时，可设置为0
  int32_t  biXPelsPerMeter ;      //说明水平分辨率，用象素/米表示
  int32_t  biYPelsPerMeter ;      //说明垂直分辨率，用象素/米表示
  uint32_t biClrUsed ;    //说明位图实际使用的彩色表中的颜色索引数
  uint32_t biClrImportant ;//说明对图象显示有重要影响的颜色索引的数目，如果是0，表示都重要。
} __attribute__ ((packed)) BITMAPINFOHEADER ;

//BMP头文件
typedef  struct
{
  uint16_t bfType ;     //文件标志.只对'BM',用来识别BMP位图类型
  uint32_t bfSize ;     //文件大小,占四个字节
  uint16_t bfReserved1 ;//保留
  uint16_t bfReserved2 ;//保留
  uint32_t bfOffBits ;  //从文件开始到位图数据(bitmap data)开始之间的的偏移量
} __attribute__ ((packed)) BITMAPFILEHEADER ;

//彩色表
typedef  struct
{
  uint8_t rgbBlue ;    //指定蓝色强度
  uint8_t rgbGreen ;   //指定绿色强度
  uint8_t rgbRed ;     //指定红色强度
  uint8_t rgbReserved ;//保留，设置为0
} __attribute__ ((packed)) RGBQUAD ;

//位图信息头
typedef  struct
{
  BITMAPFILEHEADER bmfHeader;
  BITMAPINFOHEADER bmiHeader;
  //u32 RGB_MASK[3];     //调色板用于存放RGB掩码.
  //RGBQUAD bmiColors[256];
} __attribute__ ((packed)) BITMAPINFO;
typedef RGBQUAD * LPRGBQUAD;//彩色表


//BMP解码函数
bool bmpDecode( uint8_t *filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));


//JPEG 解码函数
int32_t  jpgDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));
int32_t  InitTag(void);
void InitTable(void);   //初始化数据表
int32_t  Decode(void (*screenDrawPixel)(int16_t,int16_t,uint16_t));//解码
int32_t  DecodeMCUBlock(void);
int32_t  HufBlock(uint8_t dchufindex,uint8_t achufindex);//哈夫曼解码
int32_t  DecodeElement(void);            //解码一个像素
void IQtIZzMCUComponent(int16_t flag);   //反量化
void IQtIZzBlock(int16_t  *s,int16_t * d,int16_t flag);
void GetYUV(int16_t flag);     //色彩转换的实现,得到色彩空间数据
void StoreBuffer(void (*screenDrawPixel)(int16_t,int16_t,uint16_t));

uint8_t ReadByte(void);          //从文件里面读取一个字节出来
void Initialize_Fast_IDCT(void); //初始化反离散傅立叶变换
void Fast_IDCT(int32_t * block); //快速反离散傅立叶变换
void idctrow(int32_t * blk);
void idctcol(int32_t * blk);
bool drawUDPicture(const char* filename,uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey, void (*screenDrawPixel)(int16_t,int16_t,uint16_t));//智能显示图片
uint8_t pictype(uint8_t* filename); //判断图片类型
#endif
