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
#include <U8glib.h>

extern U8GLIB *pu8g;

int
lcd_glyph_height(void)
{
    return u8g_GetFontBBXHeight(pu8g->getU8g());
    //return u8g_GetFontBBXOffY(pu8g->getU8g());
}

void
lcd_moveto (int col, int row)
{
    //TRACE ("u8g moveto (%d, %d)", col, row);
    pu8g->setPrintPos (col, row);
}

int
lcd_printstr (const char * utf8_str, pixel_len_t max_length)
{
    unsigned int ret;
    unsigned int x;
    unsigned int y;

    x = pu8g->getPrintCol();
    y = pu8g->getPrintRow();
    TRACE ("uxg_DrawUtf8Str(x=%d,y=%d,maxlen=%d", x, y, max_length);
    ret = uxg_DrawUtf8Str (pu8g->getU8g(), x, y, utf8_str, max_length);
    TRACE ("u8g->setPrintPos(x=%d + ret=%d,y=%d", x, ret, y);
    pu8g->setPrintPos (x + ret, y);

    return ret;
}

int
lcd_printstr_P (const char * utf8_str_P, pixel_len_t max_length)
{
    unsigned int ret;
    unsigned int x;
    unsigned int y;

    x = pu8g->getPrintCol();
    y = pu8g->getPrintRow();
    TRACE ("uxg_DrawUtf8StrP(x=%d,y=%d,maxlen=%d", x, y, max_length);
    ret = uxg_DrawUtf8StrP (pu8g->getU8g(), x, y, utf8_str_P, max_length);
    TRACE ("u8g->setPrintPos(x=%d + ret=%d,y=%d", x, ret, y);
    pu8g->setPrintPos (x + ret, y);

    return ret;
}

#endif // ! USE_HD44780
