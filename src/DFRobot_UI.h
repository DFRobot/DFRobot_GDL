/*!
 * @file DFRobot_UI.h
 * @brief DFRobot_UI 类的基础结构,可以在屏幕上创建一系列的控件，如按钮,文本框,进度条等
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
 */
#ifndef __DFROBOT_UI_H
#define __DFROBOT_UI_H

#include "Arduino.h"
#include "DFRobot_GDL.h"
#include "DFRobot_Touch.h"
#if defined (__AVR__)
#define TOUCHPOINTS   1
#else
#define TOUCHPOINTS   5
#endif
#include "DFRobot_Gesture.h"
#define BLACK_RGB565        0x0000
#define BLUE_RGB565         0x001F
#define RED_RGB565          0xF800
#define GREEN_RGB565        0x07E0
#define CYAN_RGB565         0x07FF
#define MAGENTA_RGB565      0xF81F
#define YELLOW_RGB565       0xFFE0
#define WHITE_RGB565        0xFFFF
#define NAVY_RGB565         0x000F
#define DARKGREEN_RGB565    0x03E0
#define DARKCYAN_RGB565     0x03EF
#define MAROON_RGB565       0x7800
#define PURPLE_RGB565       0x780F
#define OLIVE_RGB565        0x7BE0
#define LIGHTGREY_RGB565    0xC618
#define DARKGREY_RGB565     0x7BEF
#define ORANGE_RGB565       0xFD20
#define GREENYELLOW_RGB565  0xAFE5
#define DCYAN_RGB565        0x03EF

class DFRobot_UI : public DFRobot_Gesture
{
public:
  //不完全类型，在定义之前会使用到这些类型，
  struct bar;
  struct slider;
  struct button;
  struct Switch;
  struct object;
  struct textBox;
  typedef String scanF() ;
  
  /**
   * @brief 滑条的回调函数，当进度条的进度值发生改变时，会进入此回调函数
   * @param value 进度条的值
   * @n 回调函数里面发生的事件，用户需自定义
   */
  typedef void sliderCallback(struct slider &slider,struct textBox &obj = NULL);
  
  
  /**
   * @brief tableview的回调函数
   * @param highLightPage 高亮的页的页数
   * @n 回调函数里面发生的事件，用户需自定义
   */
  typedef void tableViewCallback(void *highLightPage);
  
  /**
   * @brief 开关的回调函数，当开关的状态发生改变时，会进入此回调函数
   * @param sw 开关对象
   * @param obj 文本框对象
   * @n 回调函数里面发生的事件，用户需自定义
   */
  typedef void switchCallback(struct Switch &sw,struct textBox &obj);
  
  /**
   * @brief 按钮的回调函数，当按钮被按下，会进入此回调函数
   * @param btn 按钮的对象
   * @param obj 文本框对象
   * @n 回调函数里面发生的事件，用户需自定义
   */
  typedef void buttonCallback(struct button &btn,struct textBox &obj );
  
  /**
   * @brief 进度条的回调函数
   * @param obj 进度条的对象
   * @n 引发进度条发生改变的事件应该写在里面
   */

  typedef void barCallback(struct bar &obj);
  
  /**
   * @brief 数字键盘回调函数类型
   * @param kp 控件私有数据的地址
   */  
  typedef void keyPadCallback(void *kp);

  /**
   * @brief 控件事件处理函数函数类型
   */
  typedef void(DFRobot_UI::*eventHanding)(void *obj);
  
  /**
   * @brief 控件绘制函数类型
   */
  typedef void(DFRobot_UI::*drawingControl)(void *obj);
  
  /*!
    文本框的状态
  */
  struct object{
    uint16_t posx;/**<对象在x轴的坐标>**/
    uint16_t posy;/**<对象在y轴的坐标>**/
    uint16_t  width;/**<对象的宽度>**/
    uint16_t  height;/**<对象的高度>**/
    uint16_t fgColor;/**<对象字体的前景的颜色>**/
    uint16_t bgColor;/**<对象字体的背景颜色>**/
    uint8_t fontSize;/**<控件字体元素的大小>**/
    drawingControl draw; /**<绘制控件的函数>**/
    struct object * next;/**<链表下一个结构体的头指针>**/
    eventHanding event;/**<控件的事件处理函数>**/
  };
  typedef struct object sObject_t;
  typedef enum {
    DRAWBOX,  /**<开始绘制文本框>**/
    DRAWTEXT,/**<绘制字符串.>**/
    ADDCHAR,/**<向文本框添加一个字符.>**/
    NOCHANGE,/**<无改变.>**/
    CLEARACHAR,/**<删除文本框内容的最后一个字符.>**/
  } eTextState_t;
  
  /*!
    触摸点的数据
  */
  typedef struct :object{
    uint8_t id;/**<触摸点的id>**/
    uint16_t x;/**<触摸点的x坐标>**/
    uint16_t y;/**<触摸点的y坐标>**/
    uint16_t wSize/**<触摸区域的宽度>**/;
    uint16_t hSize;/**<触摸区域的高度>**/
  } sPoint_t;
  
  /*!
    开关控件的参数 sSwitch_t类型
  */
  struct Switch:object{
    bool change;/**<开关是否改变的标志>**/
    uint8_t  laststate;/**<开关的上一次的状态>**/
    uint8_t  state;/**<开关的状态(on / off)>**/
    switchCallback *callBack;/**<回调函数的指针>**/
	void setCallback(switchCallback *callBack);/**<设置回调函数>**/
	void setOutput(struct textBox * obj);/**<设置开关输出的位置>**/
	struct textBox *output;
  } ;
  
  typedef struct Switch sSwitch_t;
  
  /*!
    sSlider_t类型的结构体 ,用来存储开关控件的参数
  */
  struct slider:object{
    uint16_t sliderHeight;/**<滑块的高度>**/
    uint16_t sliderWidth;/**<滑块的宽度>**/
    uint16_t range;/**<范围>**/
    uint16_t  lastsliderPos;/**<滑块上一次的位置(y坐标)>**/
    uint16_t  sliderPos;/**<滑块的位置>**/
    int8_t value;/**<滑块的值>**/
    bool change;/**<滑块值是否发生改变的标志>**/
    sliderCallback *callBack;/**<滑块回调函数的指针>**/
	void setCallback(sliderCallback *callBack);/**<设置回调函数>**/
	void setOutput(struct textBox * obj);/**<设置滑条输出的位置>**/
	struct textBox *output;
  } ;
  typedef struct slider sSlider_t;
  typedef enum {
    BAR ,/**<条形>**/
    CIRCULAR,/**<圆形>**/
	COLUMN,/**<柱形>**/
  } eBarMode_t;
  /*!
    结构体，用来存储进度条控件的私有参数
  */
  struct bar:object{
    uint16_t sliderPos;/**<进度条的进度的x坐标>**/
    uint16_t color;
    eBarMode_t mode;  /**<可以选择不同类型的进度条>**/
    barCallback *callBack;/**<进度条回调函数的函数指针>**/
    uint8_t lastValue;/**<进度条上一次的进度>**/
    uint8_t value;/**<进度条的进度(0~99)>**/
	void setCallback(barCallback *callBack);/**<设置回调函数>**/
	void setValue(uint8_t value);/**<设置进度条的值>**/
	void setStyle(eBarMode_t mode);/**<设置进度条的样式>**/
  }  ;
  typedef struct bar sBar_t;
  /*!
    结构体类型，用来存储按钮控件的私有参数
  */
  struct button :object{
    char  text[10] ;/**<按钮的名字>**/
    bool click;/**<按钮是否被点击的标志>**/
    buttonCallback  *callBack ;/**<按钮的回调函数的函数指针>**/
	void setCallback(buttonCallback *callBack);/**<设置回调函数>**/
	void setText(char * c);/**<设置按钮的名字>**/
	void setOutput(struct textBox * obj);/**<设置按钮的输出文本框位置>**/
	struct textBox *output;/**<设置按钮的输出文本框位置>**/
  }  ;
  typedef struct button sButton_t;
  
  /*!
    枚举类型，用来定义不同的主题效果
  */
  typedef enum {
    CLASSIC,/**<经典主题>**/
    MODERN,/**<流行主题>**/
  } eTheme_t;

  /*!
    结构体类型，用来存储文本框控件的私有参数
  */
  struct textBox : object{
    eTextState_t state;/**<文本框的状态>**/
    char cache ;/**<文本框添加一个字符的缓存>**/
    char  text[100] ;/**<文本框内容>**/
    uint16_t cursorx;/**<文本框光标在x轴的坐标>**/
    uint16_t cursory;/**<文本框光标在y轴的坐标>**/
    bool selected;/**<文本框是否被选择>**/
	void setText(char *text);/**<设置文本框的内容>**/
	void setText(String &str);
	void addChar(char txt) ;/**<向文本框添加一个字符>**/
	void deleteChar();/**<删除文本框最后一个一个字符>**/
	bool getState();/**<获取文本框状态**/
  } ;
  typedef struct textBox sTextBox_t;
  
  /*!
    结构体类型，用来存储tableview控件的私有参数
  */
  struct tableview:object{
    uint16_t offset;/**<table间的间距>**/
    sButton_t text[4];/**<每个table相当与一个按钮>**/
    uint8_t  numPage;/**<table的个数>**/
    uint8_t page;
	uint8_t highLightPage;/**<高亮的table>**/
    tableViewCallback *callback;/**<tableview的回调函数>**/
	void setCallback(tableViewCallback *callBack);/**<设置tableview的回调函数>**/
	void changeTableview(uint8_t page);/**<改变tableview的高亮页>**/
	void setName(uint16_t pageNum,char * c1 ,char * c2 ,char * c3,char * c4);/**<设置tableview的名字>**/
	uint8_t changeed;
  }  ;
  typedef struct tableview sTableview_t;
  
  typedef enum {
    NOCHOICE,
    CHOICE,
  } eKpMode_t;
  struct keyPad:object{
    eKpMode_t mode;/**<文数字键盘的模式0.带有文本框,1.不带文本框，数字输出的位置由自己指定>**/
    bool text;
    sButton_t btn[12];/**<数字键盘里面的按钮数据>**/
    keyPadCallback *callBack;/**<数字键盘的回调函数>**/
	void setCallback(keyPadCallback *callBack);
    sTextBox_t *textBox;/**<数字盘的文本框数据>**/
	void setMode(eKpMode_t mode);/**<设置数字键盘的模式>**/
    void setOutput(sTextBox_t * text);/**<设置数字键盘的输出文本框>**/
  } ;
  typedef struct keyPad sKeyPad_t;
  typedef enum {
    CENTER,
    RIGHT,
    LEFT,
  } eLocation_t;
  

protected:
  scanF * scan;
public:

  /**
   * @brief 构造函数
   * @param gdl 
   * @param touch 触摸对象
   */
  DFRobot_UI(DFRobot_GDL *gdl,DFRobot_Touch *touch);
  
  /**
   * @brief 初始化函数,初始化UI使用到的一些参数
   */
  void begin();
  /**
   * @brief 设置UI的主题
   * @param the eTheme_t类型数据，包含了了两种主题，主要是颜色和按钮验收的不同
   * @n   the 的参数 ：THEME1,
                     ：THEME2,
   */
  void setTheme(eTheme_t the);

  /**
   * @brief 绘制字符串
   * @param x 所需绘制字符串在屏幕上的x坐标
   * @param y 所需绘制字符串在屏幕上的x坐标
   * @param c 字符数组的指针
   * @param color 字体的颜色
   * @param bg 字体背景的颜色
   * @param size 字体的大小
   * @param mode 字体显示模式
   * @n mode  0 ： 正常显示
   *          1 ： 颜色反转
   */
  void drawString(int16_t x, int16_t y, char  *c, uint16_t color, uint16_t bg, uint8_t size, boolean mode);
  
  /**
   * @brief 刷新屏幕
   */
  void refresh();
  
  /**
   * @brief 使文本框停止输入
   */
  void endInput();

  /**
   * @brief 设置触摸的手势识别区域
   * @param x 识别区域的开始位置的x坐标
   * @param y 识别区域的开始位置的y坐标
   * @param width 识别区域的宽度
   * @param height 识别区域的高度
   */
  void setGestureArea(uint16_t x,uint16_t y,uint16_t width,uint16_t height);
  
  /**
   * @brief 获取手势
   * @return sGestures_t类型的数据，代表着不同的手势
   * @n      UPGLIDE ：向上滑动
   * @n      DOWNGLIDE ：向下滑动
   * @n      LEFTGLIDE ：向左滑动
   * @n      RIGHTGLIDE ：向右滑动
   * @n      LONGPRESSDE ：长按屏幕
   * @n      SINGLECLICK ：单击屏幕
   * @n      DOUBLECLICK ：双击屏幕
   * @n      NONE ：没有手势
    */
  eGesture_t getGestures();
  
  /**
   * @brief 在屏幕上创建一个数字键盘
   * @return 数字键盘对象的地址
   */
  sKeyPad_t &creatKeyPad();
  
  /**
   * @brief 绘制控件
   * @param posx 控件的x坐标
   * @param posy 控件的y坐标
   * @param width 控件的宽度
   * @param height 控件的高度
   */
  void draw(sObject_t *obj,uint16_t posx = 0,uint16_t posy = 0,uint16_t width = 0,uint16_t height = 0);
  
  /**
   * @brief 在屏幕上创建一个按钮控件
   * @return 按钮对象的地址
   */
  sButton_t &creatButton();
  /**
   * @brief 在屏幕上创建一个文本框控件
   * @return 文本框对象的地址
   */
  sTextBox_t &creatText();
  /**
   * @brief 在屏幕上创建一个滑条控件
   * @return 滑条对象的地址
   */
  sSlider_t &creatSlider();
  /**
   * @brief 在屏幕上创建一个开关控件
   * @return 开关对象的地址
   */
  sSwitch_t &creatSwitch();
  
  
  /**
   * @brief 在屏幕上创建一个tableview控件
   * @return tableview对象的地址
   */
  sTableview_t &creatTableview();
  
  /**
   * @brief 在屏幕上创建一个进度条
   * @n 用户可以自定义结构体里面的数据或者使用经初始化的参数
   */
  sBar_t &creatBar();
  

  
  uint16_t bgColor; //屏幕背景颜色

private:
  /**
   * @brief 刷新进度条
   * @param bar sBar_t，里面包含了进度条的位置，长度和宽度等参数
   */
  void barEvent(void *obj);
  /**
   * @brief 刷新tableview
   * @param sTableview_t sSlider_t，里面包含了滑条的位置，长度和宽度等参数
   */
  void tableviewEvent(void *obj);
  /**
   * @brief 在屏幕上创建一个tableview控件
   * @param tv sTableview_t，里面包含了tableview的页数，内容等参数
   * @n 用户需要自定义这些数据
   */
  void drawBar(void *obj);
  void sliderEvent(void *obj);
  void buttonEvent(void *btn1);
  /**
   * @brief 创建一个数字键盘控件
   * @param te sKeyPad_t 类型的结构体
   */
  void drawKeyBoard(void *obj);
  
  /**
   * @brief 刷新数字键盘
   * @param te sKeyPad_t，里面包含了数字键盘的位置，长度和宽度等参数
   */
  void KeyBoardEvent(void *obj);
  /**
   * @brief 绘制文本框
   * @param obj 文本框对象的地址
   */
  void drawText(void *obj);
  /**
   * @brief 更新触摸点的数据
   */
  void updateCoordinate();
  
  /**
   * @brief 刷新文本框
   * @param te 里面包含了开关的位置，长度和宽度等参数
   * @n 可以对文本框的操作有：
     @n                    1.让文本框显示字符串
     @n                    2.在文本框添加一个字符
     @n                    2.在文本框删除一个字符
   */
  void refreshTextBox(void *obj);
  /**
   * @brief 刷新滑条
   * @param slider sSlider_t，里面包含了滑条的位置，长度和宽度等参数
   */
  void refreshSliser(void *obj);
  
  /**
   * @brief 在屏幕上创建一个滑条
   * @param slider sSlider_t类型的数据
   * @n 用户可以自定义结构体里面的数据或者使用经初始化的参数
   */
  void drawSlider(void *obj);
  void drawSwitch(void *obj);
  void switchEvent(void *obj);
  void drawTableview(void *obj);
  uint8_t pointNum(String str);
  uint8_t stringToPoint(String str, sPoint_t *point);
  uint8_t stringToPoint(String str);
  void drawClickButton(sObject_t *obj,char *text,bool click);
  void drawButtonString(sObject_t *obj, eLocation_t x, eLocation_t y, char * c,bool click);
  void drawkpString(sButton_t *kp, eLocation_t x, eLocation_t y, char * c);
  void drawButton(void *obj);
  void drawCursor(sTextBox_t* obj, uint8_t offset_x, uint8_t offset_y, bool state);
  bool judgePress(sObject_t *obj, uint16_t x, uint16_t y);
  bool judgeKpPress(sButton_t*obj, uint16_t x, uint16_t y);
  bool release(uint16_t x,uint16_t y);
  bool press(uint16_t x,uint16_t y);
  bool focus(uint16_t x,uint16_t y);
  
  sPoint_t * position;
  DFRobot_GDL *_gdl;
  eTheme_t theme;
  uint16_t  cursorPosx ,cursorPosy;
  uint16_t lcdWidth;
  uint16_t lcdHeight;
  DFRobot_Touch *_touch;
  uint8_t number;
  long long timer;
  long long timer1;
  
  sTextBox_t textbox;
  sObject_t head;
  bool cursorState;
  
  uint8_t click;
  uint8_t pressed;
  uint8_t screenPressed; 
  uint16_t gesturex,gesturey,gestureWidth,gestureHeight;
};
#endif
