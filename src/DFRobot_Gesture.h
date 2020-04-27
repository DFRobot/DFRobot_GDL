/*!
 * @file DFRobot_Gesture.h
 * @brief Basic structure of DFRobot_Gesture class. Recognize gestures by collecting the points on the touch screen where the finger swipes
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src
 */
#ifndef __DFROBOT_GESTURE_H
#define __DFROBOT_GESTURE_H

#include "Arduino.h"

#define POINT 3

class DFRobot_Gesture{
public:
//Used to store the information of a point from pressed-state and released-state on the touch screen
typedef struct{
    uint16_t pointx[POINT]; //X-coordinates of all recorded points
	uint16_t pointy[POINT]; //Y-coordinates of all recorded points
	uint8_t id; //Touch point id
	uint8_t pressed; //Whether it was pressed
}sTouchRecord_t;
//Record the location information of five points in real-time //
typedef struct{
	uint16_t x[5];
	uint16_t y[5];
	
}sTouchMessage_t;
  /*!
    The enumeration defines different gestures
  */
  typedef enum {
  	SCLICK,/**<Single finger tap>**/
  	DDOUBLECLICK,/**<Single finger double-tap>**/
  	DLONGPRESSED,/**<Single finger long-press>**/
  	SLEFTGLIDE,/**<One finger swipes left>**/
  	SRIGHTGLIDE,/**<One finger swipes right>**/
  	SDOWNGLIDE,/**<One finger swipes down>**/
  	SUPGLIDE,/**<One finger swipes up>**/
    DUPGLIDE,/**<Two fingers swipe up>**/
    DDOWNGLIDE,/**<Two fingers swipe down>**/
    DLEFTGLIDE,/**<Two fingers swipe left>**/
    DRIGHTGLIDE,/**<Two fingers swipe right>**/
    SHRINK,/**<Two fingers zoom out>**/
    MAGNIFY,/**<Two fingers zoom in>**/
  	DWROTATE,/**<Two fingers rotate clockwise>**/
  	DCWROTATE,/**<Two fingers rotate counterclockwise>**/
  	DCLICK,/**<Two-fingers tap>**/
  	TCLICK,/**<Three-fingers tap>**/
  	TLEFTGLIDE,/**<Three-fingers swipe left>**/
  	TRIGHTGLIDE,/**<Three-fingers swipe right>**/
  	TDOWNGLIDE,/**<Three-fingers swipe down>**/
  	TUPGLIDE,/**<Three-fingers swipe up>**/
   	WCLICK,/**<Four-fingers tap>**/
	PCLICK,/**<Five-fingers tap>**/
    NONE,/**<Gesture not recognized>**/
  }eGesture_t;
  /*!
    The enumeration defines a finger's change of direction in X or Y axis on the screen
  */
  typedef enum{
  	  UPL, /**<The finger's coordinate in the y direction decreases>**/
  	  DOWNL,/**<The finger's coordinate in the y direction increases>**/
  	  LEFTL,/**<The finger's coordinate  in the x direction decreases>**/
  	  RIGHTL,/**<The finger's coordinate in the x direction increases>**/
  	  NOCHANGE,/**<The range of finger changing is too small, defined as unchanged>**/
  	  UNIDENTIFIABLE,/**<Cannot be recognized>**/
  }eDirection_t;

  /*!
    The enumeration defines the change of a single finger on the screen
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
    * @brief Get the gesture detected by the touch screen
    * @param str String, which contains touch point information
    * @return  Variables of eGesture_t type represent different gestures
    */
  eGesture_t gesture(String str);
  /**
    * @brief Get the gesture detected by the touch screen
    * @param release Whether the finger released during the touch
    * @param number The number of fingers touching the screen
    * @return Variables of eGesture_t type represent different gestures
    */
  eGesture_t fingers(uint8_t release,uint8_t number);
  /**
    * @brief Calculate the distance from the touch beginning position of the first finger to the end position of the first finger. 
    * @return Distance between two points
    */
  uint16_t clickCount();
  /**
    * @brief Convert the point information in the string into array for later use
    * @param str String, which contains touch point information
    * @return The number of touch points detected by the touch screen
    */
  virtual uint8_t stringToPoint(String str)=0;
  
  /**
    * @brief The function of gesture recognition for single finger touch
    * @param pointOne, the change direction of the first finger on the screen
    * @return Single-finger gesture
    */
  eGesture_t getGestureOne(eDir_t pointOne);
  /**
    * @brief The function of gesture recognition for two-fingers touch
    * @param pointOne, the change direction of the first finger on the screen
    * @param pointTwo, the change direction of the second finger on the screen
    * @return Two-fingers gesture
    */
  eGesture_t getGestureTwo(eDir_t pointOne,eDir_t pointTwo);
  /**
    * @brief The function of gesture recognition for three-fingers touch
    * @param pointOne, the change direction of the first finger on the screen
    * @param pointTwo, the change direction of the second finger on the screen
    * @param pointThree, the change direction of the third finger on the screen
    * @return Three-fingers gesture
    */
  eGesture_t getGestureThree(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree);
  /**
    * @brief The function of gesture recognition for four-fingers touch
    * @param pointOne, the change direction of the first finger on the screen
    * @param pointTwo, the change direction of the second finger on the screen
    * @param pointThree, the change direction of the third finger on the screen
    * @param pointFour, the change direction of the fourth finger on the screen
    * @return Four-fingers gesture
    */
  eGesture_t getGestureFour(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree,eDir_t pointFour);
  /**
    * @brief The function of gesture recognition for five-fingers touch
    * @param pointOne, the change direction of the first finger on the screen
    * @param pointTwo, the change direction of the second finger on the screen
    * @param pointThree, the change direction of the third finger on the screen
    * @param pointFour, the changing direction of the fourth finger on the screen
    * @param pointFive, the change direction of the fifth finger on the screen
    * @return Five-fingers gesture
    */
  eGesture_t getGestureFive(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree,eDir_t pointFour,eDir_t pointFive);
  
  sTouchMessage_t tpDev;//Touch point information
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
