/*
  ESP32_SSD1331.h - for Arduino core for the ESP32 ( Use SPI library ).
  Beta version 1.4
  
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

#ifndef _ESP32_SSD1331_H_INCLUDED
#define _ESP32_SSD1331_H_INCLUDED

#include <Arduino.h>
#include <SPI.h>

class ESP32_SSD1331
{
private:
  uint8_t _sck;
  uint8_t _miso;
  uint8_t _mosi;
  uint8_t _cs;
  uint8_t _dc;
  uint8_t _rst;
  uint8_t _scl_cnt[4] = {};
  uint8_t _ZorH_cnt[4] = {};

public:
  ESP32_SSD1331(uint8_t sck, uint8_t miso, uint8_t mosi, uint8_t cs, uint8_t dc, uint8_t rst);

  void SSD1331_Init();
  void CommandWrite(uint8_t b);
  void DataWrite(uint8_t b);
  void CommandWriteBytes(uint8_t *b, uint16_t n);
  void DataWriteBytes(uint8_t *b, uint16_t n);
  void Brightness(uint8_t brightness);
  void Brightness_FadeIn(uint8_t interval);
  void Brightness_FadeOut(uint8_t interval);
  void Display_Clear(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);
  void SSD1331_Copy(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t X, uint8_t Y);
  void SSD1331_8x16_Font_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t Fnt[][16]);
  void SSD1331_8x8_Font_DisplayOut(uint8_t txtMax, uint8_t x0, uint8_t y0, uint8_t red, uint8_t green, uint8_t blue, uint8_t Fnt[][8]);
  void Time_Copy_V_Scroll(uint8_t Direction, uint8_t ZorH, uint8_t buf[2][16], uint8_t *SclCnt, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t col_R, uint8_t col_G, uint8_t col_B);
  void Time_Copy_H_Scroll(uint8_t Direction, uint8_t ZorH, uint8_t buf[2][16], uint8_t *SclCnt, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t col_R, uint8_t col_G, uint8_t col_B);
  void Drawing_Pixel_256color(uint8_t x0, uint8_t y0, uint8_t R, uint8_t G, uint8_t B);
  void Drawing_Pixel_65kColor(uint8_t x0, uint8_t y0, uint8_t R, uint8_t G, uint8_t B);
  void Drawing_Line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B);
  void Drawing_Rectangle_Line(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B);
  void Drawing_Rectangle_Fill(uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B, uint8_t Fill_R, uint8_t Fill_G, uint8_t Fill_B);
  void Drawing_Circle_Line_256color(uint8_t x0, uint8_t y0, uint16_t r, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B);
  void Drawing_Circle_Line_65kColor(uint8_t x0, uint8_t y0, uint16_t r, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B);
  void Drawing_Circle_Fill(uint8_t x0, uint8_t y0, uint16_t r, uint8_t Line_R, uint8_t Line_G, uint8_t Line_B);
  bool Scroller_8x16_RtoL(uint8_t y0, uint8_t Zen_or_Han, uint8_t fnt_buf[16], uint8_t *scl_cnt1, uint8_t *ZorH_cnt, uint8_t col_R, uint8_t col_G, uint8_t col_B);
  bool Scroller_8x16_RtoL4line(uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t fnt_buf[2][16], uint8_t col_R, uint8_t col_G, uint8_t col_B);
  bool Scroller_8x16_RtoL4line(uint8_t y0, uint8_t num, uint8_t Zen_or_Han, uint8_t *SclCnt, uint8_t *ZorHcnt, uint8_t fnt_buf[2][16], uint8_t col_R, uint8_t col_G, uint8_t col_B);

  void Copy_Scroll(uint8_t y0, uint8_t buf[16], uint8_t scl_cnt2, uint8_t col_R, uint8_t col_G, uint8_t col_B);

};

#endif
