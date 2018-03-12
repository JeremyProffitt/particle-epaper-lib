/**
 *  @filename   :   epd2in13b.h
 *  @brief      :   Header file for e-paper library epd2in13b.c
 *  @author     :   Yehui from Waveshare
 *  
 *  Copyright (C) Waveshare     August 22 2017
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

#ifndef EPD2IN13B_H
#define EPD2IN13B_H

#include "fonts.h"

// Display resolution
#define EPD_WIDTH       104
#define EPD_HEIGHT      212

// EPD2IN13B commands
#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define VCOM_LUT                                    0x20
#define W2W_LUT                                     0x21
#define B2W_LUT                                     0x22
#define W2B_LUT                                     0x23
#define B2B_LUT                                     0x24
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_SENSOR_CALIBRATION              0x40
#define TEMPERATURE_SENSOR_SELECTION                0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define RESOLUTION_SETTING                          0x61
#define GET_STATUS                                  0x71
#define AUTO_MEASURE_VCOM                           0x80
#define READ_VCOM_VALUE                             0x81
#define VCM_DC_SETTING                              0x82
#define PARTIAL_WINDOW                              0x90
#define PARTIAL_IN                                  0x91
#define PARTIAL_OUT                                 0x92
#define PROGRAM_MODE                                0xA0
#define ACTIVE_PROGRAM                              0xA1
#define READ_OTP_DATA                               0xA2
#define POWER_SAVING                                0xE3

typedef struct EPD_t {
  int reset_pin;
  int dc_pin;
  int cs_pin;
  int busy_pin;
  int width;
  int height;
} EPD;

/* Hardware operating functions */
int  EPD_Init(EPD* epd);
void EPD_WaitUntilIdle(EPD* epd);
void EPD_DelayMs(EPD* epd, unsigned int delay_time);
void EPD_Reset(EPD* epd);
void EPD_SetLut(EPD* epd);
void EPD_DisplayFrame(EPD* epd, const unsigned char* frame_black, const unsigned char* frame_red);
void EPD_Sleep(EPD* epd);
void EPD_DigitalWrite(EPD* epd, int pin, int value);
int  EPD_DigitalRead(EPD* epd, int pin);
void EPD_SendCommand(EPD* epd, unsigned char command);
void EPD_SendData(EPD* epd, unsigned char data);

#endif /* EPD2IN13B_H */

/* END OF FILE */
