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
#include "graphic.h"
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
    uint16_t lvColor;
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
    eTextState_t state;/**<The status of the text box>**/
    char cache ;/**<Add a char cache to the text box>**/
    char  text[200] ;/**<Text box content>**/
    uint16_t cursorx;/**<The X-coordinate of the text box cursor>**/
    uint16_t cursory;/**<The Y-coordinate of the text box cursor>**/
    bool selected;/**<Whether the text box is selected>**/
	void setText(char *text);/**<Set the content of the text box>**/
	void setText(String &str);
	void addChar(char txt) ;/**<Add a character to the text box>**/
	void deleteChar();/**<Delete the last character of the text box>**/
	bool getState();/**<Get text box status>**/
  } ;
  typedef struct textBox sTextBox_t;
  
  /*!
    Struct type, used to store private parameters of the tableview control
  */
  struct tableview:object{
    uint16_t offset;/**<Space between tables>**/
    sButton_t text[4];/**<Each table is equivalent to a button>**/
    uint8_t  numPage;/**<Number of tables>**/
    uint8_t page;
	uint8_t highLightPage;/**<Highlighted table>**/
    tableViewCallback *callback;/**<Tableview callback function>**/
	void setCallback(tableViewCallback *callBack);/**<Set the callback function of tableview>**/
	void changeTableview(uint8_t page);/**<Change the highlighted page of tableview>**/
	void setName(uint16_t pageNum,char * c1 ,char * c2 ,char * c3,char * c4);/**<Set tableview name>**/
	uint8_t changeed;
  }  ;
  typedef struct tableview sTableview_t;
  
  typedef enum {
    NOCHOICE,
    CHOICE,
  } eKpMode_t;
  struct keyPad:object{
    eKpMode_t mode;/**<Mode of alphanumeric keyboard: 0, with text box; 1, without text box. The position of number output is specified by yourself>**/
    bool text;
    sButton_t btn[12];/**<Button data inside the numeric keypad>**/
    keyPadCallback *callBack;/**<Callback function of numeric keyboard>**/
	void setCallback(keyPadCallback *callBack);
    sTextBox_t *textBox;/**<Text box data of numeric keyboard>**/
	void setMode(eKpMode_t mode);/**<Set the mode of the numeric keyboard>**/
    void setOutput(sTextBox_t * text);/**<Set the output text box of the numeric keyboard>**/
  } ;
  typedef struct keyPad sKeyPad_t;
  typedef enum {
    CENTER,
    RIGHT,
    LEFT,
  } eLocation_t;


  typedef enum{
   TLINE,
   TPOINT,
   TCOLUMN,
   }eCoordinateType_t;
  struct sLine{
   sPoint_t *point;
   uint8_t number;
   uint8_t uid;
    uint16_t pointColor;
   struct sLine *line;
  };
 typedef struct sLine sLine_t;

  struct coordinate:object{
    sLine_t line;
    uint8_t lineNumber;
    uint8_t intervalX;
    uint8_t intervalY;
    uint8_t intervalWidth;
    eCoordinateType_t type;
    uint8_t pointRadius;
    uint16_t pointColor;
    void setPoint(int16_t poi[][2],uint8_t number,uint16_t color);
  };
  typedef struct coordinate sCoordinate_t;
  

protected:
  scanF * scan;
public:

  /**
   * @brief constructor
   * @param gdl
   * @param touch Touch object
   */
  DFRobot_UI(DFRobot_GDL *gdl,DFRobot_Touch *touch);
  
  /**
   * @brief init function, parameters need to be used by UI initialization 
   */
  void begin();
  /**
   * @brief Set UI theme 
   * @param the eTheme_t type data, contains two themes, mainly the different in color and button pattern 
   * @n the Parameters: CLASSIC,
                      : MODERN,
   */
  void setTheme(eTheme_t the);

  /**
   * @brief Draw string
   * @param x The x-coordinate of the string to be drawn on the screen
   * @param y The x-coordinate of the string to be drawn on the screen
   * @param c Pointer to character array
   * @param color Font color
   * @param bg Font background color
   * @param size Font size
   * @param mode Font display mode
   * @n mode 0: normal display
   *         1: color inversion
   */
  void drawString(int16_t x, int16_t y, char  *c, uint16_t color, uint16_t bg, uint8_t size, boolean mode);
  
  /**
    * @brief refresh the screen
    */
  void refresh();
  
  /**
   * @brief Stop text box input 
   */
  void endInput();

  /**
   * @brief Set the touch gesture recognition area
   * @param x The x-coordinate of the start position of the recognition area
   * @param y The y-coordinate of the start position of the recognition area
   * @param width The width of the recognition area
   * @param height The height of the recognition area
   */
  void setGestureArea(uint16_t x,uint16_t y,uint16_t width,uint16_t height);
  
  /**
   * @brief Get gesture
   * @return sGestures_t type data, represent different gestures
   * @n UPGLIDE: slide up
   * @n DOWNGLIDE: slide down
   * @n LEFTGLIDE: swipe left
   * @n RIGHTGLIDE: swipe right
   * @n LONGPRESSDE: long press the screen
   * @n SINGLECLICK: click on the screen
   * @n DOUBLECLICK: double-tap the screen
   * @n NONE: no gesture
   */
  eGesture_t getGestures();
  
  /**
   * @brief Create a numeric keyboard on the screen
   * @return Address of numeric keyboard object
   */
  sKeyPad_t &creatKeyPad();
  
  /**
   * @brief Draw control
   * @param posx X-coordinate of control
   * @param posy Y-coordinate of cpntrol
   * @param width The width of the control
   * @param height The height of the control
   */
  void draw(sObject_t *obj,uint16_t posx = 0,uint16_t posy = 0,uint16_t width = 0,uint16_t height = 0);
  
  /**
   * @brief Create a button control on the screen
   * @return Button object address
   */
  sButton_t &creatButton();
  /**
   * @brief Create a text box control on the screen
   * @return Address of the text box object
   */
  sTextBox_t &creatText();
  /**
   * @brief Create a slider control on the screen
   * @return Address of slider object
   */
  sSlider_t &creatSlider();
  /**
   * @brief Create a switch control on the screen
   * @return Address of the switch object
   */
  sSwitch_t &creatSwitch();
  

  /**
?? * @brief creates a coordinate system control on the screen
?? * @return the address of the coordinate system object
?? */
  sCoordinate_t &creatCoordinate();
  /**
   * @brief Create a tableview control on the screen
   * @return Address of the tableview object 
   */
  sTableview_t &creatTableview();
  
  /**
   * @brief Create a progress bar on the screen
   * @n Users can customize the data in the struct or use the initialized parameters
   */
  sBar_t &creatBar();
  
  void setBgColor(uint16_t Color);
  

  
  uint16_t bgColor; //Screen background color
  void drawCoordinate(void *obj);
  void drawCoordLine(void * obj,sLine_t *line);
private:
  /**
   * @brief Refresh the progress bar
   * @param bar sBar_t, which contains parameters such as the position, length and width of the progress bar
   */
  void barEvent(void *obj); 
  /**
   * @brief Refresh tableview
   * @param sTableview_t sSlider_t, which contains the position, length and width of the slider
   */
  void tableviewEvent(void *obj);
  /**
   * @brief Create a tableview control on the screen
   * @param tv sTableview_t, which contains the tableview page number, content and other parameters
   * @n Users need to customize these data
   */
  void drawBar(void *obj);
  void sliderEvent(void *obj);
  void buttonEvent(void *btn1);
  /**
   * @brief Create a numeric keyboard control
   * @param te sKeyPad_t type struct
   */
  void drawKeyBoard(void *obj);
  
  /**
   * @brief Refresh the numeric keyboard
   * @param te sKeyPad_t, which contains parameters such as the position, length and width of the numeric keyboard
   */
  void KeyBoardEvent(void *obj);
  /**
   * @brief Draw text box
   * @param obj Address of the text box object
   */
  void drawText(void *obj);
  /**
   * @brief Update touch point data
   */
  void updateCoordinate();
  
  /**
   * @brief Refresh the text box
   * @param te Contains parameters such as the position, length and width of the switch
   * @n What operation you can do with the text box:
     @n                                            1. Display a string in the text box 
     @n                                            2. Add a character in the text box
     @n                                            3. Delete a character in the text box
   */
  void refreshTextBox(void *obj);
  /**
   * @brief Refresh slider
   * @param slider sSlider_t, which contains parameters such as the position, length and width of the slider
   */
  void refreshSliser(void *obj);
  
  /**
   * @brief Create a slider on the screen
   * @param slider sSlider_t type data
   * @n Users can customize the data in the struct or use the initialized parameters
   */
  void drawSlider(void *obj);
 
  void coordinateEvent(void *obj);
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
  void drawLine(void *obj ,int16_t x0, int16_t y0, int16_t x1, int16_t y1,
        uint16_t color);
  #if defined(ESP32) || defined(ESP8266) ||  defined(ARDUINO_SAM_ZERO)||(__AVR_ATmega2560__)
  void fillRoundRectP(int16_t x, int16_t y, int16_t w,int16_t h, int16_t r, uint16_t hcolor,uint16_t color);
  void fillCircleHelperP(int16_t x0, int16_t y0, int16_t r,uint8_t corners, int16_t delta,uint16_t width1 ,uint16_t hcolor,uint16_t color);
  void drawRectShadowF(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t r,uint16_t mColor ,uint16_t gColor);
  void drawRectShadowB(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint16_t r,uint16_t mColor ,uint16_t gColor);
  void drawRectP(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t radius,uint16_t mcolor,
  uint16_t gcolor,uint16_t edgeColor,uint8_t shadow,uint16_t bg_color = 0xffff);
  void fillCircleP(int16_t x0, int16_t y0, int16_t r,uint16_t mcolor,uint16_t color);

  #endif

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
