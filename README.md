# DFRobot_GDL
DFRobot_GDL is a comprehensive display interaction framework that supports multiple MCUs, supporting display, touch, and UI.<br>
It supports a variety of display ICs and touch ICs. Users can use GDL to drive a wide variety of displays, such as colorful screen, black and white screen, e-ink screen and so on.<br>

GDL is designed to make your project more colorful by making it easier and faster to get started and learn.

## GDL Software
GDL integrates a number of functional components contained in the wiki.<br>

#### GDL WIKI
* [点击此处查看中文wiki](https://github.com/DFRobot/DFRobot_GDL/wiki/中文-WIKI)<br>
* [Click here to check its wiki](https://github.com/DFRobot/DFRobot_GDL/wiki/English-WIKI)

#### Basic Display
* Display graphics: lines, dots, geometry, etc.<br>
![Triangle](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/Triangle.jpg)
![Round](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/Circle.jpg)
![Rectangular](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/Rect.jpg)
* Show graphics<br>
    * Supports display of 16-bit, 24-bit BMP images.
    * Supports display of 32-bit BMP-format pictures, but because 32-bit
    * cannot set the background color, the transparent part is black.
    * Support for displaying pictures in jpg format.
    * There are some icons available for your reference, please [click here](https://github.com/DFRobot/DFRobot_Icon) to download and
    * check.

* Built-in Font<br>
&nbsp;Below is a list of built-in fonts that can be used<br>
    * FreeMono9pt7b
    * FreeMono12pt7b
    * FreeMono18pt7b
    * FreeMono24pt7b
    * FreeMonoBold12pt7b
    * FreeMonoBoldOblique12pt7b
    * FreeMonoOblique12pt7b
    * FreeSans12pt7b
    * FreeSansBold9pt7b
    * FreeSansBold12pt7b
    * FreeSansBoldOblique12pt7b
    * FreeSansOblique12pt7b
    * FreeSerif12pt7b
    * FreeSerifBold12pt7b
    * FreeSerifBoldItalic12pt7b
    * FreeSerifItalic12pt7b
    * SIMKAIFont12pt
    * SIMKAIFont18pt
    * SIMKAIFont24pt
    * SIMKAIFont36pt
    * SIMKAIFont48pt
    * SIMKAIFont72pt

* ttf font<br>
    * Users can customize fonts by themselves;Its generation and learning files are stored in the
    * DFRobot_GDL\src\Fonts\FontCustomScript.
    * [Please click here] to learn how to use it.
    * (https://github.com/DFRobot/DFRobot_GDL/blob/master/src/Fonts/FontCustomScript/README.md)


#### Touch
* Click/double-click<br>

* Press/swipe<br>

* Gesture operation<br>
    * Single point gestures (supported by resistance display and capacitive screens)
        * Slide to top, bottom, left and right by one finger
    * Multi-point gestures (supported by multi-touch capacitive screen)
        * Slide to top, bottom, left and right by two fingers
        * Magnify or minify fonts by two fingers
        * Rotate fonts by two fingers
        * Rotate images by two fingers



#### UI功能
* Slider<br>
![滑条](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/1.jpg)
* Progress Bar<br>
![进度条](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/2.jpg)
* Text Box<br>
![文本框](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/3.jpg)
* Keys<br>
![按键1](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/4.jpg)
![按键2](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/5.jpg)
* Table View<br>
![表格视图](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/6.jpg)
* XYZ Control(Not released yet）<br>


## GDL Compatibility

#### Support MCU
The GDL can support a variety of MCUs, and the following table allows you to view the supported MCU models.

GDL          |     Uno      |   Leonardo    |     ESP32     |    ESP8266    |     m0         |     BK7251      
-----------  | :----------: |  :----------: |  :----------: |  :----------: | :------------: |   ------------
Geometric Figure      |      √       |       √       |      √        |       √       |       √        |  
BMP Image Decode  |      √       |       √       |      √        |       √       |       √        |
Read SD      |      √       |               |               |       √       |       √        |    
Built-in Fonts      |      √       |       √       |      √        |       √       |       √        |
ttf Fonts      |      √       |       √       |      √        |       √       |       √        |
Single-point Touch     |      √       |       √       |      √        |       √       |       √        |
Multiple-point Touch      |              |               |               |               |                |
Gesture Operation      |      √       |       √       |      √        |       √       |       √        |
Slider          |      √       |       √       |      √        |       √       |       √        |
Progress Bar        |      √       |       √       |      √        |       √       |       √        | 
Text Box      |      √       |       √       |      √        |       √       |       √        | 
Keys         |      √       |       √       |      √        |       √       |       √        | 
Table View     |      √       |       √       |      √        |       √       |       √       |     

#### Display IC
This GDL supports a wide range of display-driven ICs and resolutions, and supports the DFRobot series screens and Adafruit series screens, the available drivers are  shown in the following table.<br>

Product Name     |      Product Pictures    |  Link
----------  | :-------------: | --------
1.54"       |                 |  [购买链接]()

 

#### Touch <br>
The GDL supports a variety of touch ICs, and the touch drivers supported are as follows:<br>

GDL: Touch IC       |  XPT2046     |    GT911     |   GT5688
---------------- | :----------: | :----------: |------------



## Install GDL firmware
GDL meets the needs of people at different levels of software development. With the GDL library, even users don't have a lot of Arduino or software development experience, they also can achieve cool display.

1. Download Arduino IDE
1. Download GDL code from [github](https://github.com/dfrobot/DFRobot_GDL)
1. Open “basicTest.ino” from files downloaded from `Arduino IDE`.
1. Connect `Arduino Uno`
1. Select Tools>boards:`Arduino Uno` and Tools>Ports: Arduino Uno or other boards
1. Click "Verify/Compile" and "Upload"the code to the board.

## Tutorial Video
This is a 120-minute instructional video designed to help beginners quickyly get started.


1. GDL Introduction - 5 minutes

2. How to donwload and install GDL and basic use - 5 minutes

3. How to use GDL show figures - 20 minutes

4. How to use GDL show words - 10 minutes

5. How to use GDL show pictures - 5 minutes

6. How to use ttf fonts in GDL - 10 minutes

7. GDL Touch Introduction - 5 minutes

8. GDL-Touch basic operation - 5 minutes

9. GDL-Touch gesture operation - 10 minutes

10. GDL UI Introduction - 5 minutes

11. GDL-UI-Text control - 5 minutes

12. GDL-UI-slider/progressbar control  - 5 minutes

13. GDL-UI-button control - 5 minutes

14. GDL-UI-Tableview control - 5 minutes

15. GDL-UI-XYZ system control - 5 minutes


## FAQ

1.Q: why some examples cannot run in Arduino UNO/Leonardo? <br>
&nbsp;&nbsp; A: For the lack of store memory, Arduino UNO/Leonardo cannot store the full program. You can try it with other boards with bigger storage memory such as Firebettle-ESP32 and so on. There are introductions in each head fil ino for your reference.

## About Branchs
**Branch: master ** is the latest stable version.

**Other branches** are development versions to store experimental versions that have not been fully tested.  

## 待办事项清单
- [ ]坐标系控件
- [ ]教学视频
- [ ]支持屏幕的链接


## Contacts
Please contact us via e-mail addresses as below:<br>
* Ajax.zhong@dfrobot.com
* Ivey.lu@dfrobot.com
* li.feng@dfrobot.com

## Thanks
* Internally integrated Adafruit's [GFX](https://learn.adafruit.com/adafruit-gfx-graphics-library) library. 
* The font section refers to the encoding format of [u8g2](https://github.com/olikraus/u8g2)
