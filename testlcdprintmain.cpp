/**
 * @file    testlcdprintmain.cpp
 * @brief   Test the LCD
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

// ref:
// localization by gettext or Qt
// http://stackoverflow.com/questions/11789615/how-to-support-multiple-language-in-a-linux-c-c-program
// http://fedoraproject.org/wiki/How_to_do_I18N_through_gettext
// http://doc.qt.io/qt-5/internationalization.html


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

#include "Marlin.h"
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


#if USE_HD44780
#define LCD_COL 16
#define LCD_ROW  2

#if 0
#undef LCD_COL
#undef LCD_ROW
#define LCD_COL 20
#define LCD_ROW  4
#endif

#else
#define LCD_COL 128
#define LCD_ROW  64
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

const char c00[] PROGMEM = "mix eng and " WELCOME_MSG;
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

const char c21[] PROGMEM = MSG_SAMPLE2_EN;
const char c22[] PROGMEM = MSG_SAMPLE2_RU;
const char c23[] PROGMEM = MSG_SAMPLE2_JP;
const char c24[] PROGMEM = MSG_SAMPLE2_CNZH;
const char c25[] PROGMEM = MSG_SAMPLE2_CNTW;
const char c26[] PROGMEM = MSG_SAMPLE2_FR;

const char c27[] PROGMEM = _UxGT("千万円");
const char c28[] PROGMEM = _UxGT("→↰↻⏱°🌡📂");
const char c29[] PROGMEM = _UxGT("⌚⌛⏰⏱⏳⧖⧗→↰");
const char c30[] PROGMEM = _UxGT("°🌡➤📁📂↻↺⟳⟲⮈⮊⮋⮉⮥⮭⇧↑");
const char c31[] PROGMEM = _UxGT("🕐🕑🕒🕓🕔🕕🕖🕗🕘🕙🕚🕛");
const char c32[] PROGMEM = _UxGT("🕜🕝🕞🕟🕠🕡🕢🕣🕤🕥🕦🕧");

//const char * const g_cstr_samples[] PROGMEM = {
PGM_P const g_cstr_samples[] PROGMEM = {
    //PSTR(MSG_SAMPLE2_FR),
    c00,
    c01,c02,c03,c04,c05,c06,
    c11,
    c21,
#if ENABLED(DOGLCD)
    c11,c12,c13,c14,c15,c16,
    c21,c22,c23,c24,c25,c26,
#endif
    c27,c28,c29,//c30,c31
};

static int cnt_lcd = 0;
void
update_idx ()
{
    cnt_lcd = (cnt_lcd + 1) % NUM_ARRAY(g_cstr_samples);
}

int
show_lcd(void)
{
    int i;
    PGM_P p;
    char buf[50];
#if 1
    for (i = 0; i * lcd_glyph_height() < LCD_ROW; i ++) {
        lcd_moveto (0, i * lcd_glyph_height());
        //lcd_moveto (0, (i + 1) * 12);
        memcpy_P(&p, &g_cstr_samples[(cnt_lcd + i) % NUM_ARRAY(g_cstr_samples)], sizeof(PGM_P));
        //TRACE ("call utf8_strlen_p ...");
        sprintf (buf, "%d ", utf8_strlen_p(p)); lcd_print (buf);
        //lcd_printPGM (p);
        utf8_strncpy_p (buf, p, sizeof(buf)); lcd_print (buf);
    }
#else
    i=0;
    lcd_moveto (10, LCD_ROW / 2);
    TRACE ("show u8g string idx=%d", cnt_lcd);
    memcpy_P(&p, &g_cstr_samples[(cnt_lcd + i) % NUM_ARRAY(g_cstr_samples)], sizeof(PGM_P));
    lcd_printPGM (p);
#endif
}

////////////////////////////////////////////////////////////

#if ENABLED(DOGLCD)
// use u8g
#include <U8glib.h>
#include "language.h"

#include LANGUAGE_DATA_INCL(LCD_LANGUAGE)

#include "dogm_font_data_ISO10646_1.h"
#define FONT_MENU_NAME ISO10646_1_5x7

#define OLED_SPI1_CS   10   //   ---   x Not exist
#define OLED_SPI1_DC    8   //   D/C   pin# 6 (data or command)
#define OLED_SPI1_RST   7   //   RST   pin# 5 U8G_PIN_NONE
#define OLED_SPI1_MOSI 11   //   SDA   pin# 4
#define OLED_SPI1_CLK  13   //   SCL   pin# 3

U8GLIB_SSD1306_128X64 u8g(OLED_SPI1_CLK, OLED_SPI1_MOSI, OLED_SPI1_CS, OLED_SPI1_DC, OLED_SPI1_RST);
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_DEV_0|U8G_I2C_OPT_NO_ACK|U8G_I2C_OPT_FAST);
//U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NO_ACK);
U8GLIB *pu8g = &u8g;

void
u8g_prepare(U8GLIB * pu8g)
{
    TRACE ("u8g prepare");
    pu8g->setFont(u8g_font_6x10);
    //pu8g->setFont(bleeding_cowboys);
    pu8g->setFontRefHeightExtendedText();
    pu8g->setDefaultForegroundColor();
    pu8g->setFontPosTop();
}

void
clear_lcd ()
{
    TRACE ("u8g clear_lcd");
}

void
setup_lcd ()
{
    TRACE ("u8g setup_lcd");

    pu8g = &u8g;
    u8g_prepare(pu8g);
    u8g.setFont(FONT_MENU_NAME);
    uxg_SetUtf8Fonts (g_fontinfo, NUM_ARRAY(g_fontinfo));
}

void
lcd_update (void)
{
    u8g_t * pu8g1;
    pu8g1 = u8g.getU8g();
    TRACE ("u8g lcd_update");
    clear_lcd ();
    u8g.firstPage();
    do {
#if 0
        show_lcd();
#else
        lcd_moveto (5, 20);
        lcd_print (LCD_STR_BEDTEMP); //lcd_print_wchar (LCD_STR_BEDTEMP[0]);
        lcd_print (LCD_STR_DEGREE);
        lcd_print (LCD_STR_THERMOMETER);
        lcd_print (LCD_STR_UPLEVEL);
        lcd_print (LCD_STR_REFRESH);
        lcd_print (LCD_STR_FOLDER);
        lcd_print (LCD_STR_FEEDRATE);
        lcd_print (LCD_STR_CLOCK);

        //uxg_DrawUtf8StrP(pu8g1, 10, 32, c01, 10000);
        lcd_moveto (5, 40);
        lcd_printPGM (c01);
#endif
    } while( u8g.nextPage() );
    update_idx ();
}

#else
#include <LiquidCrystal.h>

//LiquidCrystal(uint8_t rs, uint8_t rw, uint8_t enable,
//    uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
//    uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7);
LiquidCrystal lcd(PIN_LCD_RS, PIN_LCD_RW, PIN_LCD_EN,   PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

// from Marlin:
static byte bedTemp[8] = {
    B00000,
    B11111,
    B10101,
    B10001,
    B10101,
    B11111,
    B00000,
    B00000
}; //thanks Sonny Mounicou

static byte degree[8] = {
    B01100,
    B10010,
    B10010,
    B01100,
    B00000,
    B00000,
    B00000,
    B00000
};

static byte thermometer[8] = {
    B00100,
    B01010,
    B01010,
    B01010,
    B01010,
    B10001,
    B10001,
    B01110
};

static byte feedrate[8] = {
    B11100,
    B10000,
    B11000,
    B10111,
    B00101,
    B00110,
    B00101,
    B00000
}; //thanks Sonny Mounicou

static byte clock[8] = {
    B00000,
    B01110,
    B10011,
    B10101,
    B10001,
    B01110,
    B00000,
    B00000
}; //thanks Sonny Mounicou

static byte uplevel[8] = {
    B00100,
    B01110,
    B11111,
    B00100,
    B11100,
    B00000,
    B00000,
    B00000
}; //thanks joris

static byte refresh[8] = {
    B00000,
    B00110,
    B11001,
    B11000,
    B00011,
    B10011,
    B01100,
    B00000,
}; //thanks joris

static byte folder[8] = {
    B00000,
    B11100,
    B11111,
    B10001,
    B10001,
    B11111,
    B00000,
    B00000
}; //thanks joris

void
createchar_lcd ()
{
    lcd.createChar(LCD_STR_BEDTEMP[0], bedTemp);
    lcd.createChar(LCD_STR_DEGREE[0], degree);
    lcd.createChar(LCD_STR_THERMOMETER[0], thermometer);
    lcd.createChar(LCD_STR_UPLEVEL[0], uplevel);
    lcd.createChar(LCD_STR_REFRESH[0], refresh);
    lcd.createChar(LCD_STR_FOLDER[0], folder);
    lcd.createChar(LCD_STR_FEEDRATE[0], feedrate);
    lcd.createChar(LCD_STR_CLOCK[0], clock);
}

void
setup_lcd ()
{
    //pinMode(9, OUTPUT);digitalWrite (9, LOW);
    // set up the LCD's number of columns and rows:
    createchar_lcd();
    lcd.begin(LCD_COL, LCD_ROW);
#if DEBUG
    // Print a message to the LCD.
    //lcd.print("hello, world!");
    //lcd_print ("hello"); lcd_print_wchar ('-');
    //lcd_printPGM (PSTR("HELLO"));
    //test_show_uchar();

    lcd_print_wchar (LCD_STR_BEDTEMP[0]);
    lcd_print (LCD_STR_DEGREE);
    lcd_print (LCD_STR_THERMOMETER);
    lcd_print (LCD_STR_UPLEVEL);
    lcd_print (LCD_STR_REFRESH);
    lcd_print (LCD_STR_FOLDER);
    lcd_print (LCD_STR_FEEDRATE);
    lcd_print (LCD_STR_CLOCK);

    //lcd_moveto (0,1);
    //lcd_printPGM (c28);
#endif
}

void
clear_lcd ()
{
    lcd.clear();
    lcd_moveto(0,0); lcd_print ("                ");
    lcd_moveto(0,1); lcd_print ("                ");
    lcd_moveto(0,0);
}

void
lcd_update (void)
{
    clear_lcd ();
    show_lcd();
    update_idx ();
}

#endif // DOGLCD

/////////////////////////////////////////////////////////////////////////////
void
setup(void)
{
#if DEBUG && defined(ARDUINO)
    Serial.begin(9600);
    delay (1000); // delay to avoid endless loop in tx, and the user can flash a new bin
    // the user can also: 1) shutdown power, 2) press and hold reset key, 3) download bin(release key)

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

static unsigned long pre_tm_lcd = 0;
void
loop(void)
{
    unsigned long now = millis();
    if (pre_tm_lcd + 2000 < now) {
        pre_tm_lcd = now;

        lcd_update();
    }
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
