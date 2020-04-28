/*!
 * @file DFRobot_UI.h
 * @brief The basic structure of DFRobot_UI class. 
 * @n Create a series of controls on the screen, such as button, text boxe, progress bar, etc.
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-6
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
  //Partial types, these types will be used before defining
  struct bar;
  struct slider;
  struct button;
  struct Switch;
  struct object;
  struct textBox;
  typedef String scanF() ;
  
  /**
   * @brief Slider callback function, when the progress value of the progress bar changes, it will enter this callback function
   * @param value The value of the progress bar
   * @n Users need to customize events occurred in the callback function
   */
  typedef void sliderCallback(struct slider &slider,struct textBox &obj = NULL);
  
  
  /**
   * @brief Tableview callback function
   * @param highLightPage Page number of the highlighted page
   * @n Users need to customize events occurred in the callback function
   */
  typedef void tableViewCallback(void *highLightPage);
  
  /**
   * @brief Switch callback function, when the state of the switch changes, it will enter this callback function 
   * @param sw Switch object
   * @param obj Text box object
   * @n Users need to customize events occurred in the callback function
   */
  typedef void switchCallback(struct Switch &sw,struct textBox &obj);
  
  /**
    * @brief Button callback function, when the button is pressed, it will enter this callback function
    * @param btn Button object
    * @param obj Text box object
    * @n Users need to customize events occurred in the callback function
    */
  typedef void buttonCallback(struct button &btn,struct textBox &obj );
  
  /**
   * @brief Progress bar callback function
   * @param obj The object of the progress bar
   * @n The event that caused the progress bar to change should be written in it
   */
  typedef void barCallback(struct bar &obj);
  
  /**
   * @brief Numeric keyboard callback function type
   * @param kp Control private data address
   */   
  typedef void keyPadCallback(void *kp);

  /**
   * @brief Control event handler function type
   */
  typedef void(DFRobot_UI::*eventHanding)(void *obj);
  
  /**
   * @brief control drawing function type
   */
  typedef void(DFRobot_UI::*drawingControl)(void *obj);
  
   /*!
     The status of the text box
   */
  struct object{
    uint16_t posx;/**<X-coordinate of the object>**/
    uint16_t posy;/**<Y-coordinate of the object>**/
    uint16_t  width;/**<Object width>**/
    uint16_t  height;/**<Object height>**/
    uint16_t fgColor;/**<The foreground color of the object font>**/
    uint16_t bgColor;/**<The background color of the object font>**/
    uint8_t fontSize;/**<The size of control font elements>**/
    drawingControl draw; /**<Function for drawing control>**/
    struct object * next;/**<Head pointer of the next structure in the linked list>**/
    eventHanding event;/**<Control event handler function>**/
  };
  typedef struct object sObject_t;
  typedef enum {
    DRAWBOX,  /**<Start drawing text box>**/
    DRAWTEXT,/**<Draw string>**/
    ADDCHAR,/**<Add a character to the text box>**/
    NOCHANGE,/**<No change>**/
    CLEARACHAR,/**<Delete the last character in the text box>**/
  } eTextState_t;
  
  /*!
  Touch point data
  */
  typedef struct :object{
    uint8_t id;/**<Touch point id>**/
    uint16_t x;/**<X coordinate of touch point>**/
    uint16_t y;/**<Y-coordinate of touch point>**/
    uint16_t wSize;/**<Touch area width>**/
    uint16_t hSize;/**<Touch area height>**/
  } sPoint_t;
  
  /*!
  Switch control parameter sSwitch_t type
  */
  struct Switch:object{
    bool change;/**<Flag for for indicating Whether the switch has changed>**/
    uint8_t  laststate;/**<The last state of the switch>**/
    uint8_t  state;/**<Switch state: on/off>**/
    switchCallback *callBack;/**<Pointer of callback function>**/
	void setCallback(switchCallback *callBack);/**<Set callback function>**/
	void setOutput(struct textBox * obj);/**<Set the output position of switch>**/
	struct textBox *output;
  } ;
  
  typedef struct Switch sSwitch_t;
  
  /*!
    Struct of sSlider_t type, used to store the parameters of the switch control
  */
  struct slider:object{
    uint16_t sliderHeight;/**<Slider height>**/
    uint16_t sliderWidth;/**<Slider width>**/
    uint16_t range;/**<Range>**/
    uint16_t  lastsliderPos;/**<The last position of the slider (y coordinate)>**/
    uint16_t  sliderPos;/**<Slider position>**/
    int8_t value;/**<Slider value>**/
    bool change;/**<Flag for indicating whether the slider value has changed>**/
    sliderCallback *callBack;/**<Pointer to the slider callback function>**/
	void setCallback(sliderCallback *callBack);/**<Set callback function>**/
	void setOutput(struct textBox * obj);/**<Set the output position of the slider>**/
	struct textBox *output;
  } ;
  typedef struct slider sSlider_t;
  typedef enum {
    BAR ,/**<Strip>**/
    CIRCULAR,/**<Round>**/
	COLUMN,/**<Columnar>**/
  } eBarMode_t;
  /*!
   Struct, used to store the private parameters of the progress bar control
  */
  struct bar:object{
    uint16_t sliderPos;/**<X-coordinate of the progress of the progress bar>**/
    uint16_t color;
    eBarMode_t mode;  /**<You can choose different types of progress bars>**/
    barCallback *callBack;/**<Function pointer of the progress bar callback function>**/
    uint8_t lastValue;/**<Last progress of progress bar >**/
    uint8_t value;/**<Progress of the progress bar (0 ~ 99)>**/
	void setCallback(barCallback *callBack); /**<Set callback function>**/
	void setValue(uint8_t value); /**<Set the value of the progress bar>**/
	void setStyle(eBarMode_t mode);/**<Style the progress bar>**/
  }  ;
  typedef struct bar sBar_t;
  /*!
   Struct type, used to store the private parameters of the button control
  */
  struct button :object{
    char  text[10] ;/**<Button name>**/
    bool click;/**<Flag to indicate whether the button was clicked>**/
    buttonCallback  *callBack ;/**<Function pointer of the button callback function>**/
	void setCallback(buttonCallback *callBack);/**<Set callback function>**/
	void setText(char * c);/**<Set button name>**/
	void setOutput(struct textBox * obj);/**<Set the output text box position of the button>**/
	struct textBox *output;/**<the output position of the button>**/
  }  ;
  typedef struct button sButton_t;
  
  /*!
   Enumerated types, used to define different theme effects
  */
  typedef enum {
    CLASSIC,/**<Classic theme>**/
    MODERN,/**<Popular themes>**/
  } eTheme_t;

  /*!
    Struct type, used to store the private parameters of the text box control
  */
  struct textBox : object{
    eTextState_t state;/**<文本框的状态>**/  /**<The status of the text box>**/
    char cache ;/**<文本框添加一个字符的缓存>**/  /**<Add a character cache to the text box>**/
    char  text[100] ;/**<文本框内容>**/  /**<Text box content>**/
    uint16_t cursorx;/**<文本框光标在x轴的坐标>**/  /**<The coordinate of the text box cursor on the x axis>**/
    uint16_t cursory;/**<文本框光标在y轴的坐标>**/  /**<The coordinate of the text box cursor on the y-axis>**/
    bool selected;/**<文本框是否被选择>**/  /**<Whether the text box is selected>**/
	void setText(char *text);/**<设置文本框的内容>**/  /**<Set the content of the text box>**/
	void setText(String &str);
	void addChar(char txt) ;/**<向文本框添加一个字符>**/  /**<Add a character to the text box>**/
	void deleteChar();/**<删除文本框最后一个一个字符>**/  /**<Delete the last character of the text box>**/
	bool getState();/**<获取文本框状态>**/   /**<Get text box status>**/
  } ;
  typedef struct textBox sTextBox_t;
  
  /*!
    结构体类型，用来存储tableview控件的私有参数  Structure type, used to store private parameters of the tableview control
  */
  struct tableview:object{
    uint16_t offset;/**<table间的间距>**/  /**<spacing between tables>**/
    sButton_t text[4];/**<每个table相当与一个按钮>**/  /**<Each table is equivalent to a button>**/
    uint8_t  numPage;/**<table的个数>**/  /**<number of tables>**/
    uint8_t page;
	uint8_t highLightPage;/**<高亮的table>**/  /**<Highlighted table>**/
    tableViewCallback *callback;/**<tableview的回调函数>**/  /**<Tableview callback function>**/
	void setCallback(tableViewCallback *callBack);/**<设置tableview的回调函数>**/  /**<Set the callback function of tableview>**/
	void changeTableview(uint8_t page);/**<改变tableview的高亮页>**/  /**<Change the highlighted page of tableview>**/
	void setName(uint16_t pageNum,char * c1 ,char * c2 ,char * c3,char * c4);/**<设置tableview的名字>**/  /**<Set tableview name>**/
	uint8_t changeed;
  }  ;
  typedef struct tableview sTableview_t;
  
  typedef enum {
    NOCHOICE,
    CHOICE,
  } eKpMode_t;
  struct keyPad:object{
    eKpMode_t mode;/**<文数字键盘的模式0.带有文本框,1.不带文本框，数字输出的位置由自己指定>**/  /**<Mode of alphanumeric keyboard 0. With text box, 1. Without text box, the position of digital output is specified by yourself>**/
    bool text;
    sButton_t btn[12];/**<数字键盘里面的按钮数据>**/  /**<Button data inside the numeric keypad>**/
    keyPadCallback *callBack;/**<数字键盘的回调函数>**/  /**<Callback function of numeric keyboard>**/
	void setCallback(keyPadCallback *callBack);
    sTextBox_t *textBox;/**<数字盘的文本框数据>**/  /**<Text box data of digital disk>**/
	void setMode(eKpMode_t mode);/**<设置数字键盘的模式>**/  /**<Set the mode of the numeric keypad>**/
    void setOutput(sTextBox_t * text);/**<设置数字键盘的输出文本框>**/  /**<Set the output text box of the numeric keyboard>**/
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
  /**
   * @brief constructor
   * @param gdl
   * @param touch touch object
   */
  DFRobot_UI(DFRobot_GDL *gdl,DFRobot_Touch *touch);
  
  /**
   * @brief 初始化函数,初始化UI使用到的一些参数
   */
  /**
   * @brief initialization function, initialize some parameters used by UI
   */
  void begin();
  /**
   * @brief 设置UI的主题
   * @param the eTheme_t类型数据，包含了了两种主题，主要是颜色和按钮验收的不同
   * @n   the 的参数 ：CLASSIC,
                     ：MODERN,
   */
  /**
   * @brief Set the theme of the UI
   * @param the eTheme_t type data, contains two themes, mainly the difference in color and button acceptance
   * Parameters of @n the: CLASSIC,
                         : MODERN,
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
  /**
   * @brief draw string
   * @param x The x coordinate of the string to be drawn on the screen
   * @param y The x coordinate of the string to be drawn on the screen
   * @param c pointer to character array
   * @param color font color
   * @param bg font background color
   * @param size font size
   * @param mode font display mode
   * @n mode 0: normal display
   * 1: Color inversion
   */
  void drawString(int16_t x, int16_t y, char  *c, uint16_t color, uint16_t bg, uint8_t size, boolean mode);
  
  /**
   * @brief 刷新屏幕
   */
  /**
    * @brief refresh the screen
    */
  void refresh();
  
  /**
   * @brief 使文本框停止输入
   */
  /**
   * @brief makes the text box stop typing
   */
  void endInput();

  /**
   * @brief 设置触摸的手势识别区域
   * @param x 识别区域的开始位置的x坐标
   * @param y 识别区域的开始位置的y坐标
   * @param width 识别区域的宽度
   * @param height 识别区域的高度
   */
  /**
   * @brief Set the touch gesture recognition area
   * @param x The x coordinate of the start position of the recognition area
   * @param y the y coordinate of the start position of the recognition area
   * @param width the width of the recognition area
   * @param height The height of the recognition area
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
  /**
   * @brief get gesture
   * @return sGestures_t type data, representing different gestures
   * @n UPGLIDE: slide up
   * @n DOWNGLIDE: slide down
   * @n LEFTGLIDE: swipe left
   * @n RIGHTGLIDE: swipe right
   * @n LONGPRESSDE: Long press the screen
   * @n SINGLECLICK: Click on the screen
   * @n DOUBLECLICK: double-tap the screen
   * @n NONE: no gesture
   */
  eGesture_t getGestures();
  
  /**
   * @brief 在屏幕上创建一个数字键盘
   * @return 数字键盘对象的地址
   */
  /**
   * @brief creates a numeric keyboard on the screen
   * @return address of numeric keyboard object
   */
  sKeyPad_t &creatKeyPad();
  
  /**
   * @brief 绘制控件
   * @param posx 控件的x坐标
   * @param posy 控件的y坐标
   * @param width 控件的宽度
   * @param height 控件的高度
   */
  /**
   * @brief drawing control
   * @param posx control x coordinate
   * @param posy control's y coordinate
   * @param width The width of the control
   * @param height The height of the control
   */
  void draw(sObject_t *obj,uint16_t posx = 0,uint16_t posy = 0,uint16_t width = 0,uint16_t height = 0);
  
  /**
   * @brief 在屏幕上创建一个按钮控件
   * @return 按钮对象的地址
   */
  /**
   * @brief creates a button control on the screen
   * @return button object address
   */
  sButton_t &creatButton();
  /**
   * @brief 在屏幕上创建一个文本框控件
   * @return 文本框对象的地址
   */
  /**
   * @brief Create a text box control on the screen
   * @return the address of the text box object
   */
  sTextBox_t &creatText();
  /**
   * @brief 在屏幕上创建一个滑条控件
   * @return 滑条对象的地址
   */
  /**
   * @brief creates a slider control on the screen
   * @return Address of slider object
   */
  sSlider_t &creatSlider();
  /**
   * @brief 在屏幕上创建一个开关控件
   * @return 开关对象的地址
   */
  /**
   * @brief creates a switch control on the screen
   * @return The address of the switch object
   */
  sSwitch_t &creatSwitch();
  
  
  /**
   * @brief 在屏幕上创建一个tableview控件
   * @return tableview对象的地址
   */
  /**
   * @brief creates a tableview control on the screen
   * @return tableview object address
   */
  sTableview_t &creatTableview();
  
  /**
   * @brief 在屏幕上创建一个进度条
   * @n 用户可以自定义结构体里面的数据或者使用经初始化的参数
   */
  /**
   * @brief creates a progress bar on the screen
   * @n Users can customize the data in the structure or use the initialized parameters
   */
  sBar_t &creatBar();
  

  
  uint16_t bgColor; //屏幕背景颜色  Screen background color

private:
  /**
   * @brief 刷新进度条
   * @param bar sBar_t，里面包含了进度条的位置，长度和宽度等参数
   */
  /**
   * @brief refresh the progress bar
   * @param bar sBar_t, which contains parameters such as the position, length and width of the progress bar
   */
  void barEvent(void *obj);
  /**
   * @brief 刷新tableview
   * @param sTableview_t sSlider_t，里面包含了滑条的位置，长度和宽度等参数
   */ 
  /**
   * @brief refresh tableview
   * @param sTableview_t sSlider_t, which contains the position, length and width of the slider
   */
  void tableviewEvent(void *obj);
  /**
   * @brief 在屏幕上创建一个tableview控件
   * @param tv sTableview_t，里面包含了tableview的页数，内容等参数
   * @n 用户需要自定义这些数据
   */
  /**
   * @brief creates a tableview control on the screen
   * @param tv sTableview_t, which contains the tableview page number, content and other parameters
   * @n Users need to customize these data
   */
  void drawBar(void *obj);
  void sliderEvent(void *obj);
  void buttonEvent(void *btn1);
  /**
   * @brief 创建一个数字键盘控件
   * @param te sKeyPad_t 类型的结构体
   */
  /**
   * @brief Create a numeric keyboard control
   * @param te sKeyPad_t structure
   */
  void drawKeyBoard(void *obj);
  
  /**
   * @brief 刷新数字键盘
   * @param te sKeyPad_t，里面包含了数字键盘的位置，长度和宽度等参数
   */
  /**
   * @brief refresh the numeric keyboard
   * @param te sKeyPad_t, which contains parameters such as the position, length and width of the numeric keypad
   */
  void KeyBoardEvent(void *obj);
  /**
   * @brief 绘制文本框
   * @param obj 文本框对象的地址
   */
  /**
   * @brief draw text box
   * @param obj The address of the text box object
   */
  void drawText(void *obj);
  /**
   * @brief 更新触摸点的数据
   */
  /**
   * @brief update touch point data
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
  /**
   * @brief refresh the text box
   * @param te contains parameters such as the position, length and width of the switch
   * @n can operate on the text box:
     @n 1. Make the text box display a string
     @n 2. Add a character in the text box
     @n 2. Delete a character in the text box
   */
  void refreshTextBox(void *obj);
  /**
   * @brief 刷新滑条
   * @param slider sSlider_t，里面包含了滑条的位置，长度和宽度等参数
   */
  /**
   * @brief refresh slider
   * @param slider sSlider_t, which contains parameters such as the position, length and width of the slider
   */
  void refreshSliser(void *obj);
  
  /**
   * @brief 在屏幕上创建一个滑条
   * @param slider sSlider_t类型的数据
   * @n 用户可以自定义结构体里面的数据或者使用经初始化的参数
   */
  /**
   * @brief creates a slider on the screen
   * @param slider sSlider_t type data
   * @n Users can customize the data in the structure or use the initialized parameters
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
