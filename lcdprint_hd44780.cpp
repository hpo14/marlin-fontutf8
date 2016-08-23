/**
 * @file    lcdprint_hd44780.c
 * @brief   LCD print api for HD44780
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */

/**
 * Due to the limitation of the HD44780 hardware, the current available LCD modules can only support
 *   Western(English), Cyrillic(Russian), Kana(Japanese) charsets.
 */

#include <string.h>

#include "fontutils.h"
#include "lcdprint.h"

#if USE_HD44780

#if defined(ARDUINO)
#include <LiquidCrystal.h>
extern LiquidCrystal lcd;
#define _lcd_write(a) lcd.write(a)
#else
#define _lcd_write(a) printf ("Write LCD: %c (%d)\n", (a), (int)(a));
#endif

////////////////////////////////////////////////////////////
typedef struct _hd44780_charmap_t {
    wchar_t uchar;
    uint8_t idx;
} hd44780_charmap_t;

#define IV(a) L##a

static const hd44780_charmap_t g_hd44780_charmap[] PROGMEM = {
    // sorted by uchar:
#if DISPLAY_CHARSET_HD44780 == JAPANESE

    {IV('¢'), 0xEC},
    {IV('ä'), 0xE1},
    {IV('ö'), 0xEF},
    {IV('÷'), 0xFD},
    {IV('ü'), 0xF5},
    {IV('ˣ'), 0xEB},
    {IV('Θ'), 0xF2},
    {IV('Σ'), 0xF6},
    {IV('Ω'), 0xF4},
    {IV('α'), 0xE0},
    {IV('β'), 0xE2},
    {IV('ε'), 0xE3}, // epsilon
    {IV('μ'), 0xE4}, // mu
    {IV('π'), 0xF7},
    {IV('ρ'), 0xE6}, // rho
    {IV('σ'), 0xE5}, // sigma
    {IV('√'), 0xE8},
    {IV('∞'), 0xF3},
    {IV('█'), 0xFF},

    //{IV(''), 0xA0},
    {IV('。'), 0xA1},
    {IV('「'), 0xA2},
    {IV('」'), 0xA3},
    {IV('〝'), 0xDE},
    {IV('゜'), 0xDF},
    {IV('ァ'), 0xA7},
    {IV('ア'), 0xB1},
    {IV('ィ'), 0xA8},
    {IV('イ'), 0xB2},
    {IV('ゥ'), 0xA9},
    {IV('ウ'), 0xB3},
    {IV('ェ'), 0xAA},
    {IV('エ'), 0xB4},
    {IV('ォ'), 0xAB},

    {IV('オ'), 0xB5},
    {IV('カ'), 0xB6},
    {IV('キ'), 0xB7},
    {IV('ク'), 0xB8},
    {IV('ケ'), 0xB9},
    {IV('コ'), 0xBA},
    {IV('サ'), 0xBB},
    {IV('シ'), 0xBC},
    {IV('ス'), 0xBD},
    {IV('セ'), 0xBE},
    {IV('ソ'), 0xBF},

    {IV('タ'), 0xC0},
    {IV('チ'), 0xC1},
    {IV('ッ'), 0xAF},
    {IV('ツ'), 0xC2},
    {IV('テ'), 0xC3},
    {IV('ト'), 0xC4},
    {IV('ナ'), 0xC5},
    {IV('ニ'), 0xC6},
    {IV('ヌ'), 0xC7},
    {IV('ネ'), 0xC8},
    {IV('ノ'), 0xC9},
    {IV('ハ'), 0xCA},
    {IV('ヒ'), 0xCB},
    {IV('フ'), 0xCC},
    {IV('ヘ'), 0xCD},
    {IV('ホ'), 0xCE},
    {IV('マ'), 0xCF},

    {IV('ミ'), 0xD0},
    {IV('ム'), 0xD1},
    {IV('メ'), 0xD2},
    {IV('モ'), 0xD3},
    {IV('ャ'), 0xAC},
    {IV('ヤ'), 0xD4},
    {IV('ュ'), 0xAD},
    {IV('ユ'), 0xD5},
    {IV('ョ'), 0xAE},
    {IV('ヨ'), 0xD6},
    {IV('ラ'), 0xD7},
    {IV('リ'), 0xD8},
    {IV('ル'), 0xD9},
    {IV('レ'), 0xDA},
    {IV('ロ'), 0xDB},
    {IV('ワ'), 0xDC},
    {IV('ヲ'), 0xA6},
    {IV('ン'), 0xDD},
    {IV('・'), 0xA5},
    {IV('ー'), 0xB0},
    {IV('ヽ'), 0xA4},

    //{IV('g'), 0xE7}, // error
    //{IV(''), 0xE9},
    //{IV('j'), 0xEA}, // error
    //{IV(''), 0xED},
    //{IV(''), 0xEE},

    //{IV('p'), 0xF0}, // error
    //{IV('q'), 0xF1}, // error
    //{IV(''), 0xF8},
    //{IV('y'), 0xF9}, // error
    {IV('万'), 0xFB},
    {IV('円'), 0xFC},
    {IV('千'), 0xFA},
    //{IV(''), 0xFE},

#elif DISPLAY_CHARSET_HD44780 == WESTERN
// 0x10 -- 0x1F (except 0x1C)
// 0x80 -- 0xFF (except 0xA7,0xB0,0xB1,0xB3,0xB4,0xBF,0xD1,0xF8,0xFA,0xFC-0xFF)

    {IV('¡'), 0xA9},
    {IV('¢'), 0xA4},
    {IV('£'), 0xA5},
    {IV('¥'), 0xA6},
    {IV('§'), 0xD2}, // section sign
    {IV('©'), 0xCF},

    {IV('ª'), 0x9D},
    {IV('«'), 0xBB},
    {IV('®'), 0xCE},

    {IV('°'), 0xB2},
    //{IV(''), 0xD1},
    {IV('±'), 0x10}, //∓±
    //{'='), 0x1C}, // error
    {IV('²'), 0x1E},
    {IV('³'), 0x1F},
    {IV('¶'), 0xD3}, // pilcrow sign
    {IV('º'), 0x9E},
    {IV('»'), 0xBC},
    //{IV(''), 0xB3}, // error
    //{IV(''), 0xB4}, // error
    {IV('¼'), 0xB6},
    {IV('½'), 0xB5},
    {IV('¿'), 0x9F},

    {IV('Â'), 0x8F},
    {IV('Ã'), 0xAA},
    {IV('Ä'), 0x8E},
    {IV('Æ'), 0x92},
    {IV('Ç'), 0x80},
    {IV('É'), 0x90},
    {IV('Ñ'), 0x9C},
    {IV('Õ'), 0xAC},
    {IV('Ö'), 0x99},
    {IV('×'), 0xB7},
    {IV('Ø'), 0xAE},
    {IV('Ü'), 0x9A},
    {IV('à'), 0x85},
    {IV('á'), 0xA0},
    {IV('â'), 0x83},
    {IV('ã'), 0xAB},
    {IV('ä'), 0x84},
    {IV('å'), 0x86},
    {IV('æ'), 0x91},
    {IV('ç'), 0x87},
    {IV('è'), 0x8A},
    {IV('é'), 0x82},
    {IV('ê'), 0x88},
    {IV('ë'), 0x89},
    {IV('ì'), 0x8D},
    {IV('í'), 0xA1},
    {IV('î'), 0x8C},
    {IV('ï'), 0x8B},

    {IV('ñ'), 0x9B},
    {IV('ò'), 0x95},
    {IV('ó'), 0xA2},
    {IV('ô'), 0x93},
    {IV('õ'), 0xAD},
    {IV('ö'), 0x94},
    {IV('÷'), 0xB8},
    {IV('ø'), 0xAF},
    {IV('ù'), 0x97},
    {IV('ú'), 0xA3},
    {IV('û'), 0x96},
    {IV('ü'), 0x81},
    {IV('ÿ'), 0x98},


    //{IV(''), 0xB0}, // error
    //{IV(''), 0xB1}, // error
    {IV('ƒ'), 0xA8},

    {IV('Γ'), 0xD4}, // Gamma
    {IV('Δ'), 0xD5}, // Delta, ◿
    {IV('Θ'), 0xD6}, // Theta
    {IV('Λ'), 0xD7}, // Lambda
    {IV('Ξ'), 0xD8}, // Xi
    {IV('Π'), 0xD9}, // Pi
    {IV('Σ'), 0xDA}, // Sigma
    {IV('Υ'), 0xDB}, // Upsilon
    {IV('Φ'), 0xDC}, // Phi
    {IV('Ψ'), 0xDD}, // Psi
    {IV('Ω'), 0xDE}, // Omega
    {IV('α'), 0xDF}, // alpha

    {IV('β'), 0xE0}, // beta
    {IV('γ'), 0xE1}, // gamma
    {IV('δ'), 0xE2}, // delta
    {IV('ε'), 0xE3}, // epsilon
    {IV('ζ'), 0xE4}, // zeta
    {IV('η'), 0xE5}, // eta
    {IV('θ'), 0xE6}, // theta
    {IV('ι'), 0xE7}, // lota
    {IV('κ'), 0xE8}, // kappa
    {IV('λ'), 0xE9}, // lambda
    {IV('μ'), 0xEA}, // mu
    {IV('ν'), 0xEB}, // nu
    {IV('ξ'), 0xEC}, // xi
    {IV('π'), 0xED}, // pi
    {IV('ρ'), 0xEE}, // rho
    {IV('σ'), 0xEF}, // sigma

    {IV('τ'), 0xF0}, // tau
    {IV('υ'), 0xF1}, // upsilon
    {IV('χ'), 0xF2}, // chi
    {IV('ψ'), 0xF3}, // psi
    {IV('ω'), 0xF4}, // omega

    {IV('•'), 0xCD}, // ·
    {IV('℞'), 0xA7}, // ℞ Pt ASCII 158
    {IV('™'), 0xD0},
    {IV('↤'), 0xF9}, // ⟻
    {IV('↵'), 0xC4},
    {IV('⇥'), 0xFB},
    {IV('√'), 0xBE}, // √
    {IV('∞'), 0xC2}, // infinity
    {IV('∫'), 0x1B},
    {IV('∼'), 0x1D},
    {IV('≈'), 0x1A},
    {IV('≠'), 0xBD},
    {IV('≡'), 0x11},
    {IV('≤'), 0xB9},// ≤≥ ⩽⩾
    {IV('≥'), 0xBA},
    //{IV(''), 0xBF}, // error

    {IV('⌠'), 0xC0},
    {IV('⌡'), 0xC1},

    {IV('⎧'), 0x14},
    {IV('⎩'), 0x15},
    {IV('⎫'), 0x16},
    {IV('⎭'), 0x17},
    {IV('⎰'), 0x18},
    {IV('⎱'), 0x19},
    {IV('⎲'), 0x12},
    {IV('⎳'), 0x13},

    {IV('┌'), 0xC9},
    {IV('┐'), 0xCA},
    {IV('└'), 0xCB},
    {IV('┘'), 0xCC},
    {IV('◸'), 0xC3}, // ◿
    {IV('⭠'), 0xC8},
    {IV('⭡'), 0xC5},
    {IV('⭢'), 0xC7},
    {IV('⭣'), 0xC6},


    {IV('⯆'), 0xF5},
    {IV('⯇'), 0xF7}, // ⯅
    {IV('⯈'), 0xF6},
    //{IV(''), 0xF8}, // error
    //{IV(''), 0xFA}, // error
    //{IV(''), 0xFC}, // error
    //{IV(''), 0xFD}, // error
    //{IV(''), 0xFE}, // error
    //{IV(''), 0xFF}, // error

#elif DISPLAY_CHARSET_HD44780 == CYRILLIC
    //{IV(''), 0x80},
    //{IV(''), 0x81},
    //{IV(''), 0x82},
    //{IV(''), 0x83},
    //{IV(''), 0x84},
    //{IV(''), 0x85},
    //{IV(''), 0x86},
    //{IV(''), 0x87},
    //{IV(''), 0x88},
    //{IV(''), 0x89},
    //{IV(''), 0x8A},
    //{IV(''), 0x8B},
    //{IV(''), 0x8C},
    //{IV(''), 0x8D},
    //{IV(''), 0x8E},
    //{IV(''), 0x8F},

    //{IV(''), 0x90},
    //{IV(''), 0x91},
    //{IV(''), 0x92},
    //{IV(''), 0x93},
    //{IV(''), 0x94},
    //{IV(''), 0x95},
    //{IV(''), 0x96},
    //{IV(''), 0x97},
    //{IV(''), 0x98},
    //{IV(''), 0x99},
    //{IV(''), 0x9A},
    //{IV(''), 0x9B},
    //{IV(''), 0x9C},
    //{IV(''), 0x9D},
    //{IV(''), 0x9E},
    //{IV(''), 0x9F},

    {IV('Ё'), 0xA2},
    {IV('Б'), 0xA0},
    {IV('Г'), 0xA1},
    {IV('Д'), 0xE0},
    {IV('Ж'), 0xA3},
    {IV('З'), 0xA4},
    {IV('И'), 0xA5},
    {IV('Й'), 0xA6},
    {IV('Л'), 0xA7},
    {IV('П'), 0xA8},
    {IV('У'), 0xA9},
    {IV('Ф'), 0xAA},
    {IV('Ц'), 0xE1},
    {IV('Ч'), 0xAB},
    {IV('Ш'), 0xAC},
    {IV('Щ'), 0xE2},
    {IV('Ъ'), 0xAD},
    {IV('Ы'), 0xAE},
    {IV('Э'), 0xAF},

    {IV('Ю'), 0xB0},
    {IV('Я'), 0xB1},
    {IV('б'), 0xB2},
    {IV('в'), 0xB3},
    {IV('г'), 0xB4},
    {IV('д'), 0xE3},
    {IV('ж'), 0xB6},
    {IV('з'), 0xB7},
    {IV('и'), 0xB8},
    {IV('й'), 0xB9},
    {IV('к'), 0xBA}, //клмноп
    {IV('л'), 0xBB},
    {IV('м'), 0xBC},
    {IV('н'), 0xBD},
    {IV('п'), 0xBE},
    {IV('т'), 0xBF},

    {IV('у'), 0xC0},
    {IV('ф'), 0xE4},
    {IV('ц'), 0xE5},
    {IV('ш'), 0xC1},
    {IV('щ'), 0xE6},
    {IV('ъ'), 0xC2},
    {IV('ы'), 0xC3},
    {IV('ь'), 0xC4},
    {IV('э'), 0xC5},
    {IV('ю'), 0xC6},
    {IV('я'), 0xC7},
    {IV('ё'), 0xB5},
    //{IV(''), 0xC8},
    //{IV(''), 0xC9},
    //{IV(''), 0xCA},
    //{IV(''), 0xCB},
    //{IV(''), 0xCC},
    //{IV(''), 0xCD},
    //{IV(''), 0xCE},
    //{IV(''), 0xCF},

    //{IV(''), 0xD0},
    //{IV(''), 0xD1},
    //{IV(''), 0xD2},
    //{IV(''), 0xD3},
    //{IV(''), 0xD4},
    //{IV(''), 0xD5},
    //{IV(''), 0xD6},
    //{IV(''), 0xD7},
    //{IV(''), 0xD8},
    //{IV(''), 0xD9},
    //{IV(''), 0xDA},
    //{IV(''), 0xDB},
    //{IV(''), 0xDC},
    //{IV(''), 0xDD},
    //{IV(''), 0xDE},
    //{IV(''), 0xDF},

    //{IV(''), 0xE7},
    //{IV(''), 0xE8},
    //{IV(''), 0xE9},
    //{IV(''), 0xEA},
    //{IV(''), 0xEB},
    //{IV(''), 0xEC},
    //{IV(''), 0xED},
    //{IV(''), 0xEE},
    //{IV(''), 0xEF},

    //{IV(''), 0xF0},
    //{IV(''), 0xF1},
    //{IV(''), 0xF2},
    //{IV(''), 0xF3},
    //{IV(''), 0xF4},
    //{IV(''), 0xF5},
    //{IV(''), 0xF6},
    //{IV(''), 0xF7},
    //{IV(''), 0xF8},
    //{IV(''), 0xF9},
    //{IV(''), 0xFA},
    //{IV(''), 0xFB},
    //{IV(''), 0xFC},
    //{IV(''), 0xFD},
    //{IV(''), 0xFE},
    //{IV(''), 0xFF},

#endif
};

void
test_show_uchar()
{
    wchar_t pre = 0;
    hd44780_charmap_t cur;
    int i;

    for (i = 0; i < NUM_TYPE(g_hd44780_charmap); i ++) {
        memcpy_P (&cur, g_hd44780_charmap + i, sizeof(cur));
        //fprintf (stdout, "[% 2d] 0x%04X, 0x%02X%s\n", i, cur.uchar, (int)(cur.idx), (pre < cur.uchar?"":" <--- ERROR"));
#if 1
        TRACE("[% 2d] 0x%04X, 0x%02X%s", i, cur.uchar, (unsigned int)(cur.idx), (pre < cur.uchar?"":" <--- ERROR"));
#else
        TRACE("[% 2d]", i);
        TRACE("0x%04X,", cur.uchar);
        TRACE("0x%02X,", (unsigned int)(cur.idx));
        TRACE("%s", (pre < cur.uchar?"":" <--- ERROR"));
#endif
        pre = cur.uchar;
    }
}

void
lcd_moveto (int col, int row)
{
    //lcd.setCursor(col, row);
}

/* return v1 - v2 */
static int
hd44780_charmap_compare (hd44780_charmap_t * v1, hd44780_charmap_t * v2)
{
    assert (NULL != v1);
    assert (NULL != v2);
    TRACE ("compare char1(0x%X)", v1->uchar);
    TRACE ("compare char2(0x%X)", v2->uchar);
    if (v1->uchar < v2->uchar) {
        TRACE ("compare return -1");
        return -1;
    } else if (v1->uchar > v2->uchar) {
        TRACE ("compare return 1");
        return 1;
    }
#if 0
    if (v1->idx < v2->idx) {
        return -1;
    } else if (v1->idx > v2->idx) {
        return 1;
    }
#endif
    TRACE ("compare return 0");
    return 0;
}

static int
pf_bsearch_cb_comp_hd4map_pgm (void *userdata, size_t idx, void * data_pin)
{
    hd44780_charmap_t localval;
    hd44780_charmap_t *p_hd44780_charmap = (hd44780_charmap_t *)userdata;
    memcpy_P (&localval, p_hd44780_charmap + idx, sizeof (localval));
    return hd44780_charmap_compare (&localval, (hd44780_charmap_t *)data_pin);
}

// return < 0 on error
// return the advanced cols
int
lcd_print_uchar (wchar_t c)
{
    // find the HD44780 internal ROM first
    size_t idx = 0;
    hd44780_charmap_t pinval;
    hd44780_charmap_t localval;
    pinval.uchar = c;
    pinval.idx = -1;

    // TODO: fix the '\\' that dont exist in the HD44870
    if (c < 128) {
        TRACE ("draw char: regular %d", (int)c);
        _lcd_write  ((uint8_t)c);
        return 1;
    }
    if (pf_bsearch_r ((void *)g_hd44780_charmap, NUM_TYPE(g_hd44780_charmap), pf_bsearch_cb_comp_hd4map_pgm, (void *)&pinval, &idx) >= 0) {
        // found
        memcpy_P (&localval, g_hd44780_charmap + idx, sizeof (localval));
        assert ((localval.uchar == c) && (localval.uchar == pinval.uchar));
        TRACE ("draw char: %d at ROM %d", (int)c, (int)localval.idx);
        _lcd_write  (localval.idx);
        return 1;
    }
    // print '?' instead
    TRACE ("draw char: Not found %d (0x%X", (int)c, (int)c);
    _lcd_write  ((uint8_t)'?');
    return 0;
}

/**
 * @brief Draw a UTF-8 string
 *
 * @param utf8_str : the UTF-8 string
 * @param len : the byte length of the string (returned by strlen(utf8_str) or strlen_P(utf8_str) )
 * @param cb_read_byte : the callback function to read one byte from the utf8_str (from RAM or ROM)
 * @param max_length : the pixel length of the string allowed (or number of slots in HD44780)
 *
 * @return the avanced pixels
 *
 * Draw a UTF-8 string
 */
static int
lcd_printstr_cb (const char * utf8_str, uint16_t len, uint8_t (*cb_read_byte)(uint8_t * str), pixel_len_t max_length)
{
    wchar_t ch;
    uint8_t *p;
    uint8_t *pend;
    int ret = 0;

    TRACE ("BEGIN lcd_printstr_cb(len=%d, maxlen=%d)", len, max_length);
    pend = (uint8_t *)utf8_str + len;
    for (p = (uint8_t *)utf8_str; p < pend; ) {
        if (ret >= max_length) {
            TRACE ("> max_lenght, quit");
            break;
        }
        ch = 0;
        p = get_utf8_value_cb (p, cb_read_byte, &ch);
        if (NULL == p) {
            TRACE("No more char, break ...");
            break;
        }
        ret += lcd_print_uchar (ch);
    }
    return ret;
}

int
lcd_printstr (const char * utf8_str, pixel_len_t max_length)
{
    //TRACE ("BEGIN lcd_printstr(str='%s', len=%d, maxlen=%d)", utf8_str, strlen(utf8_str), max_length);
    TRACE ("BEGIN lcd_printstr(str='%s')", utf8_str);
    TRACE ("BEGIN lcd_printstr('len=%d)", strlen(utf8_str));
    TRACE ("BEGIN lcd_printstr(maxlen=%d)", max_length);
    return lcd_printstr_cb(utf8_str, strlen(utf8_str), read_byte_ram, max_length);
}

int
lcd_printstr_P (const char * utf8_str_P, pixel_len_t max_length)
{
    //TRACE ("BEGIN lcd_printstr_P('%s', len=%d, maxlen=%d)", utf8_str_P, strlen_P(utf8_str_P), max_length);
    TRACE ("BEGIN lcd_printstr_P(len=%d)", strlen_P(utf8_str_P));
    TRACE ("BEGIN lcd_printstr_P(maxlen=%d)", max_length);
    return lcd_printstr_cb(utf8_str_P, strlen_P(utf8_str_P), read_byte_rom, max_length);
}

#endif // USE_HD44780
