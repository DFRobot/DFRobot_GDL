# 自定义字体生成方式
## Table of Contents
* [环境搭建](#环境搭建)
* [生成自定义字体](#生成自定义字体)
## 环境搭建
在使用setup.py脚本生成自定义字体之前，需要做如下准备:
- **登录网址https://www.python.org/downloads/，下载python3.6及以上版本；**<br>
- **安装完毕后，需要用下列命令安装python第三方依赖包:** <br>
    - pip3 install numpy
    - pip install freetype-py 
    - pip install chardet

## 生成自定义字体
- **将TTF字体文件存放在ttf文件夹里，例如：SIMKAI.TTF（简体字 楷体）**
- **在text.txt文件中输入你想生成的字符，例如：你好，世界！**
- **打开config.txt文件，配置生成字体文件的名字前缀和字体大小:**
    - **FontFileNamePre: 生成的字体文件名前缀**
    - **FontSizeList: 生成的字体的大小列表，如果为空[]，则默认生成12,18,24,36,48,72大小的文件, 如果是汉字，不要小于12**<br>
  **FontFileNamePre**代表文件名的前缀，如果为default，则代表无前缀<br>
        - 例1：如果**FontFileNamePre = default**<br>
             **FontSizeList = [12]**<br>
    则会生成一个名字为**SIMKAIFont12pt.h**的文件<br>
        - 例2：如果**FontFileNamePre = English**<br>
             **FontSizeList = [12]**<br>
    则会生成一个名字为**EnglishSIMKAIFont12pt.h**的文件<br>
        - 例3：如果FontFileNamePre = English<br>
             **FontSizeList = [12,24]**<br>
    则会生成名字为**EnglishSIMKAIFont12pt.h**和**EnglishSIMKAIFont24pt.h**的文件<br>
**生成字体文件命名和后缀 = FontFileNamePre + TTF文件名 + Font + size + pt + .h**<br>
其中FontFileNamePre只能是**英文字符**,size,即生成字体的大小<br>
- **运行setup.py脚本，会在font文件夹生成一系列后缀名为.h的字体文件，并弹出一个font.txt的文本，再进行以下步骤，即可在屏上显示：你好，世界！<br>**
    - 将font文件夹里的文件复制到DFRobot_GDL\src\Fonts\Fonts目录下;
    - 将弹出的font.txt的内容粘贴到DFRobot_GDL\src\Fonts\DFRobot_Font.h文件中;
    - 打开Arduino IDE，构造屏对象，如tft，调用tft.setFont(&SIMKAIFont48pt);
    - 调用tft.println("你好，世界！"),此时即可在屏上显示"你好，世界！"
- **注意：UNO等主控的ROM和RAM比较少，如果你一次生成的字体多，会因内存不足，而报错**
