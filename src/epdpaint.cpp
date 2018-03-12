/**
 *  @filename   :   epdpaint.cpp
 *  @brief      :   Paint tools
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare     September 9 2017
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

#include <avr/pgmspace.h>
#include "epdpaint.h"

Paint::Paint(unsigned char* image, int16_t screenWidth, int16_t screenHeight, ORIENTATION orientation, bool inverted) {
  this->image = image;
  /* 1 byte = 8 pixels, so the width should be the multiple of 8 */
  this->width = screenWidth % 8 ? screenWidth + 8 - (screenWidth % 8) : screenWidth;
  this->height = screenHeight;
  this->orientation = orientation;
  this->inverse = inverted;

  SetRotate(orientation);
}

Paint::~Paint() {}

/**
 *  @brief: clear the image
 */
void Paint::Clear(int16_t colored) {
  memset(this->image, ((bool)colored != this->inverse) ? 0xFF : 0x00, this->height * this->width / 8);
}

/**
 *  @brief: this draws a pixel by absolute coordinates.
 *          this function won't be affected by the rotate parameter.
 */
void Paint::DrawAbsolutePixel(int16_t x, int16_t y, int16_t colored) {
  if (x < 0 || x >= this->width || y < 0 || y >= this->height)
    return;

  if ((bool)colored != this->inverse)
    image[(x + y * this->width) / 8] |= 0x80 >> (x % 8);
  else
    image[(x + y * this->width) / 8] &= ~(0x80 >> (x % 8));
}

/**
 *  @brief: Getters and Setters
 */
unsigned char* Paint::GetImage(void) {
  return this->image;
}

int16_t Paint::GetWidth(void) {
  switch (this->orientation) {
    case PORTRAIT:
    case PORTRAIT_FLIPPED:
      return (width < height) ? width : height;
      break;
    case LANDSCAPE:
    case LANDSCAPE_FLIPPED:
      return (width > height) ? width : height;
      break;
    default:
      return this->width;
      break;
  }
}

//void Paint::SetWidth(int16_t width) {
//  this->width = width % 8 ? width + 8 - (width % 8) : width;
//}

int16_t Paint::GetHeight(void) {
  switch (this->orientation) {
    case PORTRAIT:
    case PORTRAIT_FLIPPED:
      return (height > width) ? height : width;
      break;
    case LANDSCAPE:
    case LANDSCAPE_FLIPPED:
      return (height < width) ? height : width;
      break;
    default:
      return this->height;
      break;
  }
}

//void Paint::SetHeight(int16_t height) {
//  this->height = height;
//}

ORIENTATION Paint::GetRotate(void) {
  return this->orientation;
}

void Paint::SetRotate(ORIENTATION orientation) {
  this->orientation = orientation;
}

bool Paint::isInverse(void) {
  return inverse;
}

bool Paint::isInverse(bool invert) {
  bool previous = inverse;
  inverse = invert;
  return previous;
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void Paint::TransformXY(int16_t* x, int16_t* y) {
  int16_t dmy;

  switch (this->orientation) {
    default:
    case PORTRAIT:
      break;
    case LANDSCAPE:
      dmy = *x;
      *x = this->width - *y;
      *y = dmy;
      break;
    case PORTRAIT_FLIPPED:
      *x = this->width - *x;
      *y = this->height - *y;
      break;
    case LANDSCAPE_FLIPPED:
      dmy = *x;
      *x = *y;
      *y = this->height - *x;
      break;
  }
}

/**
 *  @brief: this draws a pixel by the coordinates
 */
void Paint::DrawPixel(int16_t x, int16_t y, int16_t colored) {
  TransformXY(&x, &y);
  if (x < 0 || x >= this->width || y < 0 || y >= this->height)
    return;
  DrawAbsolutePixel(x, y, colored);
}

/**
 *  @brief: this draws a charactor on the frame buffer but not refresh
 */
void Paint::DrawCharAt(int16_t x, int16_t y, char ascii_char, const sFONT* font, int16_t colored) {
  int16_t i;
  int16_t j;

  uint16_t char_offset = (ascii_char - ' ') * font->Height * (font->Width / 8 + (font->Width % 8 ? 1 : 0));
  const unsigned char* ptr = &font->table[char_offset];

  for (j = 0; j < font->Height; j++) {
    for (i = 0; i < font->Width; i++) {
      if (pgm_read_byte(ptr) & (0x80 >> (i % 8))) {
        DrawPixel(x + i, y + j, colored);
      }
      if (i % 8 == 7) {
        ptr++;
      }
    }
    if (font->Width % 8 != 0) {
      ptr++;
    }
  }
}

/**
*  @brief: this displays a string on the frame buffer but not refresh
*/
void Paint::DrawStringAt(int16_t x, int16_t y, const char* text, const sFONT* font, int16_t colored) {
  const char* p_text = text;
  uint16_t counter = 0;
  int16_t refcolumn = x;

  /* Send the string character by character on EPD */
  while (*p_text != 0) {
/* Display one character on EPD */
    DrawCharAt(refcolumn, y, *p_text, font, colored);
    /* Decrement the column position by 16 */
    refcolumn += font->Width;
    /* Point on the next character */
    p_text++;
    counter++;
  }
}

/**
*  @brief: this draws a line on the frame buffer
*/
void Paint::DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t colored) {
/* Bresenham algorithm */
  TransformXY(&x0, &y0);
  TransformXY(&x1, &y1);
  int16_t dx = x1 - x0 >= 0 ? x1 - x0 : x0 - x1;
  int16_t sx = x0 < x1 ? 1 : -1;
  int16_t dy = y1 - y0 <= 0 ? y1 - y0 : y0 - y1;
  int16_t sy = y0 < y1 ? 1 : -1;
  int16_t err = dx + dy;

  while ((x0 != x1) || (y0 != y1)) {
    DrawAbsolutePixel(x0, y0, colored);
    if (2 * err >= dy && x0 != x1) {
      err += dy;
      x0 += sx;
    }
    if (2 * err <= dx && y0 != y1) {
      err += dx;
      y0 += sy;
    }
  }
}

/**
*  @brief: this draws a horizontal line on the frame buffer
*/
void Paint::DrawHorizontalLine(int16_t x, int16_t y, int16_t line_width, int16_t colored) {
  int16_t i;
  for (i = x; i < x + line_width; i++) {
    DrawPixel(i, y, colored);
  }
}

/**
*  @brief: this draws a vertical line on the frame buffer
*/
void Paint::DrawVerticalLine(int16_t x, int16_t y, int16_t line_height, int16_t colored) {
  int16_t i;
  for (i = y; i < y + line_height; i++) {
    DrawPixel(x, i, colored);
  }
}

/**
*  @brief: this draws a rectangle
*/
void Paint::DrawRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t colored) {
  int16_t min_x, min_y, max_x, max_y;
  min_x = x1 > x0 ? x0 : x1;
  max_x = x1 > x0 ? x1 : x0;
  min_y = y1 > y0 ? y0 : y1;
  max_y = y1 > y0 ? y1 : y0;

  DrawHorizontalLine(min_x, min_y, max_x - min_x + 1, colored);
  DrawHorizontalLine(min_x, max_y, max_x - min_x + 1, colored);
  DrawVerticalLine(min_x, min_y, max_y - min_y + 1, colored);
  DrawVerticalLine(max_x, min_y, max_y - min_y + 1, colored);
}

/**
*  @brief: this draws a filled rectangle
*/
void Paint::DrawFilledRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t colored) {
  int16_t min_x, min_y, max_x, max_y;
  int16_t i;
  min_x = x1 > x0 ? x0 : x1;
  max_x = x1 > x0 ? x1 : x0;
  min_y = y1 > y0 ? y0 : y1;
  max_y = y1 > y0 ? y1 : y0;

  for (i = min_x; i <= max_x; i++) {
    DrawVerticalLine(i, min_y, max_y - min_y + 1, colored);
  }
}

/**
*  @brief: this draws a circle
*/
void Paint::DrawCircle(int16_t x, int16_t y, int16_t radius, int16_t colored) {
/* Bresenham algorithm */
  int16_t x_pos = -radius;
  int16_t y_pos = 0;
  int16_t err = 2 - 2 * radius;
  int16_t e2;

  do {
    DrawPixel(x - x_pos, y + y_pos, colored);
    DrawPixel(x + x_pos, y + y_pos, colored);
    DrawPixel(x + x_pos, y - y_pos, colored);
    DrawPixel(x - x_pos, y - y_pos, colored);
    e2 = err;
    if (e2 <= y_pos) {
      err += ++y_pos * 2 + 1;
      if (-x_pos == y_pos && e2 <= x_pos) {
        e2 = 0;
      }
    }
    if (e2 > x_pos) {
      err += ++x_pos * 2 + 1;
    }
  } while (x_pos <= 0);
}

/**
*  @brief: this draws a filled circle
*/
void Paint::DrawFilledCircle(int16_t x, int16_t y, int16_t radius, int16_t colored) {
/* Bresenham algorithm */
  int16_t x_pos = -radius;
  int16_t y_pos = 0;
  int16_t err = 2 - 2 * radius;
  int16_t e2;

  do {
    DrawPixel(x - x_pos, y + y_pos, colored);
    DrawPixel(x + x_pos, y + y_pos, colored);
    DrawPixel(x + x_pos, y - y_pos, colored);
    DrawPixel(x - x_pos, y - y_pos, colored);
    DrawHorizontalLine(x + x_pos, y + y_pos, 2 * (-x_pos) + 1, colored);
    DrawHorizontalLine(x + x_pos, y - y_pos, 2 * (-x_pos) + 1, colored);
    e2 = err;
    if (e2 <= y_pos) {
      err += ++y_pos * 2 + 1;
      if (-x_pos == y_pos && e2 <= x_pos) {
        e2 = 0;
      }
    }
    if (e2 > x_pos) {
      err += ++x_pos * 2 + 1;
    }
  } while (x_pos <= 0);
}

/* END OF FILE */