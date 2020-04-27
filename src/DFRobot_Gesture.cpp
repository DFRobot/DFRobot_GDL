/*!
 * @file DFRobot_Gesture.h
 * @brief DFRobot_Gesture Class Implementation
 * @copyright Copyright (c) 2010 DFRobot Co. Ltd (http://www.dfrobot.com)
 * @licence The MIT License (MIT)
 * @author [fengli] (li.feng@dfrobot.com)
 * @version V1.0
 * @date 2019-12-6
 * @get from https://www.dfrobot.com
 * @url https://github.com/DFRobot/DFRobot_GDL/src
 */
#include "DFRobot_Gesture.h"
#include <math.h>

DFRobot_Gesture::eGesture_t DFRobot_Gesture::fingers(uint8_t release,uint8_t number){
    uint8_t t = 0;
	uint8_t i = 0;
	uint8_t num1 = 0;
	uint8_t record = 0;


 if(release == 0){
	for(t=0;t<number;t++){
		if(tpDev.x[t]>touchge[t].pointx[point-1] + 30) record = 1;
		if(tpDev.y[t]>touchge[t].pointy[point-1] + 30) record = 1;
		if(tpDev.x[t]<touchge[t].pointx[point-1] - 30) record = 1;
		if(tpDev.y[t]<touchge[t].pointy[point-1] - 30) record = 1;
	}
	if(record == 1 ){
	  for(i=0;i<number;i++){
		  touchge[i].pointx[point] = tpDev.x[i];
	    touchge[i].pointy[point] = tpDev.y[i];
	  }
		point += 1;
	}
 }
	if(release == 1){
     switch(point){
			
		   case 1: {
		    for(uint8_t num=0;num<number;num++){
               touchge[num].pointx[1] = touchgeT[num].pointx[0];
	           touchge[num].pointy[1] = touchgeT[num].pointy[0];
               touchge[num].pointx[2] = touchgeT[num].pointx[0];
	           touchge[num].pointy[2] = touchgeT[num].pointy[0];
				 }
				  point = 3;
			 };break ;
			 case 2:{
				 for(uint8_t num=0;num<number;num++){
			     touchge[num].pointx[2] = touchgeT[num].pointx[0];
	             touchge[num].pointy[2] = touchgeT[num].pointy[0];
								 }
				
				 point = 3;
			 };break;
			 
			 default : break;
		 }
	  longpressed =0;
	 }
	
	for(num1=0;num1<number; num1++){
	    touchgeT[num1].pointx[0] =  tpDev.x[num1];
		touchgeT[num1].pointy[0] =  tpDev.y[num1];
	}

	if( point == 3) {

		
		
		eGesture_t ges1;
		distanceClick =  clickCount();
		switch(number){
			case 1: ges1 = getGestureOne(pointe(1));break;
			case 2: ges1 = getGestureTwo(pointe(1),pointe(2));break;
			case 3: ges1 = getGestureThree(pointe(1),pointe(2),pointe(3));break;
			case 4: ges1 = getGestureFour(pointe(1),pointe(2),pointe(3),pointe(4));break;
			case 5: ges1 = getGestureFive(pointe(1),pointe(2),pointe(3),pointe(4),pointe(5));break;
			default : ges1 = NONE;
		}
     //lastGesture = ges1;
     //timer2 = millis();
	  point = 0;
	  return ges1;
	}
    if(millis()>timer2+600 && click == 1){
		click = 0;
		lastGesture = NONE;
	    return SCLICK;
	}
	if(number == 1 &&  millis() - timer1>1600 && point == 1 && longpressed == 0 && distanceClick<100){
		longpressed = 1;
		lastGesture = DLONGPRESSED;
		return DLONGPRESSED;
		
	}
	return NONE;
}
uint16_t DFRobot_Gesture::clickCount(){
uint16_t distance1;


	int x1 = (touchge[0].pointx[0] - clickOldx);
	int y1 = (touchge[0].pointy[0] - clickOldy);
return sqrtf((unsigned long)x1*x1+(unsigned long)y1*y1);

}
uint8_t DFRobot_Gesture::bfDistance(){
  int olddisX = touchge[1].pointx[0] > touchge[0].pointx[0] ? (touchge[1].pointx[0]- touchge[0].pointx[0]) :(touchge[0].pointx[0] - touchge[1].pointx[0]);
  int olddisY = touchge[1].pointy[0] > touchge[0].pointy[0] ? (touchge[1].pointy[0]- touchge[0].pointy[0]) :(touchge[0].pointy[0] - touchge[1].pointy[0]);
  int disX = touchge[1].pointx[2] > touchge[0].pointx[2] ? (touchge[1].pointx[2]- touchge[0].pointx[2]) :(touchge[0].pointx[2] - touchge[1].pointx[2]);
  int disY = touchge[1].pointy[2] > touchge[0].pointy[2] ? (touchge[1].pointy[2]- touchge[0].pointy[2]) :(touchge[0].pointy[2] - touchge[1].pointy[2]);
  int d2 ;
	int x1 = (touchge[1].pointx[2] - touchge[0].pointx[2]);
	int y1 = (touchge[1].pointy[2] - touchge[0].pointy[2]);
    d2 = sqrtf((unsigned long)x1*x1+(unsigned long)y1*y1);
	int x2 = (touchge[1].pointx[0] - touchge[0].pointx[0]);
	int y2 = (touchge[1].pointy[0] - touchge[0].pointy[0]);
    distance = sqrtf((unsigned long)x2*x2+(unsigned long)y2*y2);


// for(uint8_t i=0 ;i < 2 ;i++){
	// Serial.print("point");
	// Serial.print(i);
	// Serial.print(": ");
	// for(uint8_t j=0 ;j < 3 ;j++){
		// Serial.print("(");
	// Serial.print(touchge[i].pointx[j]);
	// Serial.print(",");
	// Serial.print(touchge[i].pointy[j]);
// Serial.print(")");
// }
	// Serial.println(" ");
	
// }
    // Serial.print("distance2: ");
  // Serial.println(d2);
 // Serial.print("distance1: ");
  // Serial.println(distance);
  
	changeX = disX > olddisX ?(disX - olddisX):(olddisX - disX);
	changeY = disY > olddisY ?(disY - olddisY):(olddisY - disY);
	if(d2 > distance) {
	  change = d2-distance;
		return 1;
	}
  else{
		change = distance - d2;
	  return 0;
	}
}

DFRobot_Gesture::eDir_t  DFRobot_Gesture::pointe(int point1){
	uint16_t x1,x2,x3 ;
	uint16_t y1,y2,y3 ;
	eDirection_t linex ;
	eDirection_t liney ;
	x1 = touchge[point1-1].pointx[0];
	y1 = touchge[point1-1].pointy[0];
	x2 = touchge[point1-1].pointx[1];
	y2 = touchge[point1-1].pointy[1];
	x3 = touchge[point1-1].pointx[2];
	y3 = touchge[point1-1].pointy[2];
    //delay(100);
	linex = directionX(x1,x2,x3);
	liney = directionY(y1,y2,y3);
	if(linex == NOCHANGE && liney == NOCHANGE ){
	   return CLICK;
	}
	else if(linex == NOCHANGE && liney == UPL){
	   return UP;
	}
	else if(linex == NOCHANGE && liney == DOWNL){
	   return  DOWN;
	}
	else if(linex == LEFTL && liney == NOCHANGE){
	   return  LEFT;
	}
	else if(linex == LEFTL && liney == UPL){
	   return LETTUP;
	}
	else if(linex == LEFTL && liney == DOWNL){
	   return LETTUP;
	}
	else if(linex == RIGHTL && liney == NOCHANGE){
	   return RIGHT;
	}
	else if(linex == RIGHTL && liney == UPL){
	   return RIGHTUP;
	}
	else if(linex == RIGHTL && liney == DOWNL){
	   return RIGHTDOWN;
	}
	else{
	   return UNABLE;
	}
	
}

DFRobot_Gesture::eDirection_t DFRobot_Gesture::directionX(uint16_t x1,uint16_t x2,uint16_t x3){
  uint16_t average = (x1 + x2 + x3)/3;
  uint16_t min1 = minp(x1,x2,x3);
	uint16_t max1 = maxp(x1,x2,x3);
	
  if((average < min1+10) && (average > max1-10) ){
	   return NOCHANGE;
	}
  else if(x3 >= x2+10 && x2 > x1+10){
	   return RIGHTL;
	}
  else if(x3+10 <= x2 && x2 +10< x1){
	   return LEFTL;
	}
	else{
	   return UNIDENTIFIABLE;
	}
}

DFRobot_Gesture::eDirection_t DFRobot_Gesture::directionY(uint16_t y1,uint16_t y2,uint16_t y3){
  uint16_t average = (y1 + y2 + y3)/3;
  uint16_t min1 = minp(y1,y2,y3);
	uint16_t max1 = maxp(y1,y2,y3);

  if((average < min1+10)  && (average > max1-10) ){
	   return NOCHANGE;
	}
  else if((y3 >= y2) && (y2 > y1)){
	   return DOWNL;
	}
  else if((y3 <= y2) && (y2 < y1)){
	   return UPL;
	}
	else{
	   return UNIDENTIFIABLE;
	}
}

uint16_t DFRobot_Gesture::maxp(uint16_t num1,uint16_t num2,uint16_t num3){
  uint16_t max ;
	max = num1>=num2 ? num1 :num2;
	return max>=num3 ? max :num3;
}
uint16_t DFRobot_Gesture::minp(uint16_t num1,uint16_t num2,uint16_t num3){
  uint16_t min ;
	min = num1<=num2 ? num1 :num2;
	return min<=num3 ? min :num3;
}
DFRobot_Gesture::eGesture_t DFRobot_Gesture::getGestureOne(eDir_t pointOne){
   if(pointOne == CLICK &&  lastGesture != SCLICK && distanceClick<30 )
   {
	     timer2 = millis();
		 click = 1;
		 if(lastGesture == DLONGPRESSED){
			 click = 0;
		 }
	     lastGesture = SCLICK;

          return NONE;
   }
    else if(pointOne == CLICK &&  lastGesture == SCLICK)
	{     if((millis() - timer2)<500){
		  lastGesture = NONE;
		  click = 0;
	      return DDOUBLECLICK;
     	}
		else {
			lastGesture = NONE;
			click = 0;
			
			return SCLICK;
			
		}
	}
	else if(pointOne == LEFT || pointOne == LETTUP || pointOne == LEFTDOWN)
	{
	      return SLEFTGLIDE;
	}
	else if(pointOne == RIGHT || pointOne == RIGHTUP || pointOne == RIGHTDOWN)
	{
	      return SRIGHTGLIDE;
	}
	else if(pointOne == DOWN )
	{
	      return SDOWNGLIDE;
	}
	else if(pointOne == UP){
		  return SUPGLIDE;
	}
	else{
	    return NONE;
	}
}
uint8_t DFRobot_Gesture::judgeRotate(){
	int16_t x1 ,x2,x3,y1,y2,y3;
	int px12,py12;
	int px23,py23;
	double  concave;
	bool clockwise;
	
	x1 = touchge[0].pointx[0];
	y1 = 0 - touchge[0].pointy[0];
	x2 = touchge[0].pointx[1];
	y2 = 0 - touchge[0].pointy[1];	
	x3 = touchge[0].pointx[2];
	y3 = 0 - touchge[0].pointy[2];
	px12 = x2-x1;
	py12 = y2-y1;
	
	px23 = x3-x2;
	py23 = y3-y2;
	concave = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
	if(concave < 0)
		clockwise = 1;
	else if(concave > 0) 
		clockwise = 0;
    else
        return 2;	

	return clockwise;
	
}
DFRobot_Gesture::eGesture_t DFRobot_Gesture::getGestureTwo(eDir_t pointOne,eDir_t pointTwo){
  uint8_t dir = bfDistance();
  if(pointOne == CLICK && pointTwo == CLICK && distanceClick<30){
	   return DCLICK;
	 }
	else if((pointOne == LEFT || pointOne == LETTUP || pointOne == LEFTDOWN) && (pointTwo == LEFT || pointTwo == LETTUP || pointTwo == LEFTDOWN)){
	   return DLEFTGLIDE;
	}
	else if((pointOne == RIGHT || pointOne == RIGHTUP || pointOne == RIGHTDOWN) && (pointTwo == RIGHT || pointTwo == RIGHTUP || pointTwo == RIGHTDOWN)){
	   return DRIGHTGLIDE;
	}
	else if(pointOne == DOWN && pointTwo == DOWN){
	   return DDOWNGLIDE;
	}
	else if(pointOne == UP && pointTwo == UP){
	   return DUPGLIDE;
	}
	else if(distance > 100 && change > 100 && dir == 0){
	  return SHRINK;
	}
	else if(change > 100 && dir == 1){
	  return MAGNIFY;
	}
	else if(changeX>30 &&changeY >30 && change<80){
		uint8_t dir1 = judgeRotate();
		if( dir1 == 1) return DWROTATE;
	    else if(dir1 == 0)return DCWROTATE;
		else return NONE;
	}

	else{
	  return NONE;
	
	}



}
DFRobot_Gesture::eGesture_t DFRobot_Gesture::getGestureThree(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree){
  if(pointOne ==CLICK && pointTwo ==CLICK && pointThree ==CLICK&& distanceClick<30)
		return TCLICK;
	else if((pointOne == LEFT || pointOne == LETTUP || pointOne == LEFTDOWN) && (pointTwo == LEFT || pointTwo == LETTUP || pointTwo == LEFTDOWN)&&(pointThree == LEFT || pointThree == LETTUP || pointThree == LEFTDOWN))
    return TLEFTGLIDE;
	else if((pointOne == RIGHT || pointOne == RIGHTUP || pointOne == RIGHTDOWN) && (pointTwo == RIGHT || pointTwo == RIGHTUP || pointTwo == RIGHTDOWN)&& (pointThree == RIGHT || pointThree == RIGHTUP || pointThree == RIGHTDOWN))
    return TRIGHTGLIDE;
	else if(pointOne == DOWN && pointTwo == DOWN && pointThree == DOWN)
		return TDOWNGLIDE;
	else if(pointOne == UP && pointTwo == UP && pointThree == UP)
		return TUPGLIDE;
	else
		return NONE;
}
DFRobot_Gesture::eGesture_t DFRobot_Gesture::getGestureFour(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree,eDir_t pointFour){
    if(pointOne ==CLICK && pointTwo ==CLICK && pointThree ==CLICK&& pointFour ==CLICK)
		   return WCLICK;
		else
		   return NONE;

}
DFRobot_Gesture::eGesture_t DFRobot_Gesture::getGestureFive(eDir_t pointOne,eDir_t pointTwo,eDir_t pointThree,eDir_t pointFour,eDir_t pointFive){
    if(pointOne ==CLICK && pointTwo ==CLICK && pointThree ==CLICK&& pointFour ==CLICK&& pointFive ==CLICK)
		   return PCLICK;
		else
		   return NONE;
}
DFRobot_Gesture::eGesture_t DFRobot_Gesture::gesture(String str){
	uint8_t t;
	uint8_t number = stringToPoint(str);
	uint8_t nump = number;
	eGesture_t gesture =NONE;
	
	if(lastPN==0 && number >=1){
	    clickOldx = tpDev.x[0];
		clickOldy = tpDev.y[0];
		timer1 = millis();
	}
	
	if(lastPN > number){
	  number = lastPN;
		release = 1;	 
	}
	else if(lastPN < number){
	  point = 0;
	
	}
	else{
	  lastPN = number;
	}
	
	if(point == 0 && nump > 0){
	  for(t=0;t<number;t++){
	    touchge[t].pointx[0] = tpDev.x[t];
	    touchge[t].pointy[0] = tpDev.y[t];
		  }
	  point +=1;
	}
  if(millis() -timer3 > 100){
     gesture = fingers(release,number);    
  }
	if(release == 1){
	    timer3 = millis();
	}
  
	release = 0;
	lastPN = nump;
	return gesture;
}





