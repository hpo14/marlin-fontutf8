/**
 * @file    testlcdprint.ino
 * @brief   Test the LCD
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#if defined(ARDUINO)
#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <SPI.h>
#include <Wire.h>
#include <U8glib.h>
#else
#include <unistd.h>
#define delay(a) usleep(a)
#endif

#if defined(STM32_MCU_SERIES)
#define USE_MAPLEMINI 1
#endif

#if USE_MAPLEMINI
#define serialportgps Serial3
//#include <SoftwareSerial.h>
//export SoftwareSerial nss(0, 1);             //set sotfware serial communications to arduino ports 3 and 4 (TX = 3 and RX = 4)
//#define serialportgps nss
#else
#include <SoftwareSerial.h>
extern SoftwareSerial nss;             //set sotfware serial communications to arduino ports 3 and 4 (TX = 3 and RX = 4)
#define serialportgps nss
#endif

