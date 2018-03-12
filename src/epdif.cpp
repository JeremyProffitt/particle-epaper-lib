/**
 *  @filename   :   epdif.cpp
 *  @brief      :   Implements EPD interface functions
 *                  Users have to implement all the functions in epdif.cpp
 *  @author     :   Yehui from Waveshare
 *  @enhancement:	  For particle.io by ScruffR (September 23 2017)
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

#include "epdif.h"

bool EpdIf::IfInit(void) {
  pinMode(_CS, OUTPUT);
  pinMode(_RST, OUTPUT);
  pinMode(_DC, OUTPUT);
  pinMode(_BUSY, INPUT);
  _SPI.beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  _SPI.begin();
  return (_init = true);
}

int16_t EpdIf::DigitalRead(int16_t pin) 
{
#if defined(PARTICLE) && (SYSTEM_VERSION >= 0x00040400)  
  if (!_init) IfInit();
  return pinReadFast(pin);
#else
  return digitalRead(pin);
#endif
}

void EpdIf::DigitalWrite(int16_t pin, int16_t value)
{
#if defined(PARTICLE) && (SYSTEM_VERSION >= 0x00040400)
  if (!_init) IfInit();
  digitalWriteFast(pin, value);
#else
  digitalWrite(pin, value);
#endif
}

void EpdIf::DelayMs(uint16_t delaytime)
{
#if defined(PARTICLE)
  for (uint32_t _ms = millis(); millis() - _ms < delaytime; Particle.process());
#else
  delay(delaytime);
#endif
}

void EpdIf::SpiTransfer(unsigned char data, int16_t len)
{
  digitalWrite(_CS, LOW);
  for(int16_t i=0; i < len; i++)
    _SPI.transfer(data);
  digitalWrite(_CS, HIGH);
}

void EpdIf::SpiTransfer(const unsigned char *data, int16_t len) {
  digitalWrite(_CS, LOW);
  _SPI.transfer((void*)data, NULL, len, NULL);
  digitalWrite(_CS, HIGH);
}

