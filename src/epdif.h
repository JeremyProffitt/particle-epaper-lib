/**
 *  @filename   :   epdif.h
 *  @brief      :   Header file of epdif.cpp providing EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *  @enhancement:	For particle.io by ScruffR (September 23 2017)
 *
 *  Copyright (C) Waveshare     August 10 2017
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

#ifndef EPDIF_H
#define EPDIF_H

#include <Arduino.h>
#include <SPI.h>

// Pin definition
//#define RST_PIN         8
//#define DC_PIN          9
//#define CS_PIN          10
//#define BUSY_PIN        7

#if defined(PARTICLE)
#define yield() Particle.process()
#else
#define yield() ;
#endif

class EpdIf {
public:
  EpdIf(void)
    : _SPI(SPI), _CS(10), _DC(9), _RST(8), _BUSY(7), _init(false)
  {}
  EpdIf(SPIClass& hwSPI, int16_t pinCS, int16_t pinDC, int16_t pinReset, int16_t pinBusy)
    : _SPI(hwSPI), _CS(pinCS), _DC(pinDC), _RST(pinReset), _BUSY(pinBusy), _init(false)
  {}

  ~EpdIf(void)
  {}

  bool    IfInit(void);
  int16_t DigitalRead(int16_t pin);
  void    DigitalWrite(int16_t pin, int16_t value);
  void    DelayMs(uint16_t delaytime);
  void    SpiTransfer(unsigned char data, int16_t len = 1);
  void    SpiTransfer(const unsigned char *data, int16_t len);

protected:
  SPIClass& _SPI;
  int16_t   _CS;
  int16_t   _DC;
  int16_t   _RST;
  int16_t   _BUSY;
  bool      _init;
};

#endif
