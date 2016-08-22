/**
 * @file    testlcdprintmain.cpp
 * @brief   Test the LCD
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

#include "lcdprint.h"

#if defined(ARDUINO)

#if ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#ifdef __cplusplus__
#if DEBUG
#include "HardwareSerial.h"
#include "USBAPI.h"
#endif
#include <Wire.h>
#endif

#elif defined(U8G_RASPBERRY_PI)
#include <unistd.h>
//#define delay(a) usleep((a) * 1000)

#define USE_U8G 1

#elif USE_SDL

#undef USE_U8G
#define USE_U8G 1
#undef USE_SDL
#define USE_SDL 1
#define delay(a) SDL_Delay((a)*1000)

#else
#include <stdio.h>
#include <unistd.h>
//#define delay(a) usleep((a) * 1000)

#endif


#include "syslang.h"
#include "sysmarlin.h"

#include "language.h"


/////////////////////////////////////////////////////////////////////////////
#ifndef TRACE
#define TRACE(...)
#endif

#define TRACE0 TRACE
#define TRACE1 TRACE
#define TRACE2 TRACE
#define TRACE3 TRACE

////////////////////////////////////////////////////////////
#define PIN_LCD_RS  8
#define PIN_LCD_RW 12
#define PIN_LCD_EN 13
#define PIN_LCD_D4  4
#define PIN_LCD_D5  5
#define PIN_LCD_D6  6
#define PIN_LCD_D7  7

#define PIN_RGBLED_R  9
#define PIN_RGBLED_B 10
#define PIN_RGBLED_G 11

#define PIN_ROTARY_BUT A0
#define PIN_ROTARY_A   A1
#define PIN_ROTARY_B   A2

#define LCD_COL 16
#define LCD_ROW  2

#if 1
#undef LCD_COL
#undef LCD_ROW
#define LCD_COL 20
#define LCD_ROW  4
#endif

/////////////////////////////////////////////////////////////////////////////
// sample translations
#define MSG_SAMPLE1_EN   _UxGT("Welcome")
#define MSG_SAMPLE1_RU   _UxGT("добро пожаловать")
#define MSG_SAMPLE1_JP   _UxGT("歓迎")
#define MSG_SAMPLE1_CNZH _UxGT("欢迎")
#define MSG_SAMPLE1_CNTW _UxGT("歡迎")
#define MSG_SAMPLE1_FR   _UxGT("bienvenue")

#define MSG_SAMPLE2_EN   _UxGT("You did a great job!")
#define MSG_SAMPLE2_RU   _UxGT("Вы проделали хорошую работу!")
#define MSG_SAMPLE2_JP   _UxGT("あなたは素晴らしい仕事をしました！")
#define MSG_SAMPLE2_CNZH _UxGT("你做得很好！")
#define MSG_SAMPLE2_CNTW _UxGT("你做得很好！")
#define MSG_SAMPLE2_FR   _UxGT("Vous avez fait un très bon travail!")


const char c01[] PROGMEM = WELCOME_MSG;
const char c02[] PROGMEM = MSG_SD_INSERTED;
const char c03[] PROGMEM = MSG_SD_REMOVED;
const char c04[] PROGMEM = MSG_LCD_ENDSTOPS;
const char c05[] PROGMEM = MSG_MAIN;
const char c06[] PROGMEM = MSG_AUTOSTART;
const char c11[] PROGMEM = MSG_SAMPLE1_EN;
const char c12[] PROGMEM = MSG_SAMPLE1_RU;
const char c13[] PROGMEM = MSG_SAMPLE1_JP;
const char c14[] PROGMEM = MSG_SAMPLE1_CNZH;
const char c15[] PROGMEM = MSG_SAMPLE1_CNTW;
const char c16[] PROGMEM = MSG_SAMPLE1_FR;
const char c21[] PROGMEM = MSG_SAMPLE1_EN;
const char c22[] PROGMEM = MSG_SAMPLE1_RU;
const char c23[] PROGMEM = MSG_SAMPLE1_JP;
const char c24[] PROGMEM = MSG_SAMPLE1_CNZH;
const char c25[] PROGMEM = MSG_SAMPLE1_CNTW;
const char c26[] PROGMEM = MSG_SAMPLE1_FR;

//const char * const g_cstr_samples[] PROGMEM = {
PGM_P const g_cstr_samples[] PROGMEM = {
#if 1
    c01,c02,c03,c04,c05,c06, 
#elif 1
    c11,c12,c13,
    c21,c22,c23,
#else
    c11,c12,c13,c14,c15,c16,
    c21,c22,c23,c24,c25,c26,
#endif
    //PSTR(MSG_SAMPLE2_FR),
};

////////////////////////////////////////////////////////////

#if USE_HD44780
#include <LiquidCrystal.h>

//LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
//    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
//    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_EN,   PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

void
setup_lcd ()
{
    //pinMode(9, OUTPUT);digitalWrite (9, LOW);
    // set up the LCD's number of columns and rows:
    lcd.begin(LCD_COL, LCD_ROW);
    // Print a message to the LCD.
    //lcd.print("hello, world!");
    lcd_printPGM (PSTR("HELLO"));
    delay (2000);
}

static int pre_tm_lcd = 0;
static int cnt_lcd = 0;
int
test_lcd(void)
{
    int i;
    unsigned long now = millis();
    PGM_P p;
    if (pre_tm_lcd + 1000 < now) {
        pre_tm_lcd = now;
        cnt_lcd = (cnt_lcd + LCD_ROW) % NUM_TYPE(g_cstr_samples);
        for (i = 0; i < LCD_ROW; i ++) {
            lcd_moveto (0, i);
            memcpy_P(&p, &g_cstr_samples[(cnt_lcd + i) % NUM_TYPE(g_cstr_samples)], sizeof(PGM_P));
            lcd_printPGM (p);
        }
    }
    return 1;
}

#endif // USE_HD44780

/////////////////////////////////////////////////////////////////////////////
void
setup(void)
{
#if DEBUG && defined(ARDUINO)
    Serial.begin(9600);
    // Wait for USB Serial.
    while (!Serial) {}

    // Read any input
    delay(200);
    while (Serial.read() >= 0) {}

    //TRACE ("done setup serial port to 9600");
#endif
    setup_lcd ();
    //lcd_init();

}

void
loop(void)
{
    //lcd_update();
    TRACE ("test main");
    test_lcd ();
}

#if ! defined(ARDUINO)
int
main(void)
{
    setup();
    while (1) {
        loop();
        delay(500);
    }
    return 0;
}
#endif
