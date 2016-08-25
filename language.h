#ifndef LANGUAGE_H
#define LANGUAGE_H 1

#include "syslang.h"
#include "sysmarlin.h"

// LCD Menu Messages
#define LANGUAGE_INCL_(M) STRINGIFY_(language_##M.h)
#define LANGUAGE_INCL(M) LANGUAGE_INCL_(M)
#define INCLUDE_LANGUAGE LANGUAGE_INCL(LCD_LANGUAGE)

// example: #include language_cn.h
//#include INCLUDE_LANGUAGE
#include LANGUAGE_INCL(LCD_LANGUAGE)


#define LANGUAGE_DATA_INCL_(M) STRINGIFY_(language_data_##M.h)
#define LANGUAGE_DATA_INCL(M) LANGUAGE_DATA_INCL_(M)
#define INCLUDE_LANGUAGE_DATA LANGUAGE_DATA_INCL(LCD_LANGUAGE)

//#define LANGUAGE_DATA_INCL(M) STRINGIFY_(language_data_##M.h)
//#include LANGUAGE_DATA_INCL(cn)

#endif // LANGUAGE_H

