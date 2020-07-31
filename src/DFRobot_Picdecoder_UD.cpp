#include "DFRobot_Picdecoder_UD.h"


#ifdef ENABLE_UD
#include <UD.h>
UDFile picFile_M0;//Define the File type variable of the UD library as a global variable, used to point to open file, get location, offset, or close file
#else
#include <SD.h>
File picFile_M0; //Define the File type variable of SD library as a global variable, used to point to open file, get location, offset, or close file
#endif


DFRobot_Picdecoder_UD::PIC_POS PICINFO_M0;//Image location information
//BMP/JPG shared functions
//Initialize intelligent drawing
void DFRobot_Picdecoder_UD::AI_Drow_Init(void)
{
  float temp, temp1;
  temp = (float)PICINFO_M0.S_Width / PICINFO_M0.ImgWidth;
  temp1 = (float)PICINFO_M0.S_Height / PICINFO_M0.ImgHeight;

  if (temp < temp1)temp1 = temp;
  if (temp1 > 1)temp1 = 1;
  PICINFO_M0.S_XOFF += (PICINFO_M0.S_Width - temp1 * PICINFO_M0.ImgWidth) / 2;
  PICINFO_M0.S_YOFF += (PICINFO_M0.S_Height - temp1 * PICINFO_M0.ImgHeight) / 2;
  temp1 *= 10000;
  PICINFO_M0.Div_Fac = temp1;

  PICINFO_M0.staticx = 5000;
  PICINFO_M0.staticy = 5000;
}
//Determine if the pixel is displayed
inline uint8_t DFRobot_Picdecoder_UD::IsElementOk(uint16_t x, uint16_t y, uint8_t chg)
{
  if (x != PICINFO_M0.staticx || y != PICINFO_M0.staticy)
  {
    if (chg == 1)
    {
      PICINFO_M0.staticx = x;
      PICINFO_M0.staticy = y;
    }
    return 1;
  }
  else return 0;
}
//Determine image format
uint8_t DFRobot_Picdecoder_UD::pictype(uint8_t* filename)
{
  if (filename[strlen((const char*)filename) - 1] == 'p'||filename[strlen((const char*)filename) - 1] == 'P') return BMPTYPE;
  else if(filename[strlen((const char*)filename) - 1] == 'g'||filename[strlen((const char*)filename) - 1] == 'G') return JPGTYPE;
  DBG("File format error");
  return FUNC_Err;
}

bool DFRobot_Picdecoder_UD::bmpDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  uint16_t count;
  uint8_t  rgb, color_byte;
  uint8_t R, G, B;
  uint16_t x, y,color;
  uint16_t uiTemp;    
  uint16_t countpix = 0;
  uint16_t realx = 0;
  uint16_t realy = 0;
  uint8_t  yok = 1;
  uint8_t  bitype;
  BITMAPINFO* pbmp;
  #ifdef ENABLE_UD
  SerialUSB.println((const char*)filename);
    picFile_M0 = UD.open((const char*)filename, FILE_READ);
  #else
    picFile_M0 = SD.open((const char*)filename, FILE_READ);
  #endif
  if(!picFile_M0)
    {
      DBG("open fail");
      return false;
    }
  picFile_M0.read(jpg_buffer, JD_SZBUF);
  pbmp = (BITMAPINFO*)jpg_buffer;
  (uint8_t*)jpg_buffer;
  count = pbmp->bmfHeader.bfOffBits;
  color_byte = pbmp->bmiHeader.biBitCount / 8;
  PICINFO_M0.ImgHeight = pbmp->bmiHeader.biHeight;
  PICINFO_M0.ImgWidth = pbmp->bmiHeader.biWidth;  
  
  if ((PICINFO_M0.ImgWidth * color_byte) % 4)
    uiTemp = ((PICINFO_M0.ImgWidth * color_byte) / 4 + 1) * 4;
  else
    uiTemp = PICINFO_M0.ImgWidth * color_byte;
  AI_Drow_Init();
  
  x = 0;  
  y = PICINFO_M0.ImgHeight;
  rgb = 0;
  realy = y * PICINFO_M0.Div_Fac / 10000;
  while (1)
  {
    while (count < JD_SZBUF)  
    {
      if (color_byte == 3)   
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
      else if (color_byte == 2) 
      {
        switch (rgb)
        {
        case 0: //gggbbbbb
          B = (jpg_buffer[count] & 0x1f )<<3;
          G = jpg_buffer[count] >> 5;
          break;
        case 1:
          if (pbmp->bmiHeader.biCompression == BI_RGB)
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
      else if (color_byte == 4)
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
      rgb++;
      count++;
      if (rgb == color_byte)
      {
        if (x < PICINFO_M0.ImgWidth)
        {
          realx = x * PICINFO_M0.Div_Fac / 10000;
          if (IsElementOk(realx, realy, 1) && yok)
          {
            color = R >> 3;
            color = color << 6;
            color |= (G >> 2);
            color = color << 5;
            color |= (B >> 3);
            screenDrawPixel(realx + PICINFO_M0.S_XOFF, realy + PICINFO_M0.S_YOFF, color);
          }
        }
        x++;
        rgb = 0;
      }
      countpix++;
      if (countpix >= uiTemp)
      {
        y--;
        if (y <= 0)
        {
          return true;
        }
        realy = y * PICINFO_M0.Div_Fac / 10000;
        if (IsElementOk(realx, realy, 0))yok = 1;
        else yok = 0;
        x = 0;
        countpix = 0;
        rgb = 0;
      }
    }
    picFile_M0.read(jpg_buffer, JD_SZBUF);
    if(!jpg_buffer)
    {
      DBG("read err");
      return false;
    }
    count = 0;
  }
}

/*M0, ESP32 and ESP8266 supports jpg decoding*/
#if defined ARDUINO_SAM_ZERO || defined(ESP32) || defined(ESP8266)

uint8_t SPI_FLASH_BUF_M0[JPEG_WBUF_SIZE];
long* iclip_M0 = (long*)SPI_FLASH_BUF_M0;

bool DFRobot_Picdecoder_UD::drawPicture(const char* filename, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey,void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  screenWidth = ex, screenHeight = ey;

  int32_t funcret;
  uint8_t fileTpe;
  int32_t index;
  for (index = 0; index < JD_SZBUF; index++)
    iclip_M0[index] = 0;
  if (ey > sy)PICINFO_M0.S_Height = ey - sy;
  else PICINFO_M0.S_Height = sy - ey;
  if (ex > sx)PICINFO_M0.S_Width = ex - sx;
  else PICINFO_M0.S_Width = sx - ex;
  if (PICINFO_M0.S_Height == 0 || PICINFO_M0.S_Width == 0)
  {
    PICINFO_M0.S_Height = screenHeight;
    PICINFO_M0.S_Width = screenWidth;
    DBG("Display area invalid");
    return false;
  }
  PICINFO_M0.S_YOFF = sy;
  PICINFO_M0.S_XOFF = sx;

  fileTpe = pictype((uint8_t*)filename); 
  if (fileTpe == BMPTYPE)
  {
    DBG("draw bmp");
    funcret = bmpDecode((uint8_t*)filename, screenDrawPixel);
  }
  else if (fileTpe == JPGTYPE)
  {
    DBG("draw jpg");
    funcret = jpgDecode((uint8_t*)filename, screenDrawPixel);
  }
  else
  {
    picFile_M0.close();
    DBG("format err");
    return false;
  }
  picFile_M0.close();
  if (funcret == FUNC_OK)
  {
    return true;
  }
  else
  {
    DBG("Decode false");
    return false;
  }
}

void DFRobot_Picdecoder_UD::jpg_seek(uint8_t* pbase, uint8_t** pnow)
{
  uint32_t pos;
  uint16_t offset;
  offset = *pnow - pbase;
  if (offset > JD_SZBUF-24)
  {
    pos = picFile_M0.position();
    picFile_M0.seek(pos -JD_SZBUF + offset);
    picFile_M0.read(pbase, JD_SZBUF);
    *pnow = pbase;
  }
}

int32_t DFRobot_Picdecoder_UD::InitTag(void)
{
  bool finish = false;
  uint8_t id;
  int16_t  llength;
  int16_t  i, j, k;
  int16_t  huftab1, huftab2;
  int16_t  huftabindex;
  uint8_t hf_table_index;
  uint8_t qt_table_index;
  uint8_t comnum;

  uint8_t* lptemp;
  int16_t  colorount;

  lp = jpg_buffer + 2;
  jpg_seek(jpg_buffer, &lp);
  while (!finish)
  {
    id = *(lp + 1);
    lp += 2;  
    jpg_seek(jpg_buffer, &lp);
    switch (id)
    {
    case M_APP0:
      llength = MAKEWORD(*(lp + 1), *lp);
      lp += llength;
      jpg_seek(jpg_buffer, &lp);
      break;
    case M_DQT: 
      llength = MAKEWORD(*(lp + 1), *lp);
      qt_table_index = (*(lp + 2)) & 0x0f;
      lptemp = lp + 3;           
      if (llength < 80)    
      {
        for (i = 0; i < 64; i++)qt_table[qt_table_index][i] = (int16_t)*(lptemp++);
      }
      else
      {
        for (i = 0; i < 64; i++)qt_table[qt_table_index][i] = (int16_t)*(lptemp++);
        qt_table_index = (*(lptemp++)) & 0x0f;
        for (i = 0; i < 64; i++)qt_table[qt_table_index][i] = (int16_t)*(lptemp++);
      }
      lp += llength; 
      jpg_seek(jpg_buffer, &lp);
      break;
    case M_SOF0:
      llength = MAKEWORD(*(lp + 1), *lp); 
      PICINFO_M0.ImgHeight = MAKEWORD(*(lp + 4), *(lp + 3));
      PICINFO_M0.ImgWidth = MAKEWORD(*(lp + 6), *(lp + 5)); 
      comp_num = *(lp + 7);
      if ((comp_num != 1) && (comp_num != 3))
      {
        DBG("FUNC_FORMAT_ERROR");
        return FUNC_FORMAT_ERROR;
      }
      if (comp_num == 3)        
      {
        comp_index[0] = *(lp + 8);
        SampRate_Y_H = (*(lp + 9)) >> 4;
        SampRate_Y_V = (*(lp + 9)) & 0x0f;
        YQtTable = (int16_t*)qt_table[*(lp + 10)];

        comp_index[1] = *(lp + 11);
        SampRate_U_H = (*(lp + 12)) >> 4;
        SampRate_U_V = (*(lp + 12)) & 0x0f;
        UQtTable = (int16_t*)qt_table[*(lp + 13)];

        comp_index[2] = *(lp + 14);
        SampRate_V_H = (*(lp + 15)) >> 4;      
        SampRate_V_V = (*(lp + 15)) & 0x0f;    
        VQtTable = (int16_t*)qt_table[*(lp + 16)];
      }
      else
      {
        comp_index[0] = *(lp + 8);
        SampRate_Y_H = (*(lp + 9)) >> 4;
        SampRate_Y_V = (*(lp + 9)) & 0x0f;
        YQtTable = (int16_t*)qt_table[*(lp + 10)];

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
    case M_DHT: 
      llength = MAKEWORD(*(lp + 1), *lp);
      if (llength < 0xd0)      
      {
        huftab1 = (int16_t)(*(lp + 2)) >> 4;    
        huftab2 = (int16_t)(*(lp + 2)) & 0x0f;   
        huftabindex = huftab1 * 2 + huftab2; 
        lptemp = lp + 3;
        
        for (i = 0; i < 16; i++)        
          code_len_table[huftabindex][i] = (int16_t)(*(lptemp++));
        j = 0;
        for (i = 0; i < 16; i++)  
        {
          if (code_len_table[huftabindex][i] != 0)
          {
            k = 0;
            while (k < code_len_table[huftabindex][i])
            {
              code_value_table[huftabindex][k + j] = (int16_t)(*(lptemp++));
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
        jpg_seek(jpg_buffer, &lp);
      }
      else
      {
        hf_table_index = *(lp + 2);
        lp += 2;
        jpg_seek(jpg_buffer, &lp);
        while (hf_table_index != 0xff)
        {
          huftab1 = (int16_t)hf_table_index >> 4;
          huftab2 = (int16_t)hf_table_index & 0x0f;
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
                code_value_table[huftabindex][k + j] = (int16_t)(*(lptemp++));
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
          jpg_seek(jpg_buffer, &lp);
          hf_table_index = *lp;
        } 
      } 
      break;
    case M_DRI:
      llength = MAKEWORD(*(lp + 1), *lp);
      restart = MAKEWORD(*(lp + 3), *(lp + 2));
      lp += llength;
      jpg_seek(jpg_buffer, &lp);
      break;
    case M_SOS: 
      llength = MAKEWORD(*(lp + 1), *lp);
      comnum = *(lp + 2);
      if (comnum != comp_num)
      {
        DBG("FUNC_FORMAT_ERROR");
        return FUNC_FORMAT_ERROR; 
      }
      lptemp = lp + 3;
      for (i = 0; i < comp_num; i++)
      {
        if (*lptemp == comp_index[0])
        {
          YDcIndex = (*(lptemp + 1)) >> 4;
          YAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
        }
        else
        {
          UVDcIndex = (*(lptemp + 1)) >> 4;
          UVAcIndex = ((*(lptemp + 1)) & 0x0f) + 2;
        }
        lptemp += 2;
      }
      lp += llength;
      jpg_seek(jpg_buffer, &lp);
      finish = true;
      break;
    case M_EOI:
      DBG("pic end");
      return FUNC_FORMAT_ERROR;
    default:
      if ((id & 0xf0) != 0xd0)
      {
        llength = MAKEWORD(*(lp + 1), *lp);
        lp += llength;
        jpg_seek(jpg_buffer, &lp);
      }
      else lp += 2;
      break;
    }
  }
  return FUNC_OK;
}

void DFRobot_Picdecoder_UD::InitTable(void)
{
  int16_t i, j;
  sizei = sizej = 0;
  PICINFO_M0.ImgWidth = PICINFO_M0.ImgHeight = 0;
  rrun = vvalue = 0;
  BitPos = 0;
  CurByte = 0;
  IntervalFlag = false;
  restart = 0;
  for (i = 0; i < 3; i++) 
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

int32_t DFRobot_Picdecoder_UD::Decode(void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  int32_t funcret;
  Y_in_MCU = SampRate_Y_H * SampRate_Y_V;
  U_in_MCU = SampRate_U_H * SampRate_U_V;
  V_in_MCU = SampRate_V_H * SampRate_V_V;
  H_YtoU = SampRate_Y_H / SampRate_U_H;
  V_YtoU = SampRate_Y_V / SampRate_U_V;
  H_YtoV = SampRate_Y_H / SampRate_V_H;
  V_YtoV = SampRate_Y_V / SampRate_V_V;
  Initialize_Fast_IDCT();
  while ((funcret = DecodeMCUBlock()) == FUNC_OK) 
  {
    interval++;       
    if ((restart) && (interval % restart == 0))//be stored in MCUBuffer(YDU,YDU,YDU,YDU
      IntervalFlag = true;          // UDU,VDU) Every DU := 8*8
    else
      IntervalFlag = false;
    IQtIZzMCUComponent(0); 
    IQtIZzMCUComponent(1);
    IQtIZzMCUComponent(2);
    GetYUV(0);         
    GetYUV(1);
    GetYUV(2);
    StoreBuffer(screenDrawPixel);       //To RGB
    sizej += SampRate_Y_H * 8;
    if (sizej >= PICINFO_M0.ImgWidth)
    {
      sizej = 0;
      sizei += SampRate_Y_V * 8;
    }
    if ((sizej == 0) && (sizei >= PICINFO_M0.ImgHeight)) break;
  }
  return funcret;
}

void  DFRobot_Picdecoder_UD::GetYUV(int16_t flag)
{
  int16_t H, VV;
  int16_t i, j, k, h;
  int16_t* buf = NULL;
  int16_t* pQtZzMCU = NULL;
  switch (flag)
  {
  case 0:
    H = SampRate_Y_H;
    VV = SampRate_Y_V;
    buf = Y;
    pQtZzMCU = QtZzMCUBuffer;
    break;
  case 1:
    H = SampRate_U_H;
    VV = SampRate_U_V;
    buf = U;
    pQtZzMCU = QtZzMCUBuffer + Y_in_MCU * 64;
    break;
  case 2:
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

void DFRobot_Picdecoder_UD::StoreBuffer(void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  int16_t i = 0, j = 0;
  uint8_t R, G, B;
  int32_t y, u, v, rr, gg, bb;
  uint16_t color;
  uint16_t realx = sizej;
  uint16_t realy = 0;
  for (i = 0; i < SampRate_Y_V * 8; i++)
  {
    if ((sizei + i) < PICINFO_M0.ImgHeight)
    {
      realy = PICINFO_M0.Div_Fac * (sizei + i) / 10000;
      if (!IsElementOk(realx, realy, 0))continue;

      for (j = 0; j < SampRate_Y_H * 8; j++)
      {
        if ((sizej + j) < PICINFO_M0.ImgWidth)
        {
          realx = PICINFO_M0.Div_Fac * (sizej + j) / 10000;
          if (!IsElementOk(realx, realy, 1))continue;

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

          screenDrawPixel(realx + PICINFO_M0.S_XOFF, realy + PICINFO_M0.S_YOFF, color);
        }
        else break;
      }
    }
    else break;
  }
}
int32_t DFRobot_Picdecoder_UD::DecodeMCUBlock(void)
{
  int16_t* lpMCUBuffer;
  int16_t i, j;
  int32_t funcret;
  if (IntervalFlag)
  {
    lp += 2;
    jpg_seek(jpg_buffer, &lp);
    ycoef = ucoef = vcoef = 0;
    BitPos = 0;
    CurByte = 0;
  }
  switch (comp_num)
  {
  case 3: 
    lpMCUBuffer = MCUBuffer;
    for (i = 0; i < SampRate_Y_H * SampRate_Y_V; i++)
    {
      funcret = HufBlock(YDcIndex, YAcIndex);
      if (funcret != FUNC_OK)
        return funcret;
      BlockBuffer[0] = BlockBuffer[0] + ycoef;
      ycoef = BlockBuffer[0];
      for (j = 0; j < 64; j++)
        *lpMCUBuffer++ = BlockBuffer[j];
    }
    for (i = 0; i < SampRate_U_H * SampRate_U_V; i++)
    {
      funcret = HufBlock(UVDcIndex, UVAcIndex);
      if (funcret != FUNC_OK)
        return funcret;
      BlockBuffer[0] = BlockBuffer[0] + ucoef;
      ucoef = BlockBuffer[0];
      for (j = 0; j < 64; j++)
        *lpMCUBuffer++ = BlockBuffer[j];
    }
    for (i = 0; i < SampRate_V_H * SampRate_V_V; i++)
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
  case 1: 
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
int32_t DFRobot_Picdecoder_UD::HufBlock(uint8_t dchufindex, uint8_t achufindex)
{
  int16_t count = 0;
  int16_t i;
  int32_t funcret;
  HufTabIndex = dchufindex;
  funcret = DecodeElement();
  if (funcret != FUNC_OK)return funcret;
  BlockBuffer[count++] = vvalue;
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
      for (i = 0; i < rrun; i++)BlockBuffer[count++] = 0;
      BlockBuffer[count++] = vvalue;
    }
  }
  return FUNC_OK;
}
int32_t DFRobot_Picdecoder_UD::DecodeElement()
{
  int32_t thiscode, tempcode;
  uint16_t temp, valueex;
  int16_t codelen;
  uint8_t hufexbyte, runsize, tempsize, sign;
  uint8_t newbyte, lastbyte;

  if (BitPos >= 1)
  {
    BitPos--;
    thiscode = (uint8_t)CurByte >> BitPos;
    CurByte = CurByte & And[BitPos];  
  }
  else
  {
    lastbyte = ReadByte();
    BitPos--;    
    newbyte = CurByte & And[BitPos];
    thiscode = lastbyte >> 7;
    CurByte = newbyte;
  }
  codelen = 1;
  while ((thiscode < huf_min_value[HufTabIndex][codelen - 1]) ||
         (code_len_table[HufTabIndex][codelen - 1] == 0) ||
         (thiscode > huf_max_value[HufTabIndex][codelen - 1]))
  {
    if (BitPos >= 1)
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
  }
  temp = thiscode - huf_min_value[HufTabIndex][codelen - 1] + code_pos_table[HufTabIndex][codelen - 1];
  hufexbyte = (uint8_t)code_value_table[HufTabIndex][temp];
  rrun = (int16_t)(hufexbyte >> 4);
  runsize = hufexbyte & 0x0f;
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
    } 
    lastbyte = ReadByte();
    BitPos -= tempsize;
    valueex = (valueex << tempsize) + (lastbyte >> BitPos);
    CurByte = lastbyte & And[BitPos];
  } 
  sign = valueex >> (runsize - 1);
  if (sign)vvalue = valueex;
  else
  {
    valueex = valueex ^ 0xffff;
    temp = 0xffff << runsize;
    vvalue = -(int16_t)(valueex ^ temp);
  }
  return FUNC_OK;
}
void DFRobot_Picdecoder_UD::IQtIZzMCUComponent(int16_t flag)
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
void DFRobot_Picdecoder_UD::IQtIZzBlock(int16_t* s, int16_t* d, int16_t flag)
{
  int16_t i, j;
  int16_t tag;
  int16_t* pQt = NULL;
  int32_t buffer2[8][8];
  int32_t* buffer1;
  int16_t offset;

  switch (flag)
  {
  case 0:
    pQt = YQtTable;
    offset = 128;
    break;
  case 1:
    pQt = UQtTable;
    offset = 0;
    break;
  case 2:
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
  Fast_IDCT(buffer1);
  for (i = 0; i < 8; i++)
    for (j = 0; j < 8; j++)
      d[i * 8 + j] = buffer2[i][j] + offset;
}
void DFRobot_Picdecoder_UD::Fast_IDCT(int32_t* block)
{
  int16_t i;
  for (i = 0; i < 8; i++)idctrow(block + 8 * i);
  for (i = 0; i < 8; i++)idctcol(block + i);
}
uint8_t DFRobot_Picdecoder_UD::ReadByte(void)
{
  uint8_t i;
  i = *lp++;
  jpg_seek(jpg_buffer, &lp);
  if (i == 0xff)lp++;
  BitPos = 8;
  CurByte = i;
  return i;
}
void DFRobot_Picdecoder_UD::Initialize_Fast_IDCT(void)
{
  int16_t i;

  iclp = iclip_M0 + 512;
  for (i = -512; i < 512; i++)
    iclp[i] = (i < -256) ? -256 : ((i > 255) ? 255 : i);
}
void DFRobot_Picdecoder_UD::idctrow(int32_t* blk)
{
  int32_t x0, x1, x2, x3, x4, x5, x6, x7, x8;
  //intcut
  if (!((x1 = blk[4] << 11) | (x2 = blk[6]) | (x3 = blk[2]) |
        (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))
  {
    blk[0] = blk[1] = blk[2] = blk[3] = blk[4] = blk[5] = blk[6] = blk[7] = blk[0] << 3;
    return;
  }
  x0 = (blk[0] << 11) + 128;
  x8 = W7 * (x4 + x5);
  x4 = x8 + (W1 - W7) * x4;
  x5 = x8 - (W1 + W7) * x5;
  x8 = W3 * (x6 + x7);
  x6 = x8 - (W3 - W5) * x6;
  x7 = x8 - (W3 + W5) * x7;
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6 * (x3 + x2);
  x2 = x1 - (W2 + W6) * x2;
  x3 = x1 + (W2 - W6) * x3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181 * (x4 + x5) + 128) >> 8;
  x4 = (181 * (x4 - x5) + 128) >> 8;
  blk[0] = (x7 + x1) >> 8;
  blk[1] = (x3 + x2) >> 8;
  blk[2] = (x0 + x4) >> 8;
  blk[3] = (x8 + x6) >> 8;
  blk[4] = (x8 - x6) >> 8;
  blk[5] = (x0 - x4) >> 8;
  blk[6] = (x3 - x2) >> 8;
  blk[7] = (x7 - x1) >> 8;
}
void DFRobot_Picdecoder_UD::idctcol(int32_t* blk)
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
  x8 = W7 * (x4 + x5) + 4;
  x4 = (x8 + (W1 - W7) * x4) >> 3;
  x5 = (x8 - (W1 + W7) * x5) >> 3;
  x8 = W3 * (x6 + x7) + 4;
  x6 = (x8 - (W3 - W5) * x6) >> 3;
  x7 = (x8 - (W3 + W5) * x7) >> 3;
  x8 = x0 + x1;
  x0 -= x1;
  x1 = W6 * (x3 + x2) + 4;
  x2 = (x1 - (W2 + W6) * x2) >> 3;
  x3 = (x1 + (W2 - W6) * x3) >> 3;
  x1 = x4 + x6;
  x4 -= x6;
  x6 = x5 + x7;
  x5 -= x7;
  x7 = x8 + x3;
  x8 -= x3;
  x3 = x0 + x2;
  x0 -= x2;
  x2 = (181 * (x4 + x5) + 128) >> 8;
  x4 = (181 * (x4 - x5) + 128) >> 8;
  blk[8 * 0] = iclp[(x7 + x1) >> 14];
  blk[8 * 1] = iclp[(x3 + x2) >> 14];
  blk[8 * 2] = iclp[(x0 + x4) >> 14];
  blk[8 * 3] = iclp[(x8 + x6) >> 14];
  blk[8 * 4] = iclp[(x8 - x6) >> 14];
  blk[8 * 5] = iclp[(x0 - x4) >> 14];
  blk[8 * 6] = iclp[(x3 - x2) >> 14];
  blk[8 * 7] = iclp[(x7 - x1) >> 14];
}

int32_t DFRobot_Picdecoder_UD::jpgDecode(uint8_t* filename, void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
#ifdef ENABLE_UD
  picFile_M0 = UD.open((const char*)filename, FILE_READ);
#else
  picFile_M0 = SD.open((const char*)filename, FILE_READ);
#endif

  if (!picFile_M0)
  {
    DBG("open fail");
    return FUNC_Err;
  }

  int32_t funcret;
  picFile_M0.read(jpg_buffer,JD_SZBUF);
  InitTable();
  if ((funcret = InitTag()) != FUNC_OK)return FUNC_Err;
  if ((SampRate_Y_H == 0) || (SampRate_Y_V == 0))return FUNC_Err;
  AI_Drow_Init();
  funcret = Decode(screenDrawPixel);
  return funcret;
}

/*AVR*/
#else

bool DFRobot_Picdecoder_UD::drawPicture(const char* filename, uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey,void (*screenDrawPixel)(int16_t,int16_t,uint16_t))
{
  screenWidth = ex, screenHeight = ey;

  int32_t funcret;
  uint8_t fileTpe;
  if (ey > sy)PICINFO_M0.S_Height = ey - sy;
  else PICINFO_M0.S_Height = sy - ey;
  if (ex > sx)PICINFO_M0.S_Width = ex - sx;
  else PICINFO_M0.S_Width = sx - ex;
  if (PICINFO_M0.S_Height == 0 || PICINFO_M0.S_Width == 0)
  {
    PICINFO_M0.S_Height = screenHeight;
    PICINFO_M0.S_Width = screenWidth;
    DBG("Display area invalid");
    return false;
  }
  PICINFO_M0.S_YOFF = sy;
  PICINFO_M0.S_XOFF = sx;
  fileTpe = pictype((uint8_t*)filename);
  if (fileTpe == BMPTYPE)
  {
    DBG("draw bmp");
    funcret = bmpDecode((uint8_t*)filename, screenDrawPixel);
  }
  else if (fileTpe == JPGTYPE)
  {
    DBG("Unsupported jpg");
    funcret = FUNC_OK;
  }
  picFile_M0.close();
  if (funcret == FUNC_OK)return true;
  else
  {
    DBG("BMPDecode false");
    return false;
  }
}
#endif