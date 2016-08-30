#ifndef _SYS_MARLIN_H
#define _SYS_MARLIN_H


#define U8GLIB_SSD1306 1
//#define REPRAP_DISCOUNT_SMART_CONTROLLER 1
//#define REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER 1


//#define LCD_LANGUAGE en
//#define LCD_LANGUAGE cn
#define LCD_LANGUAGE jp
//#define LCD_LANGUAGE ru


// the definitions from Marlin
#define JAPANESE 1
#define WESTERN  2
#define CYRILLIC 3

#if LCD_LANGUAGE == jp
#define DISPLAY_CHARSET_HD44780 JAPANESE
#elif LCD_LANGUAGE == ru
#define DISPLAY_CHARSET_HD44780 CYRILLIC
#else
#define DISPLAY_CHARSET_HD44780 WESTERN
#endif

#define SERIAL_ECHO(a) Serial.print(a)
#define SERIAL_EOL Serial.println("")


#define FORCE_INLINE __attribute__((always_inline)) inline
#define UNUSED(x) (void) (x)

// Macros to make a string from a macro
#define STRINGIFY_(M) #M
#define STRINGIFY(M) STRINGIFY_(M)

// Macros to support option testing
#define _CAT(a, ...) a ## __VA_ARGS__
#define SWITCH_ENABLED_false 0
#define SWITCH_ENABLED_true  1
#define SWITCH_ENABLED_0     0
#define SWITCH_ENABLED_1     1
#define SWITCH_ENABLED_      1
#define ENABLED(b) _CAT(SWITCH_ENABLED_, b)
#define DISABLED(b) (!_CAT(SWITCH_ENABLED_, b))

// Generic support for SSD1306 / SH1106 OLED based LCDs.
#if ENABLED(U8GLIB_SSD1306) || ENABLED(U8GLIB_SH1106)
  #define ULTRA_LCD  //general LCD support, also 16x2
  #define DOGLCD  // Support for I2C LCD 128x64 (Controller SSD1306 / SH1106 graphic Display Family)
#endif

#if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)
  #define DOGLCD
  #define U8GLIB_ST7920
  #define REPRAP_DISCOUNT_SMART_CONTROLLER
#endif

#if ENABLED(ULTIMAKERCONTROLLER)              \
 || ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER) \
 || ENABLED(G3D_PANEL)                        \
 || ENABLED(RIGIDBOT_PANEL)                   \
 || ENABLED(REPRAPWORLD_KEYPAD)
  #define ULTIPANEL
  #define NEWPANEL
#endif


  #if ENABLED(DOGLCD) // Change number of lines to match the DOG graphic display
    #ifndef LCD_WIDTH
      #define LCD_WIDTH 22
    #endif
    #ifndef LCD_HEIGHT
      #define LCD_HEIGHT 5
    #endif
  #endif

  #if ENABLED(ULTIPANEL)
    #define NEWPANEL  //enable this if you have a click-encoder panel
    #define ULTRA_LCD
    #ifndef LCD_WIDTH
      #define LCD_WIDTH 20
    #endif
    #ifndef LCD_HEIGHT
      #define LCD_HEIGHT 4
    #endif
  #else //no panel but just LCD
    #if ENABLED(ULTRA_LCD)
      #ifndef LCD_WIDTH
        #define LCD_WIDTH 16
      #endif
      #ifndef LCD_HEIGHT
        #define LCD_HEIGHT 2
      #endif
    #endif
  #endif

  #if ENABLED(DOGLCD)
    /* Custom characters defined in font dogm_font_data_Marlin_symbols.h / Marlin_symbols.fon */
    // \x00 intentionally skipped to avoid problems in strings
    #define LCD_STR_REFRESH     "\x01"
    #define LCD_STR_FOLDER      "\x02"
    #define LCD_STR_ARROW_RIGHT "\x03"
    #define LCD_STR_UPLEVEL     "\x04"
    #define LCD_STR_CLOCK       "\x05"
    #define LCD_STR_FEEDRATE    "\x06"
    #define LCD_STR_BEDTEMP     "\x07"
    #define LCD_STR_THERMOMETER "\x08"
    #define LCD_STR_DEGREE      "\x09"

    #define LCD_STR_SPECIAL_MAX '\x09'
    // Maximum here is 0x1f because 0x20 is ' ' (space) and the normal charsets begin.
    // Better stay below 0x10 because DISPLAY_CHARSET_HD44780_WESTERN begins here.
  #else
    /* Custom characters defined in the first 8 characters of the LCD */
    #define LCD_STR_BEDTEMP     "\x00"  // Print only as a char. This will have 'unexpected' results when used in a string!
    #define LCD_STR_DEGREE      "\x01"
    #define LCD_STR_THERMOMETER "\x02"
    #define LCD_STR_UPLEVEL     "\x03"
    #define LCD_STR_REFRESH     "\x04"
    #define LCD_STR_FOLDER      "\x05"
    #define LCD_STR_FEEDRATE    "\x06"
    #define LCD_STR_CLOCK       "\x07"
    #define LCD_STR_ARROW_RIGHT ">"  /* from the default character set */
  #endif

  /**
   * Default LCD contrast for dogm-like LCD displays
   */
  #if ENABLED(DOGLCD)

    #define HAS_LCD_CONTRAST ( \
        ENABLED(MAKRPANEL) \
     || ENABLED(CARTESIO_UI) \
     || ENABLED(VIKI2) \
     || ENABLED(miniVIKI) \
     || ENABLED(ELB_FULL_GRAPHIC_CONTROLLER) \
    )

    #if HAS_LCD_CONTRAST
      #ifndef LCD_CONTRAST_MIN
        #define LCD_CONTRAST_MIN 0
      #endif
      #ifndef LCD_CONTRAST_MAX
        #define LCD_CONTRAST_MAX 63
      #endif
      #ifndef DEFAULT_LCD_CONTRAST
        #define DEFAULT_LCD_CONTRAST 32
      #endif
    #endif
  #endif

  #ifndef BOOTSCREEN_TIMEOUT
    #define BOOTSCREEN_TIMEOUT 2500
  #endif



// RAMPS 1.4
#if ENABLED(ULTRA_LCD)

  #if ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
    #define LCD_PINS_RS     49 //CS chip select /SS chip slave select
    #define LCD_PINS_ENABLE 51 //SID (MOSI)
    #define LCD_PINS_D4     52 //SCK (CLK) clock
  #elif ENABLED(NEWPANEL) && ENABLED(PANEL_ONE)
    #define LCD_PINS_RS 40
    #define LCD_PINS_ENABLE 42
    #define LCD_PINS_D4 65
    #define LCD_PINS_D5 66
    #define LCD_PINS_D6 44
    #define LCD_PINS_D7 64
  #else
    #define LCD_PINS_RS 16
    #define LCD_PINS_ENABLE 17
    #define LCD_PINS_D4 23
    #define LCD_PINS_D5 25
    #define LCD_PINS_D6 27
    #define LCD_PINS_D7 29
    #if DISABLED(NEWPANEL)
      #define BEEPER_PIN 33
      // Buttons are attached to a shift register
      // Not wired yet
      //#define SHIFT_CLK 38
      //#define SHIFT_LD 42
      //#define SHIFT_OUT 40
      //#define SHIFT_EN 17
    #endif
  #endif

  #if ENABLED(NEWPANEL)

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)
      #define BEEPER_PIN 37

      #define BTN_EN1 31
      #define BTN_EN2 33
      #define BTN_ENC 35

      #define SD_DETECT_PIN 49
      #define KILL_PIN 41

      #if ENABLED(BQ_LCD_SMART_CONTROLLER)
        #define LCD_PIN_BL 39
      #endif

    #elif ENABLED(REPRAPWORLD_GRAPHICAL_LCD)
      #define BTN_EN1 64
      #define BTN_EN2 59
      #define BTN_ENC 63
      #define SD_DETECT_PIN 42
    #elif ENABLED(LCD_I2C_PANELOLU2)
      #define BTN_EN1 47  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 43
      #define BTN_ENC 32
      #define LCD_SDSS 53
      #define SD_DETECT_PIN -1
      #define KILL_PIN 41
    #elif ENABLED(LCD_I2C_VIKI)
      #define BTN_EN1 22  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 7   // http://files.panucatt.com/datasheets/viki_wiring_diagram.pdf
                          // tells about 40/42.
                          // 22/7 are unused on RAMPS_14. 22 is unused and 7 the SERVO0_PIN on RAMPS_13.
      #define BTN_ENC -1
      #define LCD_SDSS 53
      #define SD_DETECT_PIN 49
    #elif ENABLED(VIKI2) || ENABLED(miniVIKI)
      #define BEEPER_PIN       33

      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0        44
      #define DOGLCD_CS        45
      #define LCD_SCREEN_ROT_180

      #define BTN_EN1          22
      #define BTN_EN2           7
      #define BTN_ENC          39

      #define SDSS             53
      #define SD_DETECT_PIN    -1  // Pin 49 for display sd interface, 72 for easy adapter board

      #define KILL_PIN         31

      #if ENABLED(TEMP_STAT_LEDS)
        #define STAT_LED_RED   32
        #define STAT_LED_BLUE  35
      #endif
    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #define BTN_EN1 35  // reverse if the encoder turns the wrong way.
      #define BTN_EN2 37
      #define BTN_ENC 31
      #define SD_DETECT_PIN 49
      #define LCD_SDSS 53
      #define KILL_PIN 41
      #define BEEPER_PIN 23
      #define DOGLCD_CS 29
      #define DOGLCD_A0 27
      #define LCD_PIN_BL 33
    #elif ENABLED(MINIPANEL)
      #define BEEPER_PIN 42
      // Pins for DOGM SPI LCD Support
      #define DOGLCD_A0  44
      #define DOGLCD_CS  66
      #define LCD_PIN_BL 65 // backlight LED on A11/D65
      #define SDSS   53

      #define KILL_PIN 64
      // GLCD features
      //#define LCD_CONTRAST 190
      // Uncomment screen orientation
      //#define LCD_SCREEN_ROT_90
      //#define LCD_SCREEN_ROT_180
      //#define LCD_SCREEN_ROT_270
      //The encoder and click button
      #define BTN_EN1 40
      #define BTN_EN2 63
      #define BTN_ENC 59
      //not connected to a pin
      #define SD_DETECT_PIN 49

    #else

      // Beeper on AUX-4
      #define BEEPER_PIN 33

      // buttons are directly attached using AUX-2
      #if ENABLED(REPRAPWORLD_KEYPAD)
        #define BTN_EN1 64 // encoder
        #define BTN_EN2 59 // encoder
        #define BTN_ENC 63 // enter button
        #define SHIFT_OUT 40 // shift register
        #define SHIFT_CLK 44 // shift register
        #define SHIFT_LD 42 // shift register
      #elif ENABLED(PANEL_ONE)
        #define BTN_EN1 59 // AUX2 PIN 3
        #define BTN_EN2 63 // AUX2 PIN 4
        #define BTN_ENC 49 // AUX3 PIN 7
      #else
        #define BTN_EN1 37
        #define BTN_EN2 35
        #define BTN_ENC 31  // the click
      #endif

      #if ENABLED(G3D_PANEL)
        #define SD_DETECT_PIN 49
        #define KILL_PIN 41
      #else
        //        #define SD_DETECT_PIN -1  // Ramps doesn't use this
      #endif

    #endif
  #endif // NEWPANEL

#endif // ULTRA_LCD


#if defined(ARDUINO)
#include <Arduino.h>
#endif

#endif // _SYS_MARLIN_H

