
#define COLOR_SET_R(c, v) (c).ch.red = (uint8_t)(((uint8_t)(v)) & 0x1F)
#define COLOR_SET_G(c, v) (c).ch.green = (uint8_t)((v) & 0x3F)
#define COLOR_SET_B(c, v) (c).ch.blue = (uint8_t)((v) & 0x1F)
#define COLOR_SET_A(c, v)  do {} while(0)
#ifndef _GRAPHIC_h
#define _GRAPHIC_h
#define COLOR_GET_R(c) (c).ch.red
#define COLOR_GET_G(c) (c).ch.green
#define COLOR_GET_B(c) (c).ch.blue
#define COLOR_GET_A(c)     0xFF
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
#define CIRC_OCT1_X(p) (p.x)
#define CIRC_OCT1_Y(p) (p.y)
#define CIRC_OCT2_X(p) (p.y)
#define CIRC_OCT2_Y(p) (p.x)

typedef struct
{
    int16_t x;
    int16_t y;
} sPoint_t;

typedef struct
{
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
} sUiarea_t;

typedef union
{
    struct
    {
        uint16_t blue : 5;
        uint16_t green : 6;
        uint16_t red : 5;
    } ch;
    uint16_t full;
} sUicolor_t;

static inline sUicolor_t colorMix(sUicolor_t c1, sUicolor_t c2, uint8_t mix)
{
    sUicolor_t ret;
    /*LV_COLOR_DEPTH == 8, 16 or 32*/
    COLOR_SET_R(ret, (uint16_t)((uint16_t) COLOR_GET_R(c1) * mix + COLOR_GET_R(c2) * (255 - mix)) >> 8);
    COLOR_SET_G(ret, (uint16_t)((uint16_t) COLOR_GET_G(c1) * mix + COLOR_GET_G(c2) * (255 - mix)) >> 8);
    COLOR_SET_B(ret, (uint16_t)((uint16_t) COLOR_GET_B(c1) * mix + COLOR_GET_B(c2) * (255 - mix)) >> 8);
    COLOR_SET_A(ret, 0xFF);
    return ret;
    
}

// void drawRect_mid(uint16_t x1,uint16_t x2,uint16_t y1,uint16_t y2){

       
        // uint16_t posy = 0;  
        // for(uint16_t row = y1+radius; row <= y2-radius; row++) {
            // posy  =  row ;
            // uint16_t mix          = (uint32_t)((uint32_t)(y2 - posy) * 255) / (100);
             // act_color    = lv_color_mix(mcolor, gcolor, mix);
 
            // tft.fillRect(x1,posy,x2-x1,1,act_color.full);
            
        // } 
 // }


#endif
