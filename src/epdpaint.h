/**
 *  @filename   :   epdpaint.h
 *  @brief      :   Header file for epdpaint.cpp
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     July 28 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef EPDPAINT_H
#define EPDPAINT_H

#include <Arduino.h>

// Display orientation (as set in epd2in9b.h for PANEL_SETTINGs)
enum ORIENTATION {
  PORTRAIT          = 0b1100,
  LANDSCAPE         = 0b0100,
  PORTRAIT_FLIPPED  = 0b1000,
  LANDSCAPE_FLIPPED = 0b0000,
};

#include "fonts.h"

class Paint {
public:
  Paint(unsigned char* image, int16_t screenWidth, int16_t screenHeight, ORIENTATION orient = PORTRAIT, bool inverted = false);
  ~Paint();
  void Clear(int16_t colored);
  int16_t  GetWidth(void);
  //void SetWidth(int16_t width);
  int16_t  GetHeight(void);
  //void SetHeight(int16_t height);
  ORIENTATION GetRotate(void);
  void SetRotate(ORIENTATION orientation);
  bool isInverse(void);
  bool isInverse(bool invert);
  unsigned char* GetImage(void);
  void TransformXY(int16_t* x, int16_t* y);
  void DrawAbsolutePixel(int16_t x, int16_t y, int16_t colored);
  void DrawPixel(int16_t x, int16_t y, int16_t colored);
  void DrawCharAt(int16_t x, int16_t y, char ascii_char, const sFONT* font, int16_t colored);
  void DrawStringAt(int16_t x, int16_t y, const char* text, const sFONT* font, int16_t colored);
  void DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t colored);
  void DrawHorizontalLine(int16_t x, int16_t y, int16_t width, int16_t colored);
  void DrawVerticalLine(int16_t x, int16_t y, int16_t height, int16_t colored);
  void DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t colored);
  void DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t colored);
  void DrawCircle(int16_t x, int16_t y, int16_t radius, int16_t colored);
  void DrawFilledCircle(int16_t x, int16_t y, int16_t radius, int16_t colored);
 
private:
  unsigned char* image;
  int16_t        width;
  int16_t        height;
  ORIENTATION    orientation;
  bool           inverse;
};

#endif

/* END OF FILE */

