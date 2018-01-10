/*
  ESP32_SSD1331.cpp - for Arduino core for the ESP32 ( Use SPI library ).
  Beta version 1.71
  
The MIT License (MIT)

Copyright (c) 2017 Mgo-tec. All rights reserved.
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

SPI.h (SPI library for ESP32)
Copyright (c) 2015 Hristo Gochkov. All rights reserved.
License : LGPL 2.1
*/

#include "ESP32_SSD1331.h"

ESP32_SSD1331::ESP32_SSD1331(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t dc, uint8_t rst)
  :_sck(sck)
  , _miso(miso)
  , _mosi(mosi)
  , _cs(cs)
  , _dc(dc)
  , _rst(rst)
{}

SPIClass spi(HSPI);

//************ SSD1331 初期化 ****************************************
void ESP32_SSD1331::SSD1331_Init(){
  ESP32_SSD1331::SSD1331_Init(true, 0, 0, 0);
}

//************ SSD1331 初期化 ****************************************
void ESP32_SSD1331::SSD1331_Init(bool bl, uint8_t cs1, uint8_t cs2, uint8_t cs3){
  pinMode(_rst, OUTPUT); //RES
  pinMode(_dc, OUTPUT); //DC

  digitalWrite(_rst, HIGH);
  digitalWrite(_rst, LOW);
  delay(1);
  digitalWrite(_rst, HIGH);

  digitalWrite(_dc, HIGH);

  if(bl == false){
    spi.begin(_sck, _miso, _mosi, -1);
  }else{
    spi.begin(_sck, _miso, _mosi, _cs);
  }
  
  spi.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  spi.setDataMode(SPI_MODE3);
  spi.setHwCs(bl);
  
  if(cs1 > 0){
    pinMode(cs1, OUTPUT);
    digitalWrite(cs1, HIGH);
    digitalWrite(cs1, LOW);
  }
  if(cs2 > 0){
    pinMode(cs2, OUTPUT);
    digitalWrite(cs2, HIGH);
    digitalWrite(cs2, LOW);
  }
  if(cs3 > 0){
    pinMode(cs3, OUTPUT);
    digitalWrite(cs3, HIGH);
    digitalWrite(cs3, LOW);
  }

  CommandWrite(0xAE); //Set Display Off
  CommandWrite(0xA0); //Remap & Color Depth setting　
    CommandWrite(0b00110010); //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format
  CommandWrite(0xA1); //Set Display Start Line
    CommandWrite(0);
  CommandWrite(0xA2); //Set Display Offset
    CommandWrite(0);
  CommandWrite(0xA4); //Set Display Mode (Normal)
  CommandWrite(0xA8); //Set Multiplex Ratio
    CommandWrite(0b00111111); //15-63
  CommandWrite(0xAD); //Set Master Configration
    CommandWrite(0b10001110); //a[0]=0 Select external Vcc supply, a[0]=1 Reserved(reset)
  CommandWrite(0xB0); //Power Save Mode
    CommandWrite(0b00000000); //0x1A Enable power save mode
  CommandWrite(0xB1); //Phase 1 and 2 period adjustment
    CommandWrite(0x74);
  CommandWrite(0xB3); //Display Clock DIV
    CommandWrite(0xF0);
  CommandWrite(0x8A); //Pre Charge A
    CommandWrite(0x81);
  CommandWrite(0x8B); //Pre Charge B
    CommandWrite(0x82);
  CommandWrite(0x8C); //Pre Charge C
    CommandWrite(0x83);
  CommandWrite(0xBB); //Set Pre-charge level
    CommandWrite(0x3A);
  CommandWrite(0xBE); //Set VcomH
    CommandWrite(0x3E);
  CommandWrite(0x87); //Set Master Current Control
    CommandWrite(0x06);
  CommandWrite(0x15); //Set Column Address
    CommandWrite(0);
    CommandWrite(95);
  CommandWrite(0x75); //Set Row Address
    CommandWrite(0);
    CommandWrite(63);
  CommandWrite(0x81); //Set Contrast for Color A
    CommandWrite(255);
  CommandWrite(0x82); //Set Contrast for Color B
    CommandWrite(255);
  CommandWrite(0x83); //Set Contrast for Color C
    CommandWrite(255);

  for(int j=0; j<64; j++){ //Display Black OUT
    for(int i=0; i<96; i++){
      DataWrite(0x00);
      DataWrite(0x00);
      DataWrite(0x00);
    }
  }

  CommandWrite(0xAF); //Set Display On
  
  if(cs1 > 0){
    digitalWrite(cs1, HIGH);
  }
  if(cs2 > 0){
    digitalWrite(cs2, HIGH);
  }
  if(cs3 > 0){
    digitalWrite(cs3, HIGH);
  }
  delay(110); //ディスプレイONコマンドの後は最低100ms 必要
}
//*********** HSPI 通信送信 ***********************************
void ESP32_SSD1331::CommandWrite(uint8_t b){
  digitalWrite(_dc, LOW);//DC
  spi.write(b);
}

void ESP32_SSD1331::DataWrite(uint8_t b){
  digitalWrite(_dc, HIGH);//DC
  spi.write(b);
}

void ESP32_SSD1331::CommandWriteBytes(uint8_t *b, uint16_t n){
  digitalWrite(_dc, LOW);//DC
  spi.writeBytes(b, n);

}

void ESP32_SSD1331::DataWriteBytes(uint8_t *b, uint16_t n){
  digitalWrite(_dc, HIGH);//DC
  spi.writeBytes(b, n);
}

//***********画面明るさ********************************
void ESP32_SSD1331::Brightness(uint8_t brightness){
  CommandWrite(0x81); //Set Contrast for Color A
    CommandWrite(brightness);
  CommandWrite(0x82); //Set Contrast for Color B
    CommandWrite(brightness);
  CommandWrite(0x83); //Set Contrast for Color C
    CommandWrite(brightness);
}
//***********画面明るさフェードイン**********************
void ESP32_SSD1331::Brightness_FadeIn(uint8_t interval){
  for(int brightness = 0; brightness < 256; brightness++){
    CommandWrite(0x81); //Set Contrast for Color A
      CommandWrite(brightness);
    CommandWrite(0x82); //Set Contrast for Color B
      CommandWrite(brightness);
    CommandWrite(0x83); //Set Contrast for Color C
      CommandWrite(brightness);
    delay(interval);
  }
}
//***********画面明るさフェードアウト********************
void ESP32_SSD1331::Brightness_FadeOut(uint8_t interval){
  for(int brightness = 255; brightness >= 0; brightness--){
    CommandWrite(0x81); //Set Contrast for Color A
      CommandWrite(brightness);
    CommandWrite(0x82); //Set Contrast for Color B
      CommandWrite(brightness);
    CommandWrite(0x83); //Set Contrast for Color C
      CommandWrite(brightness);
    delay(interval);
  }
}
//***********ディスプレイ消去***************************
void ESP32_SSD1331::Display_Clear(uint8_t cs, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
  spi.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  spi.setDataMode(SPI_MODE3);
  //spi.setHwCs(bl);
  digitalWrite(cs, LOW);
  ESP32_SSD1331::Display_Clear(x0, y0, x1, y1);
  digitalWrite(cs, HIGH);
}

//***********ディスプレイ消去***************************
void ESP32_SSD1331::Display_Clear(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1){
  //delayMicroseconds(500); //クリアーコマンドは400μs 以上の休止期間が必要かも
  delay(1);
  CommandWrite(0x25); //Clear Window
    CommandWrite(x0); //Column Address of Start
    CommandWrite(y0); //Row Address of Start
    CommandWrite(x1); //Column Address of End
    CommandWrite(y1); //Row Address of End
  //delayMicroseconds(800); //ここの間隔は各自調節してください。
  delay(1);
}
//********** 範囲コピー *********************************
void ESP32_SSD1331::SSD1331_Copy(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t X, uint8_t Y){
  uint8_t buf[7];
  buf[0] = 0x23; //Copy Command
  buf[1] = x0; buf[2] = y0; buf[3] = x1; buf[4] = y1; buf[5] = X; buf[6] = Y; 

  //delayMicroseconds(500);
  delay(1);
  CommandWriteBytes(buf, sizeof(buf)); 
  //delayMicroseconds(500);
  delay(1);
}
//********* OLED 8x16フォント出力 ********************
void ESP32_SSD1331::SSD1331_8x16_Font_DisplayOut(uint8_t cs, uint8_t txtMax, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t Fnt[][16]){
  digitalWrite(cs, LOW);
  ESP32_SSD1331::SSD1331_8x16_Font_DisplayOut(txtMax, x0, y0, red, green, blue, Fnt);
  digitalWrite(cs, HIGH);
}
//********* OLED 8x16フォント出力 ********************
void ESP32_SSD1331::SSD1331_8x16_Font_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t Fnt[][16]){
  uint8_t com[6];
  spi.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  spi.setDataMode(SPI_MODE3);
  //spi.setHwCs(true);
  
  if( txtMax > 12) txtMax = 12;
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0 + (8*txtMax) - 1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0 + 15;

  CommandWriteBytes(com, 6);

  int i, j, k;
  uint8_t bt = 0b10000000;

  uint8_t Dot = (red << 5) | (green << 2) | blue;

  uint8_t dummy[16*txtMax*8];
  uint16_t cnt = 0;

  for(i=0; i<16; i++){
    for(j=0; j<txtMax; j++){
      for(k=0; k<8; k++){
        if( Fnt[j][i] & (bt >> k) ){
          dummy[cnt] = Dot;
        }else{
          dummy[cnt] = 0;
        }
        cnt++;
      }
    }
  }

  DataWriteBytes(dummy, (16 * txtMax * 8));
}
//********* OLED 8x16フォント　サイズアップ　出力 ********************
void ESP32_SSD1331::SizeUp_8x16_Font_DisplayOut(uint8_t Size, uint8_t txtMax, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t Fnt[][16]){
  ESP32_SSD1331::HVsizeUp_8x16_Font_DisplayOut(Size, Size, txtMax, x0, y0, red, green, blue, Fnt);
}
//********* OLED 8x16フォント　サイズアップ　出力 ********************
void ESP32_SSD1331::HVsizeUp_8x16_Font_DisplayOut(uint8_t H_Size, uint8_t V_Size, uint8_t txtMax, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t Fnt[][16]){
  uint8_t com[6];
  spi.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  spi.setDataMode(SPI_MODE3);
  //spi.setHwCs(true);
  
  if( txtMax > 12) txtMax = 12;
  
  if(H_Size == 4){
    txtMax = 3;
  }else if(H_Size == 2){
    txtMax = 6;
  }
  uint8_t txtmax_sizeup = txtMax * (8 * H_Size);
  
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    //com[2] = 95;
    com[2] = x0 + txtmax_sizeup - 1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0 + (16*V_Size)-1;
    //com[5] = 63;

  CommandWriteBytes(com, 6);

  int i, j, k, ii;
  uint8_t bt = 0b10000000;

  uint8_t Dot = (red << 5) | (green << 2) | blue;
  
  uint8_t dummy[16*V_Size][96] = {};
  
  uint16_t cnt = 0;

  uint8_t size_cnt=0;
  for(i=0; i<16; i++){
      for(j=0; j<txtMax; j++){
        for(k=0; k<8; k++){
          if( Fnt[j][i] & (bt >> k) ){
            for(ii=0; ii<H_Size; ii++){
              dummy[i][cnt++] = Dot;
            }
          }else{
            cnt = cnt + H_Size;
          }
        }
      }
      cnt = 0;
  }
  //digitalWrite(_dc, LOW);//DC

  for(i=0; i<16; i++){
    for(size_cnt=0; size_cnt<V_Size; size_cnt++){
      DataWriteBytes(dummy[i], (txtmax_sizeup));
      //spi.writeBytes(dummy[i], (txtmax_sizeup));
    }
  }
}
//********* OLED 8x8フォント出力 ********************
void ESP32_SSD1331::SSD1331_8x8_Font_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t Fnt[][8]){
  uint8_t com[6];
  spi.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  spi.setDataMode(SPI_MODE3);
  //spi.setHwCs(true);

  if( txtMax > 12) txtMax = 12;
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0 + (8*txtMax) - 1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0 + 7;

  CommandWriteBytes(com, sizeof(com));

  int i, j, k;
  uint8_t bt = 0b10000000;

  uint8_t Dot = (red << 5) | (green << 2) | blue;

  uint8_t dummy[8*txtMax*8];
  uint16_t cnt = 0;

  for(i=0; i<8; i++){
    for(j=0; j<txtMax; j++){
      for(k=0; k<8; k++){
        if( Fnt[j][i] & (bt >> k) ){
          dummy[cnt] = Dot;
        }else{
          dummy[cnt] = 0;
        }
        cnt++;
      }
    }
  }

  DataWriteBytes(dummy, (8 * txtMax * 8));
}
//*********** 時刻垂直スクロール ****************
void ESP32_SSD1331::Time_Copy_V_Scroll(uint8_t Direction, uint8_t ZorH, uint8_t buf[2][16], uint8_t *SclCnt, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t col_R, uint8_t col_G, uint8_t col_B){
  ESP32_SSD1331::SizeUp_Copy_V_Scroll(false, 1, Direction, ZorH, buf, *SclCnt, x0, y0, y1, col_R, col_G, col_B);
  (*SclCnt)++;
}
//*********** 時刻水平スクロール **********************
void ESP32_SSD1331::Time_Copy_H_Scroll(uint8_t Direction, uint8_t ZorH, uint8_t buf[2][16], uint8_t *SclCnt, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t col_R, uint8_t col_G, uint8_t col_B){
  uint8_t com[6];
  uint8_t Dot = (col_R << 5) | (col_G << 2) | col_B;
  uint8_t i;
  uint8_t k = 0;
  uint8_t bbb = 0;

  switch( Direction ){
    case 0:
      SSD1331_Copy(x0+1, y0, x1, y1, x0, y0);

      com[0] = 0x15; //Set Column Address
        com[1] = x1; com[2] = x1;
      com[3] = 0x75; //Set Row Address
        com[4] = y0; com[5] = y1;

      if((*SclCnt) < 8){
        bbb = 0b10000000 >> (*SclCnt);
      }else if((*SclCnt) >= 8){
        bbb = 0b10000000 >> ((*SclCnt)-8);
      }

      if((*SclCnt) < 8){
        k = 0;
      }else{
        k = 1;
      }

      break;
    case 1:
      SSD1331_Copy(x0, y0, x1-1, y1, x0+1, y0);

      com[0] = 0x15; //Set Column Address
        com[1] = x0; com[2] = x0;
      com[3] = 0x75; //Set Row Address
        com[4] = y0; com[5] = y1;

      if((*SclCnt) < 8){
        bbb = 0b00000001 << (*SclCnt);
      }else if((*SclCnt) >= 8){
        bbb = 0b00000001 << ((*SclCnt) - 8);
      }

      if(ZorH == 2){
        if((*SclCnt) < 8){
          k = 1;
        }else{
          k = 0;
        }
      }else if(ZorH ==1){
        k = 0;
      }

      break;
  }

  CommandWriteBytes(com, 6);  

  uint8_t DotDot[16];

  for(i=0; i<16; i++){
    if((buf[k][i] & bbb) > 0){
      DotDot[i] = Dot;
    }else{
      DotDot[i] = 0;
    }
  }

  DataWriteBytes(DotDot, 16);
  (*SclCnt)++;
}
//************ 256色カラー 1ピクセル 描画 *****************
void ESP32_SSD1331::Drawing_Pixel_256color(uint8_t x0, uint8_t y0, uint8_t R, uint8_t G, uint8_t B){
  uint8_t com[6];
  //R (0-7), G (0-7), B (0-3)
  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0;

  CommandWriteBytes(com, 6);

  uint8_t Dot = (R << 5) | (G << 2) | B;

  DataWrite(Dot);
}
//************ 65000色カラー 1ピクセル 描画 *****************
void ESP32_SSD1331::Drawing_Pixel_65kColor(uint8_t x0, uint8_t y0, uint8_t R, uint8_t G, uint8_t B){
  uint8_t com[6];
  uint8_t DotDot[2];

  com[0] = 0x15; //Set Column Address
    com[1] = x0;
    com[2] = x0;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y0;

  CommandWriteBytes(com, 6);

  DotDot[0] = (R << 3) | (G >> 3);
  DotDot[1] = (G << 5) | B;

  DataWriteBytes(DotDot, 2);
}
//************ 65000色　線　描画******************************
void ESP32_SSD1331::Drawing_Line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B){
  uint8_t com[8];
  //Line_R (0-31), Line_G (0-63), Line_B (0-31)
  uint8_t R, B;
  R = Line_R <<1;
  B = Line_B <<1;

  com[0] = 0x21;
    com[1] = X0;
    com[2] = Y0;
    com[3] = X1;
    com[4] = Y1;
    com[5] = R;
    com[6] = Line_G;
    com[7] = B;

  CommandWriteBytes(com, 8);
}
//************ 65000色　四角　線描画***************************
void ESP32_SSD1331::Drawing_Rectangle_Line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B){
  uint8_t com[13];
  //Line_R (0-31), Line_G (0-63), Line_B (0-31)
  uint8_t R, B;
  R = Line_R <<1;
  B = Line_B <<1;

  com[0] = 0x26; //Fill Enable or Disable
    com[1] = 0b00000000; //A0=1 Fill Disable

  com[2] = 0x22; //Drawing Rectangle
    com[3] = X0; //Column Address of Start
    com[4] = Y0; //Row Address of Start
    com[5] = X1; //Column Address of End
    com[6] = Y1; //Row Address of End
    com[7] = R;
    com[8] = Line_G;
    com[9] = B;
    com[10] = 0;
    com[11] = 0;
    com[12] = 0;

  CommandWriteBytes(com, 13);
}
//************ 65000色　四角　塗りつぶし描画***************************
void ESP32_SSD1331::Drawing_Rectangle_Fill(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B, uint8_t Fill_R, uint8_t Fill_G, uint8_t Fill_B){
  uint8_t com[13];
  //Line_R (0-31), Line_G (0-63), Line_B (0-31)
  //Fill_R (0-31), Fill_G (0-63), Fill_B (0-31)
  uint8_t lineR, lineB, fillR, fillB;
  lineR = Line_R <<1;
  lineB = Line_B <<1;
  fillR = Fill_R <<1;
  fillB = Fill_B <<1;

  com[0] = 0x26; //Fill Enable or Disable
    com[1] = 0b00000001; //A0=0 Fill Enable

  com[2] = 0x22; //Drawing Rectangle
    com[3] = X0; //Column Address of Start
    com[4] = Y0; //Row Address of Start
    com[5] = X1; //Column Address of End
    com[6] = Y1; //Row Address of End
    com[7] = lineR;
    com[8] = Line_G;
    com[9] = lineB;
    com[10] = fillR;
    com[11] = Fill_G;
    com[12] = fillB;

  CommandWriteBytes(com, 13);
}
//************* 256色カラー　円 線描画***********************
void ESP32_SSD1331::Drawing_Circle_Line_256color(uint8_t x0, uint8_t y0, uint16_t r, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B){
  uint8_t com[6];
  //Line_R (0-7), Line_G (0-7), Line_B (0-3)
  uint8_t x1, y1;

  for(int i=0; i<360; i++){
    x1 = round((float)(x0 + (r * cos(radians(i)))));
    y1 = round((float)(y0 + (r * sin(radians(i)))));
    Drawing_Pixel_256color(x1, y1, Line_R, Line_G, Line_B);
  }
  delay(1);
  //描画範囲を元に戻して置く
  com[0] = 0x15; //Set Column Address
    com[1] = 0;
    com[2] = 95;
  com[3] = 0x75; //Set Row Address
    com[4] = 0;
    com[5] = 63;

  CommandWriteBytes(com, 6);
}
//************* 65000色カラー　円 線描画***********************
void ESP32_SSD1331::Drawing_Circle_Line_65kColor(uint8_t x0, uint8_t y0, uint16_t r, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B){
  uint8_t com[8];
  uint8_t x1, y1;

  CommandWrite(0xA0); //Remap & Color Depth setting　
    CommandWrite(0b01110010); //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format

  for(int i=0; i<360; i++){
    x1 = round((float)(x0 + (r * cos(radians(i)))));
    y1 = round((float)(y0 + (r * sin(radians(i)))));
    Drawing_Pixel_65kColor(x1, y1, Line_R, Line_G, Line_B);
  }
  delay(1);
  //描画範囲を元に戻して置く
  com[0] = 0xA0; //Remap & Color Depth setting　
    com[1] = 0b00110010; //A[7:6] = 00; 256 color. A[7:6] = 01; 65k color format

  com[2] = 0x15; //Set Column Address
    com[3] = 0;
    com[4] = 95;
  com[5] = 0x75; //Set Row Address
    com[6] = 0;
    com[7] = 63;

  CommandWriteBytes(com, 8);
}
//************* 65000色カラー　円塗りつぶし描画*****************
void ESP32_SSD1331::Drawing_Circle_Fill(uint8_t x0, uint8_t y0, uint16_t r, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B){
  //Line_R (0-31), Line_G (0-63), Line_B (0-31)
  uint8_t R, B;
  R = Line_R <<1;
  B = Line_B <<1;

  uint8_t x1, y1;

  for(int i=0; i<360; i++){
    x1 = round((float)(x0 + (r * cos(radians(i)))));
    y1 = round((float)(y0 + (r * sin(radians(i)))));
    Drawing_Line(x0, y1, x1, y1, R, Line_G, B);
  }
}
//*********電光掲示板風スクロール 8x16ドット********************
bool ESP32_SSD1331::Scroller_8x16_RtoL(uint8_t y0, uint8_t Zen_or_Han, uint8_t fnt_buf[16], uint8_t* scl_cnt1, uint8_t *ZorH_cnt, uint8_t col_R, uint8_t col_G, uint8_t col_B){

  Copy_Scroll(y0, fnt_buf, *scl_cnt1, col_R, col_G, col_B);

  (*scl_cnt1)++;

  if(*scl_cnt1 == 8){
    *scl_cnt1 = 0;
    if(Zen_or_Han == 2){
      if(*ZorH_cnt == 0){
        *ZorH_cnt = 1;
        return false;
      }else{
        *ZorH_cnt = 0;
        return true;
      }
    }else{
      *ZorH_cnt = 0;
      return true;
    }
  }

  return false;
}
//********************************************************
bool ESP32_SSD1331::Scroller_8x16_RtoL4line(uint8_t CS_pin, uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t *SclCnt, uint8_t *ZorHcnt, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  spi.setFrequency(7000000); //SSD1331 のSPI Clock Cycle Time 最低150ns
  spi.setDataMode(SPI_MODE3);
  //spi.setHwCs(bl);
  
  bool ret;

  digitalWrite(CS_pin, LOW);
  ret = ESP32_SSD1331::Scroller_8x16_RtoL4line(y0, num, Zen_or_Han, SclCnt, ZorHcnt, fnt_buf, col_R, col_G, col_B);
  digitalWrite(CS_pin, HIGH);

  return ret;
}

//********************************************************
bool ESP32_SSD1331::Scroller_8x16_RtoL4line(uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t *SclCnt, uint8_t *ZorHcnt, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  Copy_Scroll(y0, fnt_buf[*ZorHcnt], *SclCnt, col_R, col_G, col_B);

  *SclCnt = *SclCnt + 1;

  if(*SclCnt == 8){
    *SclCnt = 0;
    if(Zen_or_Han == 2){
      if(*ZorHcnt == 0){
        *ZorHcnt = 1;
        return false;
      }else{
        *ZorHcnt = 0;
        return true;
      }
    }else{
      *ZorHcnt = 0;
      return true;
    }
  }

  return false;
}
//*********電光掲示板風スクロール 8x16ドット********************
bool ESP32_SSD1331::Scroller_8x16_RtoL4line(uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  return ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(false, 1, 1, 0, 95, y0, num, Zen_or_Han, fnt_buf, col_R, col_G, col_B);
}
//*********電光掲示板風スクロール 8x16ドット サイズアップ********************
bool ESP32_SSD1331::SizeUp_Scroller_8x16_RtoL(uint8_t Size, uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  return ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(false, Size, Size, 0, 95, y0, num, Zen_or_Han, fnt_buf, col_R, col_G, col_B);
}
//*********電光掲示板風スクロール 8x16ドット 全角サイズアップ********************
bool ESP32_SSD1331::SizeUp_Scroller_8x16_RtoL(uint8_t Size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  return ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(false, Size, Size, x0, x1, y0, num, Zen_or_Han, fnt_buf, col_R, col_G, col_B);
}
//*********電光掲示板風スクロール 8x16ドット 縦、横サイズアップ********************
bool ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(uint8_t H_Size, uint8_t V_Size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  return ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(false, H_Size, V_Size, x0, x1, y0, num, Zen_or_Han, fnt_buf, col_R, col_G, col_B);
}
//*********電光掲示板風スクロール 8x16ドット 縦、横サイズアップ********************
bool ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(boolean *Scl_Reset, boolean Reverse, uint8_t H_Size, uint8_t V_Size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  if(*Scl_Reset){
    _scl_cnt[num] = 0;
    _ZorH_cnt[num] = 0;
    *Scl_Reset = false;
    return false;
  }
  return ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(Reverse, H_Size, V_Size, x0, x1, y0, num, Zen_or_Han, fnt_buf, col_R, col_G, col_B);

}
//*********電光掲示板風スクロール 8x16ドット 縦、横サイズアップ********************
bool ESP32_SSD1331::HVsizeUp_Scroller_8x16_RtoL(boolean Reverse, uint8_t H_Size, uint8_t V_Size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  for(int i=0; i<H_Size; i++){
    ESP32_SSD1331::SizeUp_Copy_Scroll(Reverse, V_Size, x0, x1, y0, fnt_buf[_ZorH_cnt[num]], _scl_cnt[num], col_R, col_G, col_B);
  }
  _scl_cnt[num]++;

  if(_scl_cnt[num] == 8){
    _scl_cnt[num] = 0;
    if(Zen_or_Han == 2){
      if(_ZorH_cnt[num] == 0){
        _ZorH_cnt[num] = 1;
        return false;
      }else{
        _ZorH_cnt[num] = 0;
        return true;
      }
    }else{
      _ZorH_cnt[num] = 0;
      return true;
    }
  }

  return false;
}
//***************************
void ESP32_SSD1331::Copy_Scroll(uint8_t y0, uint8_t buf[16], uint8_t scl_cnt2, uint8_t col_R, uint8_t col_G, uint8_t col_B){
  ESP32_SSD1331::SizeUp_Copy_Scroll(false, 1, 0, 95, y0, buf, scl_cnt2, col_R, col_G, col_B);
}
//***************************
void ESP32_SSD1331::SizeUp_Copy_Scroll(uint8_t Size, uint8_t y0, uint8_t buf[16], uint8_t scl_cnt2, uint8_t col_R, uint8_t col_G, uint8_t col_B){
  ESP32_SSD1331::SizeUp_Copy_Scroll(false, Size, 0, 95, y0, buf, scl_cnt2, col_R, col_G, col_B);
}

//***************************
void ESP32_SSD1331::SizeUp_Copy_Scroll(boolean Reverse, uint8_t Size, uint8_t x0, uint8_t x1, uint8_t y0, uint8_t buf[16], uint8_t scl_cnt2, uint8_t col_R, uint8_t col_G, uint8_t col_B){
  if(Size > 4) Size = 4;
  uint8_t Dot = (col_R << 5) | (col_G << 2) | col_B;

  uint8_t y1 = y0 + 16*Size - 1;
  SSD1331_Copy(x0+1, y0, x1, y1, x0, y0);

  uint8_t com[6];
  com[0] = 0x15; //Set Column Address
    com[1] = x1;
    com[2] = x1;
  com[3] = 0x75; //Set Row Address
    com[4] = y0;
    com[5] = y1;

  CommandWriteBytes(com, 6);

  uint8_t bbb = 0b10000000 >> scl_cnt2;
  
  uint8_t max_size = 16*Size;

  uint8_t DotDot[max_size] = {};

  uint8_t Dot_write_cnt = 0;
  uint8_t font_read_cnt = 0;
  uint8_t i;
  uint8_t f_buf = 0;
  
  for(font_read_cnt=0; font_read_cnt<16; font_read_cnt++){
    if(Reverse){
      f_buf = ~buf[font_read_cnt];
    }else{
      f_buf = buf[font_read_cnt];
    }
    if( f_buf & bbb ){
      for(i=0; i<Size; i++){
        DotDot[Dot_write_cnt++] = Dot;
      }
    }else{
      for(i=0; i<Size; i++){
        DotDot[Dot_write_cnt++] = 0;
      }
    }
  }

  DataWriteBytes(DotDot, max_size);
}

//********* 16x16ドット スクロール（全角限定）縦、横サイズアップ********************
bool ESP32_SSD1331::HVsizeUp_Vscroller_16x16(uint8_t H_Size, uint8_t V_Size, uint8_t Direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  return ESP32_SSD1331::HVsizeUp_Vscroller_16x16(false, H_Size, V_Size, Direction, x0, y0, y1, num, Zen_or_Han, fnt_buf, col_R, col_G, col_B);
}
//********* 16x16ドット スクロール（全角限定）縦、横サイズアップ********************
bool ESP32_SSD1331::HVsizeUp_Vscroller_16x16(boolean *Scl_Reset, boolean Reverse, uint8_t H_Size, uint8_t V_Size, uint8_t Direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  if(*Scl_Reset){
    _scl_cnt[num] = 0;
    *Scl_Reset = false;
    return false;
  }
  return ESP32_SSD1331::HVsizeUp_Vscroller_16x16(Reverse, H_Size, V_Size, Direction, x0, y0, y1, num, Zen_or_Han, fnt_buf, col_R, col_G, col_B);
}
//*********　16x16ドット スクロール（全角限定）縦、横サイズアップ、縦方向スクロール********************
bool ESP32_SSD1331::HVsizeUp_Vscroller_16x16(boolean Reverse, uint8_t H_Size, uint8_t V_Size, uint8_t Direction, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[][16], uint8_t col_R, uint8_t col_G, uint8_t col_B){
  for(int i=0; i<V_Size; i++){
    ESP32_SSD1331::SizeUp_Copy_V_Scroll(Reverse, H_Size, Direction, Zen_or_Han, fnt_buf, _scl_cnt[num], x0, y0, y1, col_R, col_G, col_B);
  }
  _scl_cnt[num]++;

  if(_scl_cnt[num] == 16){
    _scl_cnt[num] = 0;
    return true;
  }

  return false;
}
//*********** 垂直スクロール ****************
void ESP32_SSD1331::SizeUp_Copy_V_Scroll(boolean Reverse, uint8_t h_size, uint8_t Direction, uint8_t ZorH, uint8_t buf[][16], uint8_t SclCnt, uint8_t x0, uint8_t y0, uint8_t y1, uint8_t col_R, uint8_t col_G, uint8_t col_B){
  uint8_t Dot = (col_R << 5) | (col_G << 2) | col_B;
  uint8_t i, j, k;
  uint8_t bbb = 0b10000000;
  uint8_t DotDot[8 * ZorH * h_size] = {};
  uint8_t com[6];
  uint8_t x1 = x0 + 8 * ZorH * h_size - 1;

  switch( Direction ){
    case 0:
      SSD1331_Copy(x0, y0+1, x1, y1, x0, y0);

      com[0] = 0x15; //Set Column Address
        com[1] = x0; com[2] = x1;
      com[3] = 0x75; //Set Row Address
        com[4] = y1; com[5] = y1;
      break;
    case 1:
      SSD1331_Copy(x0, y0, x1, y1-1, x0, y0+1);

      com[0] = 0x15; //Set Column Address
        com[1] = x0; com[2] = x1;
      com[3] = 0x75; //Set Row Address
        com[4] = y0; com[5] = y0;
      break;
  }  

  CommandWriteBytes(com, 6);
  
  uint8_t Dot_write_cnt = 0;
  uint8_t f_buf = 0;

  switch( Direction ){
    case 0:
      for(k=0; k<ZorH; k++){
        if(Reverse){
          f_buf = ~buf[k][SclCnt];
        }else{
          f_buf = buf[k][SclCnt];
        }
        for(i=0; i<8; i++){
          if( f_buf & (bbb >> i) ){
            for(j=0; j<h_size; j++){
              DotDot[Dot_write_cnt++] = Dot;
            }
          }else{
            for(j=0; j<h_size; j++){
              DotDot[Dot_write_cnt++] = 0;
            }
          }
        }
      }
      break;
    case 1:
      for(k=0; k<ZorH; k++){
        if(Reverse){
          f_buf = ~buf[k][15-SclCnt];
        }else{
          f_buf = buf[k][15-SclCnt];
        }
        for(i=0; i<8; i++){
          if( f_buf & (bbb >> i) ){
            for(j=0; j<h_size; j++){
              DotDot[Dot_write_cnt++] = Dot;
            }
          }else{
            for(j=0; j<h_size; j++){
              DotDot[Dot_write_cnt++] = 0;
            }
          }
        }
      }
      break;
  }

  DataWriteBytes(DotDot, sizeof(DotDot));
}