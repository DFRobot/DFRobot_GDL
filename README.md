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



#### UI
* 滑条<br>
![滑条](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/1.jpg)
* 进度条<br>
![进度条](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/2.jpg)
* 文本框<br>
![文本框](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/3.jpg)
* 按键<br>
![按键1](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/4.jpg)
![按键2](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/5.jpg)
* 表格视图<br>
![表格视图](https://github.com/DFRobot/DFRobot_GDL/raw/master/image/6.jpg)
* 坐标系控件(该功能暂未发布）<br>


## GDL Compatibility

#### Support MCU
The GDL can support a variety of MCUs, and the following table allows you to view the supported MCU models.

GDL          |     Uno      |   Leonardo    |     ESP32     |    ESP8266    |     m0         |     BK7251      
-----------  | :----------: |  :----------: |  :----------: |  :----------: | :------------: |   ------------
geometric figure     |      √       |       √       |      √        |       √       |       √        |  
BMP image decode  |      √       |       √       |      √        |       √       |       √        |
read SD      |      √       |               |               |       √       |       √        |    
built-in fonts   |      √       |       √       |      √        |       √       |       √        |
ttf fonts    |      √       |       √       |      √        |       √       |       √        |
single point touch     |      √       |       √       |      √        |       √       |       √        |
multiple point touch    |              |               |               |               |                |
gesture operation     |      √       |       √       |      √        |       √       |       √        |
slider         |      √       |       √       |      √        |       √       |       √        |
progress bar          |      √       |       √       |      √        |       √       |       √        | 
text box      |      √       |       √       |      √        |       √       |       √        | 
keys         |      √       |       √       |      √        |       √       |       √        | 
table view     |      √       |       √       |      √        |       √       |       √       |     

#### Display IC
This GDL supports a wide range of display-driven ICs and resolutions, and supports the DFRobot series screens and Adafruit series screens, the available drivers are  shown in the following table.<br>

Product Name    |      Product Pictures    |  Link
----------  | :-------------: | --------
1.54"       |                 |  [购买链接]()

 

#### Touch<br>
The GDL supports a variety of touch ICs, and the touch drivers supported are as follows:<br>

GDL：Touch IC       |  XPT2046     |    GT911     |   GT5688
---------------- | :----------: | :----------: |------------



## Install GDL firmware
GDL meets the needs of people at different levels of software development. With the GDL library, even users don't have a lot of Arduino or software development experience, they also can achieve cool display.

1. 下载Arduino IDE
1. 从[github](https://github.com/dfrobot/DFRobot_GDL)下载GDL代码
1. 使用`Arduino IDE`从下载的文件中打开“basicTest.ino”
1. 连接您的`Arduino Uno`开发板
1. 选择工具>板子：`Arduino Uno`和工具>端口：选择你的Arduino开发板
1. 单击“验证”，然后将软件“上传”到您的开发板上

## 教学
这是一个有120分钟的教学视频；旨在让初学者快速入门。

1. GDL简介--5分钟

1. GDL下载安装及基础使用--5分钟

1. GDL图形显示--20分钟

1. GDL文字显示--10分钟

1. GDL图片显示--5分钟

1. GDL使用ttf字库--10分钟

1. GDL Touch简介--5分钟

1. GDL-Touch基本操作--5分钟

1. GDL-Touch手势操作--10分钟

1. GDL UI简介--5分钟

1. GDL-UI-Text控件--5分钟

1. GDL-UI-滑条/进度条控件--5分钟

1. GDL-UI-Button控件--5分钟

1. GDL-UI-Tableview控件--5分钟

1. GDL-UI-直角坐标系控件--5分钟
 

## 常见问题

1.问：为什么有的示例程序在Arduino UNO/Leonardo上运行不了<br>
&nbsp;&nbsp; 答：因为Arduino UNO/Leonardo内存大小，无法存储下整个程序；您可以换一个内存较大的主控，如Firebettle-ESP32等。我们再每一个ino的头文件都有对应的说明，大家可以详细阅读。

## 关于分支
**master分支** 是最新的稳定版本

**其他分支** 是开发版本，用于存放未经完全测试的实验版本，使用者可以试用并通过issues提出改进意见或反馈bug

## 待办事项清单
- [ ]坐标系控件
- [ ]教学视频
- [ ]支持屏幕的链接


## Contacts
Please contact us via e-mail addresses as below<br>
* Ajax.zhong@dfrobot.com
* Ivey.lu@dfrobot.com
* li.feng@dfrobot.com

## Thanks
* 内部集成了Adafruit的GFX库，[GFX](https://learn.adafruit.com/adafruit-gfx-graphics-library)库地址
* 字体部分参考了u8g2的编码格式，[u8g2](https://github.com/olikraus/u8g2)库地址
