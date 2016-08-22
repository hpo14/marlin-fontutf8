#ifndef _SYS_MARLIN_H
#define _SYS_MARLIN_H

// the definitions from Marlin
#define JAPANESE 1
#define WESTERN  2
#define CYRILLIC 3

//#define DISPLAY_CHARSET_HD44780 JAPANESE
#define DISPLAY_CHARSET_HD44780 WESTERN
//#define DISPLAY_CHARSET_HD44780 CYRILLIC



//#define LCD_LANGUAGE en
//#define LCD_LANGUAGE cn
#define LCD_LANGUAGE jp
//m#define LCD_LANGUAGE ru




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


#endif // _SYS_MARLIN_H
