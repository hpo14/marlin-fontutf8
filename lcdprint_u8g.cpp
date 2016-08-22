/**
 * @file    lcdprint_u8g.c
 * @brief   LCD print api for u8glib
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

#include "fontutils.h"
#include "u8g_fontutf8.h"

#include "lcdprint.h"

////////////////////////////////////////////////////////////
#if ! USE_HD44780
void
lcd_moveto (int col, int row)
{
    u8g.setPrintPos (col, row);
}

int
lcd_printstr (const char * utf8_str, pixel_len_t max_length)
{
    unsigned int ret;
    unsigned int x;
    unsigned int y;

    x = u8g.getPrintCol();
    y = u8g.getPrintRow();
    ret = uxg_DrawUtf8Str (u8g.getU8g(), x, y, utf8_str, max_length);
    u8g.setPrintPos (x + ret, y);

    return ret;
}

int
lcd_printstr_P (const char * utf8_str_P, pixel_len_t max_length)
{
    unsigned int ret;
    unsigned int x;
    unsigned int y;

    x = u8g.getPrintCol();
    y = u8g.getPrintRow();
    ret = uxg_DrawUtf8StrP (u8g.getU8g(), x, y, utf8_str_P, max_length);
    u8g.setPrintPos (x + ret, y);

    return ret;
}

#endif // ! USE_HD44780