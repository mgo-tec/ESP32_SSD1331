# ESP32_SSD1331
This is Arduino core for the ESP32 library ( SPI ).  
Beta ver 1.5
HSPI only.  
  
(example)  
SCLK-----GPIO #14  
MOSI-----GPIO #13  
MISO-----NONE  
CS-------GPIO #15  
D/C------GPIO #21  
RST------GPIO #4  
  
Use SPI.h library  
  
# Change log
(1.5)  
The following function enabled to switch the device with CS pin  
- SSD1331_8x16_Font_DisplayOut  
- Scroller_8x16_RtoL4line  
- Display_Clear  
  
(1.4)  
Added function to display 8X8 dot Japanese kanji font.  
- SSD1331_8x8_Font_DisplayOut
  
(1.3)  
Since delay microseconds can not be used in ESP32 multitasking, we modified it to delay (1).  
In addition, another argument of the following function is added.  
- Scroller_8x16_RtoL4line  
  
(1.2)  
Add the following function.  
- Scroller_8x16_RtoL4line  
  
(1.1)  
Corrected scroll function of electric bulletin board.  
- Scroller_8x16_RtoL  
  
【更新履歴】(Japanese)  
(1.5)  
次の関数をCSピンでデバイスを切り替えられるようにしました。  
- SSD1331_8x16_Font_DisplayOut  
- Scroller_8x16_RtoL4line  
- Display_Clear  
  
(1.4)  
８×８ドットの美咲フォントを表示させるための関数を追加。  
- SSD1331_8x8_Font_DisplayOut
  
(1.3)  
ESP32 のマルチタスクでは delayMicroseconds が使えない為、delay(1)に修正しました。  
その他、以下の関数の別引数を追加  
- Scroller_8x16_RtoL4line  
  
(1.2)  
以下の関数を追加しました。
- Scroller_8x16_RtoL4line  
  
(1.1)  
電光掲示板風スクロール関数を修正しました。  
- Scroller_8x16_RtoL  
  
  
The MIT License (MIT)  
  
MITライセンス  
Copyright (c) 2017 Mgo-tec  
  
My Blog:  
https://www.mgo-tec.com