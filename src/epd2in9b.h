/**
 *  @filename   :   epd2in9b.h
 *  @brief      :   Header file for Dual-color e-paper library epd2in9b.cpp
 *  @author     :   Yehui from Waveshare
 *  @enhancement:	  For particle.io by ScruffR (September 23 2017)
 *
 *  Copyright (C) Waveshare     July 31 2017
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

#ifndef EPD2IN9B_H
#define EPD2IN9B_H

#include "epdif.h"

// Display resolution
#define EPD_WIDTH       128
#define EPD_HEIGHT      296

// EPD2IN9B commands
enum COMMANDS {
  PANEL_SETTING                  = 0x00,
  POWER_SETTING                  = 0x01,
  POWER_OFF                      = 0x02,
  POWER_OFF_SEQUENCE_SETTING     = 0x03,
  POWER_ON                       = 0x04,
  POWER_ON_MEASURE               = 0x05,
  BOOSTER_SOFT_START             = 0x06,
  DEEP_SLEEP                     = 0x07,
  DATA_START_TRANSMISSION_1      = 0x10,
  DATA_STOP                      = 0x11,
  DISPLAY_REFRESH                = 0x12,
  DATA_START_TRANSMISSION_2      = 0x13,
  PLL_CONTROL                    = 0x30,
  TEMPERATURE_SENSOR_COMMAND     = 0x40,
  TEMPERATURE_SENSOR_CALIBRATION = 0x41,
  TEMPERATURE_SENSOR_WRITE       = 0x42,
  TEMPERATURE_SENSOR_READ        = 0x43,
  VCOM_AND_DATA_INTERVAL_SETTING = 0x50,
  LOW_POWER_DETECTION            = 0x51,
  TCON_SETTING                   = 0x60,
  TCON_RESOLUTION                = 0x61,
  GET_STATUS                     = 0x71,
  AUTO_MEASURE_VCOM              = 0x80,
  VCOM_VALUE                     = 0x81,
  VCM_DC_SETTING_REGISTER        = 0x82,
  PARTIAL_WINDOW                 = 0x90,
  PARTIAL_IN                     = 0x91,
  PARTIAL_OUT                    = 0x92,
  PROGRAM_MODE                   = 0xA0,
  ACTIVE_PROGRAM                 = 0xA1,
  READ_OTP_DATA                  = 0xA2,
  POWER_SAVING                   = 0xE3,
};

// needs testing
enum SCREEN_ORIENTATION {
  NORMAL         = 0b0000, 
  NORMAL_FLIPPED = 0b1100,
  MIRROR         = 0b0100,
  MIRROR_FLIPPED = 0b1000,
};


class Epd : EpdIf {
public:

  Epd()
    : EpdIf(), _width(EPD_WIDTH), _height(EPD_HEIGHT), _orientation(NORMAL)
  {}
  Epd(SPIClass& hwSPI, int16_t pinCS, int16_t pinDC, int16_t pinReset, int16_t pinBusy, uint16_t Width = EPD_WIDTH, uint16_t Height = EPD_HEIGHT, SCREEN_ORIENTATION Orientation = NORMAL)
    
    : EpdIf(hwSPI, pinCS, pinDC, pinReset, pinBusy), _width(Width), _height(Height), _orientation(Orientation)
  {}

  ~Epd()
  {}

  bool Init(void);
  bool Init(SCREEN_ORIENTATION Orientation);
  void SendCommand(unsigned char command);
  void SendData(unsigned char data, int16_t len = 1);
  void SendData(const unsigned char *data, int16_t len);
  void WaitUntilIdle(void);
  void Reset(void);
  bool SetPartialWindow(const unsigned char* buffer_black, const unsigned char* buffer_red, int16_t x, int16_t y, int16_t w, int16_t h, bool transmitBlack = true, bool transmitRed = true);
  bool SetPartialWindowBlack(const unsigned char* buffer_black, int16_t x, int16_t y, int16_t w, int16_t h);
  bool SetPartialWindowRed(const unsigned char* buffer_red, int16_t x, int16_t y, int16_t w, int16_t h);
  bool DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red);
  bool DisplayFrame(void);
  bool ClearFrame(void);
  void Sleep(void);

  inline uint16_t GetWidth()   { return _width; }
  inline uint16_t GetHeight()  { return _height; }
  inline bool     isBusy(void) { return !DigitalRead(_BUSY); } //LOW: busy, HIGH: idle

private:
  uint16_t           _width;
  uint16_t           _height;
  SCREEN_ORIENTATION _orientation;
};

#endif /* EPD2IN9B_H */

/* END OF FILE */
