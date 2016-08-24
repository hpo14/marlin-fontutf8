#ifndef _SYS_LANGUAGE_H
#define _SYS_LANGUAGE_H

// the definitions for the language engine

#define USE_HD44780 0

#define LANGUAGE_DATA_INCL_(M) STRINGIFY_(language_data_##M.h)
#define LANGUAGE_DATA_INCL(M) LANGUAGE_DATA_INCL_(M)
#define INCLUDE_LANGUAGE_DATA LANGUAGE_DATA_INCL(LCD_LANGUAGE)

//#define LANGUAGE_DATA_INCL(M) STRINGIFY_(language_data_##M.h)
//#include LANGUAGE_DATA_INCL(cn)

#endif // _SYS_LANGUAGE_H
