/*!
 * @file DFRobot_Gesture.h
 * @brief DFRobot_Gesture 类的基础结构,通过采集触摸屏幕上手指划滑过的点来识别手势
 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @author [fengli](li.feng@dfrobot.com)
 * @version  V1.0
 * @date  2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src/DFRpbot_UI
 */
#ifndef __DFROBOT_GESTURE_H
#define __DFROBOT_GESTURE_H

#include "Arduino.h"

#define POINT 3

class DFRobot_Gesture{
public:
//用来存储一个点在触摸屏按下到松开的信息
typedef struct{
    uint16_t pointx[POINT]; //所有记录点的x坐标
	uint16_t pointy[POINT]; //所有记录点的y坐标
	uint8_t id; //触摸点的id
	uint8_t pressed; //是否被按下
}sTouchRecord_t;
//记录即时五个点的位置信息
typedef struct{
	uint16_t x[5];
	uint16_t y[5];
	
}sTouchMessage_t;
  /*!
    该枚举定义了不同的手势
  */
  typedef enum {
  	SCLICK,/**<单指点击>**/
  	DDOUBLECLICK,/**<单指双击>**/
  	DLONGPRESSED,/**<单指长按>**/
  	SLEFTGLIDE,/**<单指左滑>**/
  	SRIGHTGLIDE,/**<单指右滑>**/
  	SDOWNGLIDE,/**<单指下滑>**/
  	SUPGLIDE,/**<单指上滑>**/
    DUPGLIDE,/**<双指上滑>**/
    DDOWNGLIDE,/**<双指下滑>**/
    DLEFTGLIDE,/**<双指左滑>**/
    DRIGHTGLIDE,/**<双指右滑>**/
    SHRINK,/**<双指缩小手势>**/
    MAGNIFY,/**<双指放大手势>**/
  	DWROTATE,/**<双指顺时针滑动>**/
  	DCWROTATE,/**<双指逆时针滑动**/
  	DCLICK,/**<双指点击>**/
  	TCLICK,/**<三指点击>**/
  	TLEFTGLIDE,/**<三指向左滑动>**/
  	TRIGHTGLIDE,/**<三指向右滑动>**/
  	TDOWNGLIDE,/**<三指向下滑动>**/
  	TUPGLIDE,/**<三指向上滑动>**/
  	WCLICK,/**<四指点击>**/
	PCLICK,/**<五指点击>**/
    NONE,/**<未能识别手势>**/
  }eGesture_t;
  /*!
    该枚举定义了一个手指在屏幕上x或y轴的变化方向
  */
  typedef enum{
  	  UPL, /**<手指在y方向上的坐标减小>**/
  	  DOWNL,/**<手指在y方向上的坐标增大>**/
  	  LEFTL,/**<手指在x方向上的坐标减小>**/
  	  RIGHTL,/**<手指在x方向上的坐标增大>**/
  	  NOCHANGE,/**<手指变化范围太小，定义为未改变>**/
  	  UNIDENTIFIABLE,/**<无法识别>**/
  }eDirection_t;

  /*!
    该枚举定义了单个手指在屏幕上的变化
  */  
  typedef enum{
  	  LETTUP,
  	  LEFTDOWN,
  	  RIGHTUP,
  	  RIGHTDOWN,
  	  UP,
  	  DOWN,
  	  LEFT,
  	  RIGHT,
  	  CLICK, 
  	  UNABLE,
  }eDir_t;
  /**
   * @brief 获取触摸屏检测到的手势
   * @param str 字符串，里面包含了触摸点信息
   * @return eGesture_t 类型的变量代表了不同的手势
   */
  eGesture_t gesture(String str);
  /**
   * @brief 获取触摸屏检测到的手势
   * @param release 触摸的过程中手指是否松开
   * @param number 触摸手指的个数
   * @return eGesture_t 类型的变量代表了不同的手势
   */
  eGesture_t fingers(uint8_t release,uint8_t number);
  /**
   * @brief 计算从触摸开始第一手指的位置离第一个手指结束时的位置的距离
   * @return 两个点之间的距离
   */
  uint16_t clickCount();
  /**
   * @brief 将字符串里面点的信息转化到数组内以备用
   * @param str 字符串，里面包含了触摸点信息
   * @return 触摸屏检测到触摸点的个数
   */
  virtual uint8_t stringToPoint(String str)=0;
  
  /**
   * @brief 当只有单个手指触摸时手势识别的函数
   * @param  pointOne，第一个手指在屏幕上的变化方向
   * @return 单指手势
   */
  eGesture_t getGestureOne(eDir_t pointOne);
  /**
   * @brief 当有两个手指触摸时手势识别的函数
   * @param  pointOne，第一个手指在屏幕上的变化方向
   * @param  pointTwo，第二个手指在屏幕上的变化方向
   * @return 两指手势
   */
  eGesture_t getGestureTwo(eDir_t pointOne,eDir_t pointTwo);
  /**
   * @brief 当有三个手指触摸时手势识别的函数
   * @param  pointOne，第一个手指在屏幕上的变化方向
   * @param  pointTwo，第二个手指在屏幕上的变化方向
   * @param  pointThree，第三个手指在屏幕上的变化方向
   * @return 三指手势
   */
  eGesture_t getGestureThree(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree);
  /**
   * @brief 当有四个手指触摸时手势识别的函数
   * @param  pointOne，第一个手指在屏幕上的变化方向
   * @param  pointTwo，第二个手指在屏幕上的变化方向
   * @param  pointThree，第三个手指在屏幕上的变化方向
   * @param  pointFour，第四个手指在屏幕上的变化方向
   * @return 四指手势
   */
  eGesture_t getGestureFour(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree,eDir_t pointFour);
  /**
   * @brief 当有五个手指触摸时手势识别的函数
   * @param  pointOne，第一个手指在屏幕上的变化方向
   * @param  pointTwo，第二个手指在屏幕上的变化方向
   * @param  pointThree，第三个手指在屏幕上的变化方向
   * @param  pointFour，第四个手指在屏幕上的变化方向
   * @param  pointFive，第五个手指在屏幕上的变化方向
   * @return 五指手势
   */
  eGesture_t getGestureFive(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree,eDir_t pointFour,eDir_t pointFive);
  
  sTouchMessage_t tpDev;//触摸点信息
private:
  eDirection_t  directionX(uint16_t x1,uint16_t x2,uint16_t x3);
  eDirection_t  directionY(uint16_t y1,uint16_t y2,uint16_t y3);
  eDir_t  pointe(int point1);
  uint8_t bfDistance();
  uint8_t judgeRotate();
  uint16_t maxp(uint16_t num1,uint16_t num2,uint16_t num3);
  uint16_t minp(uint16_t num1,uint16_t num2,uint16_t num3);
  sTouchRecord_t touchge[5];
  sTouchRecord_t touchgeT[5];
  uint8_t lastPN = 0; 
  uint8_t release = 0;
  uint16_t change;
  int16_t changeX;
  int16_t changeY;
  int distance;
  uint8_t point ;
  eGesture_t lastGesture;
  uint8_t longpressed;
  uint16_t clickOldx,clickOldy;
  long long timer1,timer2;
  long timer3= 0;
  uint8_t click = 0;
  uint16_t distanceClick ;
    
  };


#endif