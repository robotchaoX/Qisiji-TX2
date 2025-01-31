/*
 * jetsonGPIO.h
 *
 * Copyright (c) 2015 JetsonHacks
 * www.jetsonhacks.com
 *
 * Based on Software by RidgeRun
 * Originally from:
 * https://developer.ridgerun.com/wiki/index.php/Gpio-int-test.c
 * Copyright (c) 2011, RidgeRun
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef JETSONGPIO_H_
#define JETSONGPIO_H_

#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT (3 * 1000) /* 3 seconds */
#define MAX_BUF 64

typedef unsigned int pinGPIONumber;
typedef unsigned int pinDirection;
typedef unsigned int pinValue;

enum pinDirections { inputPin = 0, outputPin = 1 };

enum pinValues {
  low = 0,
  high = 1,
  off = 0, // synonym for things like lights
  on = 1
};

// jetsonTK1 GPIO??
// enum jetsonGPIONumber {
//  gpio57 = 57,   // J3A1 - Pin 50
//  gpio160 = 160, // J3A2 - Pin 40
//  gpio161 = 161, // J3A2 - Pin 43
//  gpio162 = 162, // J3A2 - Pin 46
//  gpio163 = 163, // J3A2 - Pin 49
//  gpio164 = 164, // J3A2 - Pin 52
//  gpio165 = 165, // J3A2 - Pin 55
//  gpio166 = 166  // J3A2 - Pin 58
//};

// jetsonTX1 GPIO
// enum jetsonTX1GPIONumber {
//  gpio36 = 36,   // J21 - Pin 32 - Unused - AO_DMIC_IN_CLK
//  gpio37 = 37,   // J21 - Pin 16 - Unused - AO_DMIC_IN_DAT
//  gpio38 = 38,   // J21 - Pin 13 - Bidir  - GPIO20/AUD_INT
//  gpio63 = 63,   // J21 - Pin 33 - Bidir  - GPIO11_AP_WAKE_BT
//  gpio184 = 184, // J21 - Pin 18 - Input  - GPIO16_MDM_WAKE_AP
//  gpio186 = 186, // J21 - Pin 31 - Input  - GPIO9_MOTION_INT
//  gpio187 = 187, // J21 - Pin 37 - Output - GPIO8_ALS_PROX_INT
//  gpio219 = 219, // J21 - Pin 29 - Output - GPIO19_AUD_RST
//};

// // jetsonTX2 GPIO 原装载板
// enum jetsonTX2GPIONumber {
//  gpio397 = 397, // J21 - Pin 13  - GPIO_GEN2 Audio Code Interrupt
//  gpio389 = 389, // J21 - Pin 33  - GPIO13 AP Wake Bt GPIO
//  gpio481 = 481, // J21 - Pin 18  - GPIO_GEN5 Modem Wake AP GPIO
//  gpio398 = 398, // J21 - Pin 29  - GPIO5 Audio Reset (1.8/3.3V)
//
//};

// jetsonTX2 GPIO 图为007载板
enum jetsonTX2GPIONumber {
  gpio388 = 388, //  Pin7  / GPIO8
  gpio298 = 298, //  Pin8  / GPIO9
  gpio480 = 480, //  Pin9  / GPIO_EXP0_INT
  gpio486 = 486, //  Pin10 / GPIO_EXP1_INT

};

class GPIO_Device {
public:
  pinGPIONumber gpio_Number;
  //  unsigned int gpio_ID;
  pinDirection gpio_Direction;
  unsigned int gpio_Value; // gpioGetValue
  //  pinValue gpio_Value; // gpioGetValue 的结果

  char *gpio_Edge;
  unsigned int gpio_ActiveLowValue;
  int gpio_FileDescriptor;
  char gpio_CommandBuffer[MAX_BUF];

  GPIO_Device();
  ~GPIO_Device();
  int gpioSetGPIONumber(pinGPIONumber gpioX);
  int gpioExport();
  int gpioUnexport();
  int gpioSetDirection(pinDirection direction);
  int gpioGetDirection();
  int gpioSetValue(pinValue setValue);
  int gpioGetValue();
  int gpioSetEdge(char *edge);
  int gpioOpen();
  int gpioClose();
  int gpioActiveLow(bool activeLowValue);
};

#endif // JETSONGPIO_H_
