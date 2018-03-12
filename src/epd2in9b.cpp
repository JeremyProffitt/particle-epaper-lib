/**
 *  @filename   :   epd2in9b.cpp
 *  @brief      :   Implements for Dual-color e-paper library
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

#include <stdlib.h>
#include <epd2in9b.h>

bool Epd::Init(void)
{
/* this calls the peripheral hardware interface, see epdif */
  if (!IfInit())
  {
    return false;
  }
  /* EPD hardware init start */
  Reset();
  SendCommand(BOOSTER_SOFT_START);
  SendData(0x17, 3);
  SendCommand(POWER_ON);
  while(isBusy()) yield();
  SendCommand(PANEL_SETTING);
  //SendData(0x8F);
  SendData(0x83 | _orientation);
  SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
  SendData(0x77);
  SendCommand(TCON_RESOLUTION);
  SendData(0x80);
  SendData(0x01);
  SendData(0x28);
  //SendData((uint8_t)(_width & 0x00FF));
  //SendData((uint8_t)(_height >> 8));
  //SendData((uint8_t)(_height & 0x00FF) );
  SendCommand(VCM_DC_SETTING_REGISTER);
  SendData(0X0A);
  /* EPD hardware init end */
  return true;
}

bool Epd::Init(SCREEN_ORIENTATION Orientation) {
  _orientation = Orientation;
  return Init();
}

/**
 *  @brief: basic function for sending commands
 */
void Epd::SendCommand(unsigned char command)
{
  DigitalWrite(_DC, LOW);
  SpiTransfer(command, 1);
}

/**
*  @brief: basic function for sending data
*/
void Epd::SendData(unsigned char data, int16_t len) {
  DigitalWrite(_DC, HIGH);
  SpiTransfer(data, len);
}
void Epd::SendData(const unsigned char *data, int16_t len) {
  DigitalWrite(_DC, HIGH);
  SpiTransfer(data, len);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd::WaitUntilIdle(void)
{
  while (isBusy()) DelayMs(100);
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd::Sleep();
 */
void Epd::Reset(void)
{
  DigitalWrite(_RST, LOW);
  DelayMs(200);
  DigitalWrite(_RST, HIGH);
  DelayMs(200);
}

/**
 *  @brief: transmit partial data to the SRAM
 */
bool Epd::SetPartialWindow(const unsigned char* buffer_black, const unsigned char* buffer_red, int16_t x, int16_t y, int16_t w, int16_t h, bool transmitBlack, bool transmitRed) 
{
  if (isBusy()) return false;

  SendCommand(PARTIAL_IN);
  SendCommand(PARTIAL_WINDOW);
  unsigned char dims[] =
  { (x & 0xf8)    // x should be the multiple of 8, the last 3 bit will always be ignored
  , (((x & 0xf8) + w - 1) | 0x07)
  , (y >> 8)
  , (y & 0xff)
  , ((y + h - 1) >> 8)
  , ((y + h - 1) & 0xff)
  , (0x01)         // Gates scan both inside and outside of the partial window. (default) 
  };
  SendData(dims, sizeof(dims));
  DelayMs(2);

  if (transmitBlack) {
    SendCommand(DATA_START_TRANSMISSION_1);
    if (buffer_black != NULL) {
      SendData(buffer_black, w / 8 * h);
    }
    else {
      SendData((uint8_t)0x00, w / 8 * h);
    }
    DelayMs(2);
  }

  if (transmitRed) {
    SendCommand(DATA_START_TRANSMISSION_2);
    if (buffer_red != NULL) {
      SendData(buffer_red, w / 8 * h);
    }
    else {
      SendData((unsigned char)0x00, w / 8 * h);
    }
    DelayMs(2);
  }
  SendCommand(PARTIAL_OUT);
  
  return true;
}

/**
 *  @brief: transmit partial data to the black part of SRAM
 */
bool Epd::SetPartialWindowBlack(const unsigned char* buffer_black, int16_t x, int16_t y, int16_t w, int16_t h)   
{
  return SetPartialWindow(buffer_black, NULL, x, y, w, h, true, false);
}
//{
//  if (isBusy()) return false;
//
//  SendCommand(PARTIAL_IN);
//  SendCommand(PARTIAL_WINDOW);
//  unsigned char dims[] =
//  { (x & 0xf8)    // x should be the multiple of 8, the last 3 bit will always be ignored
//  , (((x & 0xf8) + w - 1) | 0x07)
//  , (y >> 8)
//  , (y & 0xff)
//  , ((y + h - 1) >> 8)
//  , ((y + h - 1) & 0xff)
//  , (0x01)         // Gates scan both inside and outside of the partial window. (default) 
//  };
//  SendData(dims, sizeof(dims));
//  DelayMs(2);
//  SendCommand(DATA_START_TRANSMISSION_1);
//  if (buffer_black != NULL)
//  {
//    SendData(buffer_black, w / 8 * h);
//  }
//  else
//  {
//    SendData((unsigned char)0x00, w / 8 * h);
//  }
//  DelayMs(2);
//  SendCommand(PARTIAL_OUT);
//}
//

/**
 *  @brief: transmit partial data to the red part of SRAM
 */
bool Epd::SetPartialWindowRed(const unsigned char* buffer_red, int16_t x, int16_t y, int16_t w, int16_t h)
{
  return SetPartialWindow(NULL, buffer_red, x, y, w, h, false, true);
}
//{
//  if (isBusy()) return false;
//
//  SendCommand(PARTIAL_IN);
//  SendCommand(PARTIAL_WINDOW);
//  unsigned char dims[] =
//  { (x & 0xf8)    // x should be the multiple of 8, the last 3 bit will always be ignored
//  , (((x & 0xf8) + w - 1) | 0x07)
//  , (y >> 8)
//  , (y & 0xff)
//  , ((y + h - 1) >> 8)
//  , ((y + h - 1) & 0xff)
//  , (0x01)         // Gates scan both inside and outside of the partial window. (default) 
//  };
//  SendData(dims, sizeof(dims));
//  DelayMs(2);
//  SendCommand(DATA_START_TRANSMISSION_2);
//  if (buffer_red != NULL)
//  {
//    SendData(buffer_red, w / 8 * h);
//  }
//  else
//  {
//    SendData((unsigned char)0x00, w / 8 * h);
//  }
//  DelayMs(2);
//  SendCommand(PARTIAL_OUT);
//
//  return true;
//}

/**
 * @brief: refresh and displays the frame
 */
bool Epd::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red)
{
  if (isBusy()) return false;

  if (frame_buffer_black != NULL)
  {
    SendCommand(DATA_START_TRANSMISSION_1);
    DelayMs(2);
    SendData(frame_buffer_black, this->_width * this->_height / 8);
    DelayMs(2);
  }
  if (frame_buffer_red != NULL)
  {
    SendCommand(DATA_START_TRANSMISSION_2);
    DelayMs(2);
    SendData(frame_buffer_red, this->_width * this->_height / 8);
    DelayMs(2);
  }
  SendCommand(DISPLAY_REFRESH);
  //WaitUntilIdle();
  
  return true;
}

/**
 * @brief: clear the frame data from the SRAM, this won't refresh the display
 */
bool Epd::ClearFrame(void)
{
  if (isBusy()) return false;

  SendCommand(TCON_RESOLUTION);
  unsigned char dims[] =
  { (_width >>8)
  , (_width & 0xff)
  , (_height >> 8)
  , (_height & 0xff)
  };
  SendData(dims, sizeof(dims));

  SendCommand(DATA_START_TRANSMISSION_1);
  DelayMs(2);
  SendData((unsigned char)0xFF, _width * _height / 8);
  DelayMs(2);
  SendCommand(DATA_START_TRANSMISSION_2);
  DelayMs(2);
  SendData((unsigned char)0xFF, _width * _height / 8);
  DelayMs(2);

  return true;
}

/**
 * @brief: This displays the frame data from SRAM
 */
bool Epd::DisplayFrame(void)
{
  if (isBusy()) return false;

  SendCommand(DISPLAY_REFRESH);
  //WaitUntilIdle();

  return true;
}

/**
 * @brief: After this command is transmitted, the chip would enter the deep-sleep mode to save power.
 *         The deep sleep mode would return to standby by hardware reset. The only one parameter is a
 *         check code, the command would be executed if check code = 0xA5.
 *         You can use Epd::Reset() to awaken and use Epd::Init() to initialize.
 */
void Epd::Sleep()
{
  SendCommand(DEEP_SLEEP);
  SendData(0xa5);
}


/* END OF FILE */
