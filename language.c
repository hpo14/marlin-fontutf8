
#include "lcdprint.h"
#include "language.h"

#if ! USE_HD44780
// example: #include language_data_cn.h
#include LANGUAGE_DATA_INCL(LCD_LANGUAGE)
#endif


// return another ROM string from a ROM string
const char * gettext_p2p (const char * msgid);

// return another ROM string from a RAM string
const char * gettext_a2p (char * msgid);



