#include "picdecoder.h"

#include <SD.h>
File picfile;  //定义SD库的File类型变量为全局变量，用于指向打开文件、获取位置、偏移、关闭文件


//全局变量声明,BMP和JPEG共用
int32_t screenWidth = 0, screenHeight = 0;//屏幕窗口大小
uint8_t jpg_buffer[JD_SZBUF];//数据缓存区
//图像信息
typedef struct
{
  uint32_t ImgWidth; //图像的实际宽度和高度
  uint32_t ImgHeight;

  uint32_t Div_Fac;  //缩放系数 (扩大了10000倍的)

  uint32_t S_Height; //设定的高度和宽度
  uint32_t S_Width;

  uint32_t S_XOFF;   //x轴和y轴的偏移量
  uint32_t S_YOFF;

  uint32_t staticx;  //当前显示到的ｘｙ坐标
  uint32_t staticy;
} PIC_POS;
PIC_POS PICINFO;//图像位置信息


//BMP/JPG共用的函数
//初始化智能画图
void AI_Drow_Init(void)
{
  float temp, temp1;
  temp = (float)PICINFO.S_Width / PICINFO.ImgWidth;
  temp1 = (float)PICINFO.S_Height / PICINFO.ImgHeight;

  if (temp < temp1)temp1 = temp;//取较小的那个
  if (temp1 > 1)temp1 = 1;
  //使图片处于所给区域的中间
  PICINFO.S_XOFF += (PICINFO.S_Width - temp1 * PICINFO.ImgWidth) / 2;
  PICINFO.S_YOFF += (PICINFO.S_Height - temp1 * PICINFO.ImgHeight) / 2;
  temp1 *= 10000;//扩大10000倍
  PICINFO.Div_Fac = temp1;

  PICINFO.staticx = 5000;
  PICINFO.staticy = 5000;//放到一个不可能的值上面
}
//判断这个像素是否可以显示
//(x,y) :像素原始坐标
//chg   :功能变量.
//返回值:0,不需要显示.1,需要显示
inline uint8_t IsElementOk(uint16_t x, uint16_t y, uint8_t chg)
{
  if (x != PICINFO.staticx || y != PICINFO.staticy)
  {
    if (chg == 1)
    {
      PICINFO.staticx = x;
      PICINFO.staticy = y;
    }
    return 1;
  }
  else return 0;
}
//判断图片格式
uint8_t pictype(uint8_t* filename)
{
  if (filename[strlen((const char*)filename) - 1] == 'p'||filename[strlen((const char*)filename) - 1] == 'P') return BMPTYPE;
  else if(filename[strlen((const char*)filename) - 1] == 'g'||filename[strlen((const char*)filename) - 1] == 'G') return JPGTYPE;
  DBG("File format error");
  return FUNC_Err;
}
//打开BMP图片，解码显示
bool bmpDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  uint16_t count;
  // uint8_t res;
  uint8_t  rgb, color_byte;
  uint8_t R, G, B;
  uint16_t x, y,color;
  uint16_t uiTemp;    //x轴方向像素计数器
  uint16_t countpix = 0;//记录像素
  //x,y的实际坐标
  uint16_t realx = 0;
  uint16_t realy = 0;
  uint8_t  yok = 1;
  uint8_t  bitype;//判断压缩类型
  BITMAPINFO* pbmp;//存放bmp图片头部所有信息的临时指针

  picfile = SD.open((const char*)filename, FILE_READ);//打开文件
  if(!picfile)
  {
    DBG("open fail");
    return false;
  }
  picfile.read(jpg_buffer, JD_SZBUF);//读取一次
  pbmp = (BITMAPINFO*)jpg_buffer;//得到BMP的头部信息
  (uint8_t*)jpg_buffer;
  count = pbmp->bmfHeader.bfOffBits;        //数据偏移,得到数据段的开始地址
  color_byte = pbmp->bmiHeader.biBitCount / 8;//彩色位 16/24/32
  PICINFO.ImgHeight = pbmp->bmiHeader.biHeight;//得到图片高度
  PICINFO.ImgWidth = pbmp->bmiHeader.biWidth;  //得到图片宽度
  // DBG(pbmp->bmfHeader.bfOffBits);//从文件开始到位图数据
  // DBG(pbmp->bmiHeader.biBitCount);//说明象素，1、4、8、16、24、或32
  // DBG(pbmp->bmiHeader.biWidth);//说明图象的宽度
  // DBG(pbmp->bmiHeader.biHeight);//说明图象的高度
  // DBG(pbmp->bmiHeader.biCompression);//说明图象数据压缩的类型
  
  //水平像素必须是4的倍数!!
  if ((PICINFO.ImgWidth * color_byte) % 4)
    uiTemp = ((PICINFO.ImgWidth * color_byte) / 4 + 1) * 4;
  else
    uiTemp = PICINFO.ImgWidth * color_byte;
  AI_Drow_Init();//初始化智能画图
  //开始解码BMP
  x = 0;  
  y = PICINFO.ImgHeight;
  rgb = 0;
  realy = y * PICINFO.Div_Fac / 10000;
  while (1)
  {
    while (count < JD_SZBUF)  //读取一簇512扇区 (SectorsPerClust 每簇扇区数)
    {
      if (color_byte == 3)   //24位颜色图
      {
        switch (rgb)
        {
        case 0:
          B=jpg_buffer[count];
          break;
        case 1:
          G=jpg_buffer[count];
          break;
        case 2:
          R=jpg_buffer[count];
          break;
        }
      }
      else if (color_byte == 2)  //16位
      {
        switch (rgb)
        {
        case 0: //gggbbbbb
          B = (jpg_buffer[count] & 0x1f )<<3;
          G = jpg_buffer[count] >> 5;
          break;
        case 1:
          if (pbmp->bmiHeader.biCompression == BI_RGB) //RGB555颜色图为没有压缩 （0rrrrrgg gggbbbbb）
          {
            G |= (jpg_buffer[count] & 0x3) << 3;
            G *= 8;
            R = jpg_buffer[count] << 1;
          }
          else //RGB565  （rrrrrggg gggbbbbb）
          {
            G |= (jpg_buffer[count] & 0x7)<<3;
            G *= 4;
            R = jpg_buffer[count];
          }
          break;
        }
      }
      else if (color_byte == 4)//32位颜色图
      {
        switch (rgb)
        {
        case 0:
          B = jpg_buffer[count];
          break;
        case 1:
          G = jpg_buffer[count];
          break;
        case 2:
          R = jpg_buffer[count];
          break;
        case 3:
          break;
        }
      }
      //位图颜色得到
      rgb++;
      count++;
      if (rgb == color_byte) //水平方向读取到1像素数数据后显示
      {
        if (x < PICINFO.ImgWidth)
        {
          realx = x * PICINFO.Div_Fac / 10000;//x轴实际值
          if (IsElementOk(realx, realy, 1) && yok)//符合条件
          {
            color = R >> 3;
            color = color << 6;
            color |= (G >> 2);
            color = color << 5;
            color |= (B >> 3);
            //画点
            screenDrawPixel(realx + PICINFO.S_XOFF, realy + PICINFO.S_YOFF, color);
          }
        }
        x++;//x轴增加一个像素
        rgb = 0;
      }
      countpix++;//像素累加
      if (countpix >= uiTemp)//水平方向像素值到了.换行
      {
        y--;
        if (y <= 0)
        {
          return true;//BMP显示结束.
        }
        realy = y * PICINFO.Div_Fac / 10000;//实际y值改变
        if (IsElementOk(realx, realy, 0))yok = 1;//此处不改变PICINFO.staticx,y的值
        else yok = 0;
        x = 0;
        countpix = 0;
        rgb = 0;
      }
    }
    picfile.read(jpg_buffer, JD_SZBUF);//读出
    if(!jpg_buffer)
    {
      DBG("read err");
      return false;
    }
    count = 0;
  }
}

/*M0, ESP32 and ESP8266可以支持jpg解码*/
#if defined ARDUINO_SAM_ZERO || defined(ESP32) || defined(ESP8266)
//定义在JPEG函数里面用到的变量
int16_t     SampRate_Y_H, SampRate_Y_V;
int16_t     SampRate_U_H, SampRate_U_V;
int16_t     SampRate_V_H, SampRate_V_V;
int16_t     H_YtoU, V_YtoU, H_YtoV, V_YtoV;
int16_t     Y_in_MCU, U_in_MCU, V_in_MCU;
uint8_t* lp;//取代lpJpegBuf
int16_t     qt_table[3][64];
int16_t     comp_num;
uint8_t      comp_index[3];
uint8_t        YDcIndex, YAcIndex, UVDcIndex, UVAcIndex;
uint8_t      HufTabIndex;
int16_t* YQtTable, * UQtTable, * VQtTable;
int16_t       code_pos_table[4][16], code_len_table[4][16];
uint16_t  code_value_table[4][256];
uint16_t  huf_max_value[4][16], huf_min_value[4][16];
int16_t     BitPos, CurByte;//byte的第几位,当前byte
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
//反Z字形编码表
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
//数据缓冲区
uint8_t SPI_FLASH_BUF[JPEG_WBUF_SIZE];//jpg工作区
long* iclip = (long*)SPI_FLASH_BUF;

//FileName:要显示的图片文件  BMP/JPG/JPEG
//(sx,sy) :开始显示的坐标点
//(ex,ey) :结束显示的坐标点
//图片会在开始和结束的坐标点范围内显示
//(*screenDrawPixel)(int16_t,int16_t,uint16_t):传入的函数指针，用于画点
bool drawSDPicture(const char* filename, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey,void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  //窗口大小
  screenWidth = ex, screenHeight = ey;

  int32_t funcret;//返回值
  //得到显示方框大小
  uint8_t fileTpe;
  int32_t index;
  for (index = 0; index < JD_SZBUF; index++)
    iclip[index] = 0;
  if (ey > sy)PICINFO.S_Height = ey - sy;
  else PICINFO.S_Height = sy - ey;
  if (ex > sx)PICINFO.S_Width = ex - sx;
  else PICINFO.S_Width = sx - ex;
  //显示区域无效
  if (PICINFO.S_Height == 0 || PICINFO.S_Width == 0)
  {
    PICINFO.S_Height = screenHeight;
    PICINFO.S_Width = screenWidth;
    DBG("Display area invalid");
    return false;
  }
  //显示的开始坐标点
  PICINFO.S_YOFF = sy;
  PICINFO.S_XOFF = sx;

  fileTpe = pictype((uint8_t*)filename);  //得到文件的类型
  if (fileTpe == BMPTYPE)//得到一个BMP图像
  {
    DBG("draw bmp");
    funcret = bmpDecode((uint8_t*)filename, screenDrawPixel);
  }
  else if (fileTpe == JPGTYPE)//得到JPG/JPEG图片
  {
    DBG("draw jpg");
    funcret = jpgDecode((uint8_t*)filename, screenDrawPixel);
  }
  else
  {
    picfile.close();
    DBG("format err");
    return false;  //格式不正确!!!
  }
  picfile.close();//关闭SD库对象
  if (funcret == FUNC_OK)
  {
    //DBG("have completed");
    return true;//解码成功
  }
  else
  {
    DBG("Decode false");
    return false;   //解码失败
  }
}

//对指针地址进行改变!
//pc    :当前指针
//返回值:当前指针的减少量.在d_buffer里面自动进行了偏移
void jpg_seek(uint8_t* pbase, uint8_t** pnow)
{
  uint32_t pos;
  uint16_t offset;
  offset = *pnow - pbase;//当前的偏移量.
  if (offset > JD_SZBUF-24)//将要结束了,做下一次读取
  {
    pos = picfile.position();//得到当前位置
    picfile.seek(pos -JD_SZBUF + offset);//因为之前读了1024还没有用完，所以指向没有用到的位置开始读
    picfile.read(pbase, JD_SZBUF); //读取1024个字节.
    *pnow = pbase;//复位
  }
}

//初始化d_buffer的数据
int32_t InitTag(void)
{
  bool finish = false;
  uint8_t id;
  int16_t  llength;
  int16_t  i, j, k;
  int16_t  huftab1, huftab2;
  int16_t  huftabindex;
  uint8_t hf_table_index;
  uint8_t qt_table_index;
  uint8_t comnum;//最长为256个字节

  uint8_t* lptemp;
  int16_t  colorount;

  lp = jpg_buffer + 2;//跳过两个字节SOI(0xFF，0xD8 Start of Image)
//  lp-=P_Cal(lp);
  jpg_seek(jpg_buffer, &lp);
  while (!finish)
  {
    id = *(lp + 1);//取出低位字节(高位在前，低位在后)
    lp += 2;     //跳过取出的字节
    //lp-=P_Cal(lp);
    jpg_seek(jpg_buffer, &lp);
    switch (id)
    {
    case M_APP0: //JFIF APP0 segment marker (0xE0)
      //标志应用数据段的开始
      llength = MAKEWORD(*(lp + 1), *lp);//得到应用数据段长度
      lp += llength;
      //  lp-=P_Cal(lp);
      jpg_seek(jpg_buffer, &lp);
      break;
    case M_DQT: //定义量化表标记(0xFF,0xDB)
      llength = MAKEWORD(*(lp + 1), *lp);//(量化表长度)两个字节
      qt_table_index = (*(lp + 2)) & 0x0f;//量化表信息bit 0..3: QT 号(0..3, 否则错误)
      //bit 4..7: QT 精度, 0 = 8 bit, 否则 16 bit
      lptemp = lp + 3;            //n 字节的 QT, n = 64*(精度+1)
      //d_buffer里面至少有有512个字节的余度,这里最大用到128个字节
      if (llength < 80)         //精度为 8 bit
      {
        for (i = 0; i < 64; i++)qt_table[qt_table_index][i] = (int16_t)*(lptemp++);
      }
      else              //精度为 16 bit
      {
        for (i = 0; i < 64; i++)qt_table[qt_table_index][i] = (int16_t)*(lptemp++);
        qt_table_index = (*(lptemp++)) & 0x0f;
        for (i = 0; i < 64; i++)qt_table[qt_table_index][i] = (int16_t)*(lptemp++);
      }
      lp += llength;  //跳过量化表
      //lp-=P_Cal(lp);
      jpg_seek(jpg_buffer, &lp);
      break;
    case M_SOF0:                  //帧开始 (baseline JPEG 0xFF,0xC0)
      llength = MAKEWORD(*(lp + 1), *lp);   //长度 (高字节, 低字节), 8+components*3
      PICINFO.ImgHeight = MAKEWORD(*(lp + 4), *(lp + 3));//图片高度 (高字节, 低字节), 如果不支持 DNL 就必须 >0
      PICINFO.ImgWidth = MAKEWORD(*(lp + 6), *(lp + 5));  //图片宽度 (高字节, 低字节), 如果不支持 DNL 就必须 >0
      comp_num = *(lp + 7);//components 数量(1 uint8_t), 灰度图是 1, YCbCr/YIQ 彩色图是 3, CMYK 彩色图是 4
      if ((comp_num != 1) && (comp_num != 3))
      {
        DBG("FUNC_FORMAT_ERROR");
        return FUNC_FORMAT_ERROR;
      }// 格式错误
      if (comp_num == 3)            //YCbCr/YIQ 彩色图
      {
        comp_index[0] = *(lp + 8);      //component id (1 = Y, 2 = Cb, 3 = Cr, 4 = I, 5 = Q)
        SampRate_Y_H = (*(lp + 9)) >> 4;  //水平采样系数
        SampRate_Y_V = (*(lp + 9)) & 0x0f;//垂直采样系数
        YQtTable = (int16_t*)qt_table[*(lp + 10)];//通过量化表号取得量化表地址

        comp_index[1] = *(lp + 11);        //component id
        SampRate_U_H = (*(lp + 12)) >> 4;      //水平采样系数
        SampRate_U_V = (*(lp + 12)) & 0x0f;    //垂直采样系数
        UQtTable = (int16_t*)qt_table[*(lp + 13)];//通过量化表号取得量化表地址

        comp_index[2] = *(lp + 14);        //component id
        SampRate_V_H = (*(lp + 15)) >> 4;      //水平采样系数
        SampRate_V_V = (*(lp + 15)) & 0x0f;    //垂直采样系数
        VQtTable = (int16_t*)qt_table[*(lp + 16)];//通过量化表号取得量化表地址
      }
      else                     //component id
      {
        comp_index[0] = *(lp + 8);
        SampRate_Y_H = (*(lp + 9)) >> 4;
        SampRate_Y_V = (*(lp + 9)) & 0x0f;
        YQtTable = (int16_t*)qt_table[*(lp + 10)];//灰度图的量化表都一样

        comp_index[1] = *(lp + 8);
        SampRate_U_H = 1;
        SampRate_U_V = 1;
        UQtTable = (int16_t*)qt_table[*(lp + 10)];

        comp_index[2] = *(lp + 8);
        SampRate_V_H = 1;
        SampRate_V_V = 1;
        VQtTable = (int16_t*)qt_table[*(lp + 10)];
      }
      lp += llength;
      jpg_seek(jpg_buffer, &lp);
      break;
    case M_DHT: //定义哈夫曼表(0xFF,0xC4)
      llength = MAKEWORD(*(lp + 1), *lp);//长度 (高字节, 低字节)
      if (llength < 0xd0)       // Huffman Table信息 (1 uint8_t)
      {
        huftab1 = (int16_t)(*(lp + 2)) >> 4;     //huftab1=0,1(HT 类型,0 = DC 1 = AC)
        huftab2 = (int16_t)(*(lp + 2)) & 0x0f;   //huftab2=0,1(HT 号  ,0 = Y  1 = UV)
        huftabindex = huftab1 * 2 + huftab2;   //0 = YDC 1 = UVDC 2 = YAC 3 = UVAC
        lptemp = lp + 3;//!!!
        //在这里可能出现余度不够,多于512字节,则会导致出错!!!!
        for (i = 0; i < 16; i++)             //16 bytes: 长度是 1..16 代码的符号数
          code_len_table[huftabindex][i] = (int16_t)(*(lptemp++));//码长为i的码字个数
        j = 0;
        for (i = 0; i < 16; i++)       //得出HT的所有码字的对应值
        {
          if (code_len_table[huftabindex][i] != 0)
          {
            k = 0;
            while (k < code_len_table[huftabindex][i])
            {
              code_value_table[huftabindex][k + j] = (int16_t)(*(lptemp++));//最可能的出错地方
              k++;
            }
            j += k;
          }
        }
        i = 0;
        while (code_len_table[huftabindex][i] == 0)i++;
        for (j = 0; j < i; j++)
        {
          huf_min_value[huftabindex][j] = 0;
          huf_max_value[huftabindex][j] = 0;
        }
        huf_min_value[huftabindex][i] = 0;
        huf_max_value[huftabindex][i] = code_len_table[huftabindex][i] - 1;
        for (j = i + 1; j < 16; j++)
        {
          huf_min_value[huftabindex][j] = (huf_max_value[huftabindex][j - 1] + 1) << 1;
          huf_max_value[huftabindex][j] = huf_min_value[huftabindex][j] + code_len_table[huftabindex][j] - 1;
        }
        code_pos_table[huftabindex][0] = 0;
        for (j = 1; j < 16; j++)
          code_pos_table[huftabindex][j] = code_len_table[huftabindex][j - 1] + code_pos_table[huftabindex][j - 1];
        lp += llength;
        //lp-=P_Cal(lp);
        jpg_seek(jpg_buffer, &lp);
      }//if
      else
      {
        hf_table_index = *(lp + 2);
        lp += 2;
        //lp-=P_Cal(lp);
        jpg_seek(jpg_buffer, &lp);
        while (hf_table_index != 0xff)
        {
          huftab1 = (int16_t)hf_table_index >> 4;     //huftab1=0,1
          huftab2 = (int16_t)hf_table_index & 0x0f;   //huftab2=0,1
          huftabindex = huftab1 * 2 + huftab2;
          lptemp = lp + 1;
          colorount = 0;
          for (i = 0; i < 16; i++)
          {
            code_len_table[huftabindex][i] = (int16_t)(*(lptemp++));
            colorount += code_len_table[huftabindex][i];
          }
          colorount += 17;
          j = 0;
          for (i = 0; i < 16; i++)
          {
            if (code_len_table[huftabindex][i] != 0)
            {
              k = 0;
              while (k < code_len_table[huftabindex][i])
              {
                code_value_table[huftabindex][k + j] = (int16_t)(*(lptemp++));//最可能出错的地方,余度不够
                k++;
              }
              j += k;
            }
          }
          i = 0;
          while (code_len_table[huftabindex][i] == 0)i++;
          for (j = 0; j < i; j++)
          {
            huf_min_value[huftabindex][j] = 0;
            huf_max_value[huftabindex][j] = 0;
          }
          huf_min_value[huftabindex][i] = 0;
          huf_max_value[huftabindex][i] = code_len_table[huftabindex][i] - 1;
          for (j = i + 1; j < 16; j++)
          {
            huf_min_value[huftabindex][j] = (huf_max_value[huftabindex][j - 1] + 1) << 1;
            huf_max_value[huftabindex][j] = huf_min_value[huftabindex][j] + code_len_table[huftabindex][j] - 1;
          }
          code_pos_table[huftabindex][0] = 0;
          for (j = 1; j < 16; j++)
            code_pos_table[huftabindex][j] = code_len_table[huftabindex][j - 1] + code_pos_table[huftabindex][j - 1];
          lp += colorount;
          //  lp-=P_Cal(lp);
          jpg_seek(jpg_buffer, &lp);
          hf_table_index = *lp;
        }  //while
      }  //else
      break;
    case M_DRI://定义差分编码累计复位的间隔
      llength = MAKEWORD(*(lp + 1), *lp);
      restart = MAKEWORD(*(lp + 3), *(lp + 2));
      lp += llength;
      //lp-=P_Cal(lp);
      jpg_seek(jpg_buffer, &lp);
      break;
    case M_SOS:  //扫描开始 12字节
      llength = MAKEWORD(*(lp + 1), *lp);
      comnum = *(lp + 2);
      if (comnum != comp_num)
      {
        DBG("FUNC_FORMAT_ERROR");
        return FUNC_FORMAT_ERROR; //格式错误
      }
      lptemp = lp + 3;//这里也可能出现错误
      //这里也可能出错,但是几率比较小了
      for (i = 0; i < comp_num; i++)//每组件的信息
      {
        if (*lptemp == comp_index[0])
        {
          YDcIndex = (*(lptemp + 1)) >> 4;   //Y 使用的 Huffman 表
          YAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
        }
        else
        {
          UVDcIndex = (*(lptemp + 1)) >> 4;   //U,V
          UVAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
        }
        lptemp += 2;//comp_num<256,但是2*comp_num+3可能>=512
      }
      lp += llength;
      //lp-=P_Cal(lp);
      jpg_seek(jpg_buffer, &lp);
      finish = true;
      break;
    case M_EOI:
      DBG("pic end");
      return FUNC_FORMAT_ERROR;//图片结束 标记
    default:
      if ((id & 0xf0) != 0xd0)
      {
        llength = MAKEWORD(*(lp + 1), *lp);
        lp += llength;
        //lp-=P_Cal(lp);
        jpg_seek(jpg_buffer, &lp);
      }
      else lp += 2;
      break;
    }  //switch
  } //while
  return FUNC_OK;
}

//初始化量化表，全部清零
void InitTable(void)
{
  int16_t i, j;
  sizei = sizej = 0;
  PICINFO.ImgWidth = PICINFO.ImgHeight = 0;
  rrun = vvalue = 0;
  BitPos = 0;
  CurByte = 0;
  IntervalFlag = false;
  restart = 0;
  for (i = 0; i < 3; i++) //量化表
    for (j = 0; j < 64; j++)
      qt_table[i][j] = 0;
  comp_num = 0;
  HufTabIndex = 0;
  for (i = 0; i < 3; i++)
    comp_index[i] = 0;
  for (i = 0; i < 4; i++)
    for (j = 0; j < 16; j++)
    {
      code_len_table[i][j] = 0;
      code_pos_table[i][j] = 0;
      huf_max_value[i][j] = 0;
      huf_min_value[i][j] = 0;
    }
  for (i = 0; i < 4; i++)
    for (j = 0; j < 256; j++)
      code_value_table[i][j] = 0;

  for (i = 0; i < 10 * 64; i++)
  {
    MCUBuffer[i] = 0;
    QtZzMCUBuffer[i] = 0;
  }
  for (i = 0; i < 64; i++)
  {
    Y[i] = 0;
    U[i] = 0;
    V[i] = 0;
    BlockBuffer[i] = 0;
  }
  ycoef = ucoef = vcoef = 0;
}

//调用顺序: Initialize_Fast_IDCT() :初始化
//          DecodeMCUBlock()       Huffman Decode
//          IQtIZzMCUComponent()   反量化、反DCT
//          GetYUV()               Get Y U V
//          StoreBuffer()          YUV to RGB
int32_t Decode(void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  int32_t funcret;
  Y_in_MCU = SampRate_Y_H * SampRate_Y_V;//YDU YDU YDU YDU
  U_in_MCU = SampRate_U_H * SampRate_U_V;//cRDU
  V_in_MCU = SampRate_V_H * SampRate_V_V;//cBDU
  H_YtoU = SampRate_Y_H / SampRate_U_H;
  V_YtoU = SampRate_Y_V / SampRate_U_V;
  H_YtoV = SampRate_Y_H / SampRate_V_H;
  V_YtoV = SampRate_Y_V / SampRate_V_V;
  Initialize_Fast_IDCT();
  while ((funcret = DecodeMCUBlock()) == FUNC_OK) //After Call DecodeMCUBUBlock()
  {
    interval++;                 //The Digital has been Huffman Decoded and
    if ((restart) && (interval % restart == 0))//be stored in MCUBuffer(YDU,YDU,YDU,YDU
      IntervalFlag = true;          // UDU,VDU) Every DU := 8*8
    else
      IntervalFlag = false;
    IQtIZzMCUComponent(0); //反量化 and IDCT The Data in QtZzMCUBuffer
    IQtIZzMCUComponent(1);
    IQtIZzMCUComponent(2);
    GetYUV(0);         //得到Y cR cB
    GetYUV(1);
    GetYUV(2);
    StoreBuffer(screenDrawPixel);       //To RGB
    sizej += SampRate_Y_H * 8;
    if (sizej >= PICINFO.ImgWidth)
    {
      sizej = 0;
      sizei += SampRate_Y_V * 8;
    }
    if ((sizej == 0) && (sizei >= PICINFO.ImgHeight)) break;
  }
  return funcret;
}

// 入口 QtZzMCUBuffer 出口 Y[] U[] V[]
//得到YUV色彩空间
void  GetYUV(int16_t flag)
{
  int16_t H, VV;
  int16_t i, j, k, h;
  int16_t* buf = NULL;
  int16_t* pQtZzMCU = NULL;
  switch (flag)
  {
  case 0://亮度分量
    H = SampRate_Y_H;
    VV = SampRate_Y_V;
    buf = Y;
    pQtZzMCU = QtZzMCUBuffer;
    break;
  case 1://红色分量
    H = SampRate_U_H;
    VV = SampRate_U_V;
    buf = U;
    pQtZzMCU = QtZzMCUBuffer + Y_in_MCU * 64;
    break;
  case 2://蓝色分量
    H = SampRate_V_H;
    VV = SampRate_V_V;
    buf = V;
    pQtZzMCU = QtZzMCUBuffer + (Y_in_MCU + U_in_MCU) * 64;
    break;
  }
  for (i = 0; i < VV; i++)
    for (j = 0; j < H; j++)
      for (k = 0; k < 8; k++)
        for (h = 0; h < 8; h++)
          buf[(i * 8 + k) * SampRate_Y_H * 8 + j * 8 + h] = *pQtZzMCU++;
}

//将解出的字按RGB形式存储 lpbmp (BGR),(BGR) ......入口Y[] U[] V[] 出口lpPtr
void StoreBuffer(void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  int16_t i = 0, j = 0;
  uint8_t R, G, B;
  int32_t y, u, v, rr, gg, bb;
  uint16_t color;
  //x,y的实际坐标
  uint16_t realx = sizej;
  uint16_t realy = 0;
  for (i = 0; i < SampRate_Y_V * 8; i++)
  {
    if ((sizei + i) < PICINFO.ImgHeight)// sizei表示行 sizej 表示列
    {
      realy = PICINFO.Div_Fac * (sizei + i) / 10000;//实际Y坐标
      //在这里不改变PICINFO.staticx和PICINFO.staticy的值 ,如果在这里改变,则会造成每块的第一个点不显示!!!
      if (!IsElementOk(realx, realy, 0))continue;//列值是否满足条件? 寻找满足条件的列

      for (j = 0; j < SampRate_Y_H * 8; j++)
      {
        if ((sizej + j) < PICINFO.ImgWidth)
        {
          realx = PICINFO.Div_Fac * (sizej + j) / 10000;//实际X坐标
          //在这里改变PICINFO.staticx和PICINFO.staticy的值
          if (!IsElementOk(realx, realy, 1))continue;//列值是否满足条件? 寻找满足条件的行

          y = Y[i * 8 * SampRate_Y_H + j];
          u = U[(i / V_YtoU) * 8 * SampRate_Y_H + j / H_YtoU];
          v = V[(i / V_YtoV) * 8 * SampRate_Y_H + j / H_YtoV];
          rr = ((y << 8) + 18 * u + 367 * v) >> 8;
          gg = ((y << 8) - 159 * u - 220 * v) >> 8;
          bb = ((y << 8) + 411 * u - 29 * v) >> 8;
          R = (uint8_t)rr;
          G = (uint8_t)gg;
          B = (uint8_t)bb;
          if (rr & 0xffffff00) if (rr > 255) R = 255;
            else if (rr < 0) R = 0;
          if (gg & 0xffffff00) if (gg > 255) G = 255;
            else if (gg < 0) G = 0;
          if (bb & 0xffffff00) if (bb > 255) B = 255;
            else if (bb < 0) B = 0;
          color = R >> 3;
          color = color << 6;
          color |= (G >> 2);
          color = color << 5;
          color |= (B >> 3);

          screenDrawPixel(realx + PICINFO.S_XOFF, realy + PICINFO.S_YOFF, color);
        }
        else break;
      }
    }
    else break;
  }
}
//Huffman Decode   MCU 出口 MCUBuffer  入口Blockbuffer[  ]
int32_t DecodeMCUBlock(void)
{
  int16_t* lpMCUBuffer;
  int16_t i, j;
  int32_t funcret;
  if (IntervalFlag)//差值复位
  {
    lp += 2;
    //  lp-=P_Cal(lp);
    jpg_seek(jpg_buffer, &lp);
    ycoef = ucoef = vcoef = 0;
    BitPos = 0;
    CurByte = 0;
  }
  switch (comp_num)
  {
  case 3:  //comp_num 指图的类型（彩色图、灰度图）
    lpMCUBuffer = MCUBuffer;
    for (i = 0; i < SampRate_Y_H * SampRate_Y_V; i++)  //Y
    {
      funcret = HufBlock(YDcIndex, YAcIndex);//解码4 * (8*8)
      if (funcret != FUNC_OK)
        return funcret;
      BlockBuffer[0] = BlockBuffer[0] + ycoef;//直流分量是差值，所以要累加。
      ycoef = BlockBuffer[0];
      for (j = 0; j < 64; j++)
        *lpMCUBuffer++ = BlockBuffer[j];
    }
    for (i = 0; i < SampRate_U_H * SampRate_U_V; i++)  //U
    {
      funcret = HufBlock(UVDcIndex, UVAcIndex);
      if (funcret != FUNC_OK)
        return funcret;
      BlockBuffer[0] = BlockBuffer[0] + ucoef;
      ucoef = BlockBuffer[0];
      for (j = 0; j < 64; j++)
        *lpMCUBuffer++ = BlockBuffer[j];
    }
    for (i = 0; i < SampRate_V_H * SampRate_V_V; i++)  //V
    {
      funcret = HufBlock(UVDcIndex, UVAcIndex);
      if (funcret != FUNC_OK)
        return funcret;
      BlockBuffer[0] = BlockBuffer[0] + vcoef;
      vcoef = BlockBuffer[0];
      for (j = 0; j < 64; j++)
        *lpMCUBuffer++ = BlockBuffer[j];
    }
    break;
  case 1: //灰度图
    lpMCUBuffer = MCUBuffer;
    funcret = HufBlock(YDcIndex, YAcIndex);
    if (funcret != FUNC_OK)
      return funcret;
    BlockBuffer[0] = BlockBuffer[0] + ycoef;
    ycoef = BlockBuffer[0];
    for (j = 0; j < 64; j++)
      *lpMCUBuffer++ = BlockBuffer[j];
    for (i = 0; i < 128; i++)
      *lpMCUBuffer++ = 0;
    break;
  default:
    DBG("FUNC_FORMAT_ERROR");
    return FUNC_FORMAT_ERROR;
  }
  return FUNC_OK;
}
//Huffman Decode （8*8） DU   出口 Blockbuffer[ ] 入口 vvalue
int32_t HufBlock(uint8_t dchufindex, uint8_t achufindex)
{
  int16_t count = 0;
  int16_t i;
  int32_t funcret;
  //dc
  HufTabIndex = dchufindex;
  funcret = DecodeElement();
  if (funcret != FUNC_OK)return funcret;
  BlockBuffer[count++] = vvalue;//解出的直流系数
  //ac
  HufTabIndex = achufindex;
  while (count < 64)
  {
    funcret = DecodeElement();
    if (funcret != FUNC_OK)
      return funcret;
    if ((rrun == 0) && (vvalue == 0))
    {
      for (i = count; i < 64; i++)BlockBuffer[i] = 0;
      count = 64;
    }
    else
    {
      for (i = 0; i < rrun; i++)BlockBuffer[count++] = 0;//前面的零
      BlockBuffer[count++] = vvalue;//解出的值
    }
  }
  return FUNC_OK;
}
//Huffman 解码  每个元素   出口 vvalue 入口 读文件ReadByte
int32_t DecodeElement()
{
  int32_t thiscode, tempcode;
  uint16_t temp, valueex;
  int16_t codelen;
  uint8_t hufexbyte, runsize, tempsize, sign;
  uint8_t newbyte, lastbyte;

  if (BitPos >= 1) //BitPos指示当前比特位置
  {
    BitPos--;
    thiscode = (uint8_t)CurByte >> BitPos;//取一个比特
    CurByte = CurByte & And[BitPos];   //清除取走的比特位
  }
  else                 //取出的一个字节已用完
  {
    //新取
    lastbyte = ReadByte();       //读出一个字节
    BitPos--;            //and[]:=0x0,0x1,0x3,0x7,0xf,0x1f,0x2f,0x3f,0x4f
    newbyte = CurByte & And[BitPos];
    thiscode = lastbyte >> 7;
    CurByte = newbyte;
  }
  codelen = 1;
  //与Huffman表中的码字匹配，直自找到为止
  while ((thiscode < huf_min_value[HufTabIndex][codelen - 1]) ||
         (code_len_table[HufTabIndex][codelen - 1] == 0) ||
         (thiscode > huf_max_value[HufTabIndex][codelen - 1]))
  {
    if (BitPos >= 1)//取出的一个字节还有
    {
      BitPos--;
      tempcode = (uint8_t)CurByte >> BitPos;
      CurByte = CurByte & And[BitPos];
    }
    else
    {
      lastbyte = ReadByte();
      BitPos--;
      newbyte = CurByte & And[BitPos];
      tempcode = (uint8_t)lastbyte >> 7;
      CurByte = newbyte;
    }
    thiscode = (thiscode << 1) + tempcode;
    codelen++;
    if (codelen > 16)
    {
      DBG("FUNC_FORMAT_ERROR");
      return FUNC_FORMAT_ERROR;
    }
  }  //while
  temp = thiscode - huf_min_value[HufTabIndex][codelen - 1] + code_pos_table[HufTabIndex][codelen - 1];
  hufexbyte = (uint8_t)code_value_table[HufTabIndex][temp];
  rrun = (int16_t)(hufexbyte >> 4);  //一个字节中，高四位是其前面的零的个数。
  runsize = hufexbyte & 0x0f;    //后四位为后面字的尺寸
  if (runsize == 0)
  {
    vvalue = 0;
    return FUNC_OK;
  }
  tempsize = runsize;
  if (BitPos >= runsize)
  {
    BitPos -= runsize;
    valueex = (uint8_t)CurByte >> BitPos;
    CurByte = CurByte & And[BitPos];
  }
  else
  {
    valueex = CurByte;
    tempsize -= BitPos;
    while (tempsize > 8)
    {
      lastbyte = ReadByte();
      valueex = (valueex << 8) + (uint8_t)lastbyte;
      tempsize -= 8;
    }  //while
    lastbyte = ReadByte();
    BitPos -= tempsize;
    valueex = (valueex << tempsize) + (lastbyte >> BitPos);
    CurByte = lastbyte & And[BitPos];
  }  //else
  sign = valueex >> (runsize - 1);
  if (sign)vvalue = valueex;//解出的码值
  else
  {
    valueex = valueex ^ 0xffff;
    temp = 0xffff << runsize;
    vvalue = -(int16_t)(valueex ^ temp);
  }
  return FUNC_OK;
}
//反量化MCU中的每个组件   入口 MCUBuffer 出口 QtZzMCUBuffer
void IQtIZzMCUComponent(int16_t flag)
{
  int16_t H, VV;
  int16_t i, j;
  int16_t* pQtZzMCUBuffer = NULL;
  int16_t* pMCUBuffer = NULL;

  switch (flag)
  {
  case 0:
    H = SampRate_Y_H;
    VV = SampRate_Y_V;
    pMCUBuffer = MCUBuffer;
    pQtZzMCUBuffer = QtZzMCUBuffer;
    break;
  case 1:
    H = SampRate_U_H;
    VV = SampRate_U_V;
    pMCUBuffer = MCUBuffer + Y_in_MCU * 64;
    pQtZzMCUBuffer = QtZzMCUBuffer + Y_in_MCU * 64;
    break;
  case 2:
    H = SampRate_V_H;
    VV = SampRate_V_V;
    pMCUBuffer = MCUBuffer + (Y_in_MCU + U_in_MCU) * 64;
    pQtZzMCUBuffer = QtZzMCUBuffer + (Y_in_MCU + U_in_MCU) * 64;
    break;
  }
  for (i = 0; i < VV; i++)
    for (j = 0; j < H; j++)
      IQtIZzBlock(pMCUBuffer + (i * H + j) * 64, pQtZzMCUBuffer + (i * H + j) * 64, flag);
}
//要量化的字
//反量化 8*8 DU
void IQtIZzBlock(int16_t* s, int16_t* d, int16_t flag)
{
  int16_t i, j;
  int16_t tag;
  int16_t* pQt = NULL;
  int32_t buffer2[8][8];
  int32_t* buffer1;
  int16_t offset;

  switch (flag)
  {
  case 0:   //亮度
    pQt = YQtTable;
    offset = 128;
    break;
  case 1:   //红
    pQt = UQtTable;
    offset = 0;
    break;
  case 2:   //蓝
    pQt = VQtTable;
    offset = 0;
    break;
  }

  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++)
    {
      tag = Zig_Zag[i][j];
      buffer2[i][j] = (int32_t)s[tag] * (int32_t)pQt[tag];
    }
  buffer1 = (int32_t*)buffer2;
  Fast_IDCT(buffer1);//反DCT
  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++)
      d[i * 8 + j] = buffer2[i][j] + offset;
}
//快速反DCT
void Fast_IDCT(int32_t* block)
{
  int16_t i;
  for (i = 0; i < 8; i++)idctrow(block + 8 * i);
  for (i = 0; i < 8; i++)idctcol(block + i);
}
//从源文件读取一个字节
uint8_t ReadByte(void)
{
  uint8_t i;
  i = *lp++;
  //lp-=P_Cal(lp);//经过P_Cal的处理,把指针移动
  jpg_seek(jpg_buffer, &lp);
  if (i == 0xff)lp++;
  BitPos = 8;
  CurByte = i;
  return i;
}
//初始化快速反DCT
void Initialize_Fast_IDCT(void)
{
  int16_t i;

  iclp = iclip + 512;
  for (i = -512; i < 512; i++)
    iclp[i] = (i < -256) ? -256 : ((i > 255) ? 255 : i);
}
////////////////////////////////////////////////////////////////////////
void idctrow(int32_t* blk)
{
  int32_t x0, x1, x2, x3, x4, x5, x6, x7, x8;
  //intcut
  if (!((x1 = blk[4] << 11) | (x2 = blk[6]) | (x3 = blk[2]) |
        (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
  {
    blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
    return;
  }
  x0 = (blk[0] << 11) + 128; // for proper rounding in the fourth stage
  //first stage
  x8 = W7 * (x4 + x5);
  x4 = x8 + (W1 - W7) * x4;
  x5 = x8 - (W1 + W7) * x5;
  x8 = W3 * (x6 + x7);
  x6 = x8 - (W3 - W5) * x6;
  x7 = x8 - (W3 + W5) * x7;
  //second stage
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6 * (x3 + x2);
  x2 = x1 - (W2 + W6) * x2;
  x3 = x1 + (W2 - W6) * x3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  //third stage
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181 * (x4 + x5) + 128) >> 8;
  x4 = (181 * (x4 - x5) + 128) >> 8;
  //fourth stage
  blk[0] = (x7 + x1) >> 8;
  blk[1] = (x3 + x2) >> 8;
  blk[2] = (x0 + x4) >> 8;
  blk[3] = (x8 + x6) >> 8;
  blk[4] = (x8 - x6) >> 8;
  blk[5] = (x0 - x4) >> 8;
  blk[6] = (x3 - x2) >> 8;
  blk[7] = (x7 - x1) >> 8;
}
//////////////////////////////////////////////////////////////////////////////
void idctcol(int32_t* blk)
{
  int32_t x0, x1, x2, x3, x4, x5, x6, x7, x8;
  //intcut
  if (!((x1 = (blk[8 * 4] << 8)) | (x2 = blk[8 * 6]) | (x3 = blk[8 * 2]) |
        (x4 = blk[8 * 1]) | (x5 = blk[8 * 7]) | (x6 = blk[8 * 5]) | (x7 = blk[8 * 3])))
  {
    blk[8 * 0] = blk[8 * 1] = blk[8 * 2] = blk[8 * 3] = blk[8 * 4] = blk[8 * 5]
                                           = blk[8 * 6] = blk[8 * 7] = iclp[(blk[8 * 0] + 32) >> 6];
    return;
  }
  x0 = (blk[8 * 0] << 8) + 8192;
  //first stage
  x8 = W7 * (x4 + x5) + 4;
  x4 = (x8 + (W1 - W7) * x4) >> 3;
  x5 = (x8 - (W1 + W7) * x5) >> 3;
  x8 = W3 * (x6 + x7) + 4;
  x6 = (x8 - (W3 - W5) * x6) >> 3;
  x7 = (x8 - (W3 + W5) * x7) >> 3;
  //second stage
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6 * (x3 + x2) + 4;
  x2 = (x1 - (W2 + W6) * x2) >> 3;
  x3 = (x1 + (W2 - W6) * x3) >> 3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  //third stage
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181 * (x4 + x5) + 128) >> 8;
  x4 = (181 * (x4 - x5) + 128) >> 8;
  //fourth stage
  blk[8 * 0] = iclp[(x7 + x1) >> 14];
  blk[8 * 1] = iclp[(x3 + x2) >> 14];
  blk[8 * 2] = iclp[(x0 + x4) >> 14];
  blk[8 * 3] = iclp[(x8 + x6) >> 14];
  blk[8 * 4] = iclp[(x8 - x6) >> 14];
  blk[8 * 5] = iclp[(x0 - x4) >> 14];
  blk[8 * 6] = iclp[(x3 - x2) >> 14];
  blk[8 * 7] = iclp[(x7 - x1) >> 14];
}

//打开jpg图片，解码显示
int32_t jpgDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{

  picfile = SD.open((const char*)filename, FILE_READ);//打开文件
  if (!picfile)
  {
    DBG("open fail");
    return FUNC_Err;
  }

  int32_t funcret;//返回值
  picfile.read(jpg_buffer,JD_SZBUF);
  //初始化量化表
  InitTable();
  if ((funcret = InitTag()) != FUNC_OK)return FUNC_Err;
  if ((SampRate_Y_H == 0) || (SampRate_Y_V == 0))return FUNC_Err;
  //智能画图初始化
  AI_Drow_Init();
  //解码并逐行画点
  funcret = Decode(screenDrawPixel);
  return funcret;
}

/*AVR系列主板只支持解码bmp图片*/
#else

//FileName:要显示的图片文件  BMP/JPG/JPEG
//(sx,sy) :开始显示的坐标点
//(ex,ey) :结束显示的坐标点
//图片会在开始和结束的坐标点范围内显示
//(*screenDrawPixel)(int16_t,int16_t,uint16_t):传入的函数指针，用于画点
bool drawSDPicture(const char* filename, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey,void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  //窗口大小
  screenWidth = ex, screenHeight = ey;

  int32_t funcret;//返回值
  //得到显示方框大小
  uint8_t fileTpe;
  // int32_t index;
  // for (index = 0; index < JD_SZBUF; index++)
    // iclip[index] = 0;
  if (ey > sy)PICINFO.S_Height = ey - sy;
  else PICINFO.S_Height = sy - ey;
  if (ex > sx)PICINFO.S_Width = ex - sx;
  else PICINFO.S_Width = sx - ex;
  //显示区域无效
  if (PICINFO.S_Height == 0 || PICINFO.S_Width == 0)
  {
    PICINFO.S_Height = screenHeight;
    PICINFO.S_Width = screenWidth;
    DBG("Display area invalid");
    return false;
  }
  //显示的开始坐标点
  PICINFO.S_YOFF = sy;
  PICINFO.S_XOFF = sx;
  fileTpe = pictype((uint8_t*)filename);  //得到文件的类型
  if (fileTpe == BMPTYPE)//得到一个BMP图像
  {
    DBG("draw bmp");
    funcret = bmpDecode((uint8_t*)filename, screenDrawPixel);
  }
  else if (fileTpe == JPGTYPE)//得到JPG/JPEG图片
  {
    DBG("Unsupported jpg");
    funcret = FUNC_OK;
  }
  picfile.close();//关闭SD库对象
  if (funcret == FUNC_OK)return true;//解码成功
  else
  {
    DBG("BMPDecode false");
    return false;   //解码失败
  }
}
#endif