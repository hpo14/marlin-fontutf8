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

static const hd44780_charmap_t g_hd44780_charmap[] PROGMEM = {
    // sorted by uchar:
#if DISPLAY_CHARSET_HD44780 == JAPANESE
    //{'', 0xA0},
    {'。', 0xA1},
    {'「', 0xA2},
    {'」', 0xA3},
    {'ヽ', 0xA4},
    {'・', 0xA5},
    {'ヲ', 0xA6},
    {'ァ', 0xA7},
    {'ィ', 0xA8},
    {'ゥ', 0xA9},
    {'ェ', 0xAA},
    {'ォ', 0xAB},
    {'ャ', 0xAC},
    {'ュ', 0xAD},
    {'ョ', 0xAE},
    {'ッ', 0xAF},

    {'ー', 0xB0},
    {'ア', 0xB1},
    {'イ', 0xB2},
    {'ウ', 0xB3},
    {'エ', 0xB4},
    {'オ', 0xB5},
    {'カ', 0xB6},
    {'キ', 0xB7},
    {'ク', 0xB8},
    {'ケ', 0xB9},
    {'コ', 0xBA},
    {'サ', 0xBB},
    {'シ', 0xBC},
    {'ス', 0xBD},
    {'セ', 0xBE},
    {'ソ', 0xBF},

    {'タ', 0xC0},
    {'チ', 0xC1},
    {'ツ', 0xC2},
    {'テ', 0xC3},
    {'ト', 0xC4},
    {'ナ', 0xC5},
    {'ニ', 0xC6},
    {'ヌ', 0xC7},
    {'ネ', 0xC8},
    {'ノ', 0xC9},
    {'ハ', 0xCA},
    {'ヒ', 0xCB},
    {'フ', 0xCC},
    {'ヘ', 0xCD},
    {'ホ', 0xCE},
    {'マ', 0xCF},

    {'ミ', 0xD0},
    {'ム', 0xD1},
    {'メ', 0xD2},
    {'モ', 0xD3},
    {'ヤ', 0xD4},
    {'ユ', 0xD5},
    {'ヨ', 0xD6},
    {'ラ', 0xD7},
    {'リ', 0xD8},
    {'ル', 0xD9},
    {'レ', 0xDA},
    {'ロ', 0xDB},
    {'ワ', 0xDC},
    {'ン', 0xDD},
    {'〝', 0xDE},
    {'゜', 0xDF},

    {'α', 0xE0},
    {'ä', 0xE1},
    {'β', 0xE2},
    {'ε', 0xE3}, // epsilon
    {'μ', 0xE4}, // mu
    {'σ', 0xE5}, // sigma
    {'ρ', 0xE6}, // rho
    //{'g', 0xE7}, // error
    {'√', 0xE8},
    //{'', 0xE9},
    //{'j', 0xEA}, // error
    {'ˣ', 0xEB},
    {'¢', 0xEC},
    //{'', 0xED},
    //{'', 0xEE},
    {'ö', 0xEF},

    //{'p', 0xF0}, // error
    //{'q', 0xF1}, // error
    {'Θ', 0xF2},
    {'∞', 0xF3},
    {'Ω', 0xF4},
    {'ü', 0xF5},
    {'Σ', 0xF6},
    {'π', 0xF7},
    //{'', 0xF8},
    {'y', 0xF9}, // error
    {'千', 0xFA},
    {'万', 0xFB}, 
    {'円', 0xFC},
    {'÷', 0xFD},
    //{'', 0xFE},
    {'█', 0xFF},

#elif DISPLAY_CHARSET_HD44780 == WESTERN
// 0x10 -- 0x1F (except 0x1C)
// 0x80 -- 0xFF (except 0xA7,0xB0,0xB1,0xB3,0xB4,0xBF,0xD1,0xF8,0xFA,0xFC-0xFF)

    {'¡', 0xA9},
    {'¢', 0xA4},
    {'£', 0xA5},
    {'¥', 0xA6},
    {'§', 0xD2}, // section sign
    {'©', 0xCF},

    {'ª', 0x9D},
    {'«', 0xBB},
    {'®', 0xCE},

    {'°', 0xB2},
    //{'', 0xD1},
    {'±', 0x10}, //∓±
    //{'=', 0x1C}, // error
    {'²', 0x1E},
    {'³', 0x1F},
    {'¶', 0xD3}, // pilcrow sign
    {'º', 0x9E},
    {'»', 0xBC},
    //{'', 0xB3}, // error
    //{'', 0xB4}, // error
    {'¼', 0xB6},
    {'½', 0xB5},
    {'¿', 0x9F},

    {'Â', 0x8F},
    {'Ã', 0xAA},
    {'Ä', 0x8E},
    {'Æ', 0x92},
    {'Ç', 0x80},
    {'É', 0x90},
    {'Ñ', 0x9C},
    {'Õ', 0xAC},
    {'Ö', 0x99},
    {'×', 0xB7},
    {'Ø', 0xAE},
    {'Ü', 0x9A},
    {'à', 0x85},
    {'á', 0xA0},
    {'â', 0x83},
    {'ã', 0xAB},
    {'ä', 0x84},
    {'å', 0x86},
    {'æ', 0x91},
    {'ç', 0x87},
    {'è', 0x8A},
    {'é', 0x82},
    {'ê', 0x88},
    {'ë', 0x89},
    {'ì', 0x8D},
    {'í', 0xA1},
    {'î', 0x8C},
    {'ï', 0x8B},

    {'ñ', 0x9B},
    {'ò', 0x95},
    {'ó', 0xA2},
    {'ô', 0x93},
    {'õ', 0xAD},
    {'ö', 0x94},
    {'÷', 0xB8},
    {'ø', 0xAF},
    {'ù', 0x97},
    {'ú', 0xA3},
    {'û', 0x96},
    {'ü', 0x81},
    {'ÿ', 0x98},


    //{'', 0xB0}, // error
    //{'', 0xB1}, // error
    {'ƒ', 0xA8},

    {'Γ', 0xD4}, // Gamma
    {'Δ', 0xD5}, // Delta, ◿
    {'Θ', 0xD6}, // Theta
    {'Λ', 0xD7}, // Lambda
    {'Ξ', 0xD8}, // Xi
    {'Π', 0xD9}, // Pi
    {'Σ', 0xDA}, // Sigma
    {'Υ', 0xDB}, // Upsilon
    {'Φ', 0xDC}, // Phi
    {'Ψ', 0xDD}, // Psi
    {'Ω', 0xDE}, // Omega
    {'α', 0xDF}, // alpha

    {'β', 0xE0}, // beta
    {'γ', 0xE1}, // gamma
    {'δ', 0xE2}, // delta
    {'ε', 0xE3}, // epsilon
    {'ζ', 0xE4}, // zeta
    {'η', 0xE5}, // eta
    {'θ', 0xE6}, // theta
    {'ι', 0xE7}, // lota
    {'κ', 0xE8}, // kappa
    {'λ', 0xE9}, // lambda
    {'μ', 0xEA}, // mu
    {'ν', 0xEB}, // nu
    {'ξ', 0xEC}, // xi
    {'π', 0xED}, // pi
    {'ρ', 0xEE}, // rho
    {'σ', 0xEF}, // sigma

    {'τ', 0xF0}, // tau
    {'υ', 0xF1}, // upsilon
    {'χ', 0xF2}, // chi
    {'ψ', 0xF3}, // psi
    {'ω', 0xF4}, // omega

    {'•', 0xCD}, // ·
    {'℞', 0xA7}, // ℞ Pt ASCII 158
    {'™', 0xD0},
    {'↤', 0xF9}, // ⟻
    {'↵', 0xC4},
    {'⇥', 0xFB},
    {'√', 0xBE}, // √
    {'∞', 0xC2}, // infinity
    {'∫', 0x1B},
    {'∼', 0x1D},
    {'≈', 0x1A},
    {'≠', 0xBD},
    {'≡', 0x11},
    {'≤', 0xB9},// ≤≥ ⩽⩾
    {'≥', 0xBA},
    //{'', 0xBF}, // error

    {'⌠', 0xC0},
    {'⌡', 0xC1},

    {'⎧', 0x14},
    {'⎩', 0x15},
    {'⎫', 0x16},
    {'⎭', 0x17},
    {'⎰', 0x18},
    {'⎱', 0x19},
    {'⎲', 0x12},
    {'⎳', 0x13},

    {'┌', 0xC9},
    {'┐', 0xCA},
    {'└', 0xCB},
    {'┘', 0xCC},
    {'◸', 0xC3}, // ◿
    {'⭠', 0xC8},
    {'⭡', 0xC5},
    {'⭢', 0xC7},
    {'⭣', 0xC6},


    {'⯆', 0xF5},
    {'⯇', 0xF7}, // ⯅
    {'⯈', 0xF6},
    //{'', 0xF8}, // error
    //{'', 0xFA}, // error
    //{'', 0xFC}, // error
    //{'', 0xFD}, // error
    //{'', 0xFE}, // error
    //{'', 0xFF}, // error

#elif DISPLAY_CHARSET_HD44780 == CYRILLIC
    //{'', 0x80},
    //{'', 0x81},
    //{'', 0x82},
    //{'', 0x83},
    //{'', 0x84},
    //{'', 0x85},
    //{'', 0x86},
    //{'', 0x87},
    //{'', 0x88},
    //{'', 0x89},
    //{'', 0x8A},
    //{'', 0x8B},
    //{'', 0x8C},
    //{'', 0x8D},
    //{'', 0x8E},
    //{'', 0x8F},

    //{'', 0x90},
    //{'', 0x91},
    //{'', 0x92},
    //{'', 0x93},
    //{'', 0x94},
    //{'', 0x95},
    //{'', 0x96},
    //{'', 0x97},
    //{'', 0x98},
    //{'', 0x99},
    //{'', 0x9A},
    //{'', 0x9B},
    //{'', 0x9C},
    //{'', 0x9D},
    //{'', 0x9E},
    //{'', 0x9F},

    {'Ё', 0xA2},
    {'Б', 0xA0},
    {'Г', 0xA1},
    {'Д', 0xE0},
    {'Ж', 0xA3},
    {'З', 0xA4},
    {'И', 0xA5},
    {'Й', 0xA6},
    {'Л', 0xA7},
    {'П', 0xA8},
    {'У', 0xA9},
    {'Ф', 0xAA},
    {'Ц', 0xE1},
    {'Ч', 0xAB},
    {'Ш', 0xAC},
    {'Щ', 0xE2},
    {'Ъ', 0xAD},
    {'Ы', 0xAE},
    {'Э', 0xAF},

    {'Ю', 0xB0},
    {'Я', 0xB1},
    {'б', 0xB2},
    {'в', 0xB3},
    {'г', 0xB4},
    {'д', 0xE3},
    {'ж', 0xB6},
    {'з', 0xB7},
    {'и', 0xB8},
    {'й', 0xB9},
    {'к', 0xBA}, //клмноп
    {'л', 0xBB},
    {'м', 0xBC},
    {'н', 0xBD},
    {'п', 0xBE},
    {'т', 0xBF},

    {'у', 0xC0},
    {'ф', 0xE4},
    {'ц', 0xE5},
    {'ш', 0xC1},
    {'щ', 0xE6},
    {'ъ', 0xC2},
    {'ы', 0xC3},
    {'ь', 0xC4},
    {'э', 0xC5},
    {'ю', 0xC6},
    {'я', 0xC7},
    {'ё', 0xB5},
    //{'', 0xC8},
    //{'', 0xC9},
    //{'', 0xCA},
    //{'', 0xCB},
    //{'', 0xCC},
    //{'', 0xCD},
    //{'', 0xCE},
    //{'', 0xCF},

    //{'', 0xD0},
    //{'', 0xD1},
    //{'', 0xD2},
    //{'', 0xD3},
    //{'', 0xD4},
    //{'', 0xD5},
    //{'', 0xD6},
    //{'', 0xD7},
    //{'', 0xD8},
    //{'', 0xD9},
    //{'', 0xDA},
    //{'', 0xDB},
    //{'', 0xDC},
    //{'', 0xDD},
    //{'', 0xDE},
    //{'', 0xDF},

    //{'', 0xE7},
    //{'', 0xE8},
    //{'', 0xE9},
    //{'', 0xEA},
    //{'', 0xEB},
    //{'', 0xEC},
    //{'', 0xED},
    //{'', 0xEE},
    //{'', 0xEF},

    //{'', 0xF0},
    //{'', 0xF1},
    //{'', 0xF2},
    //{'', 0xF3},
    //{'', 0xF4},
    //{'', 0xF5},
    //{'', 0xF6},
    //{'', 0xF7},
    //{'', 0xF8},
    //{'', 0xF9},
    //{'', 0xFA},
    //{'', 0xFB},
    //{'', 0xFC},
    //{'', 0xFD},
    //{'', 0xFE},
    //{'', 0xFF},

#endif
};


void
test_show_uchar()
{
#if ! defined(ARDUINO)
    wchar_t pre = 0;
    int i;

    for (i = 0; i < NUM_TYPE(g_hd44780_charmap); i ++) {
        fprintf (stdout, "[% 2d] 0x%04X, 0x%02X%s\n", i, g_hd44780_charmap[i].uchar, (int)(g_hd44780_charmap[i].idx), (pre < g_hd44780_charmap[i].uchar?"":" <--- ERROR"));
        pre = g_hd44780_charmap[i].uchar;
    }
#endif
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
    if (v1->uchar < v2->uchar) {
        return -1;
    } else if (v1->uchar > v2->uchar) {
        return 1;
    }
#if 0
    if (v1->idx < v2->idx) {
        return -1;
    } else if (v1->idx > v2->idx) {
        return 1;
    }
#endif
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
        _lcd_write  ((uint8_t)c);
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
    return lcd_printstr_cb(utf8_str, strlen(utf8_str), read_byte_ram, max_length);
}

int
lcd_printstr_P (const char * utf8_str_P, pixel_len_t max_length)
{
    return lcd_printstr_cb(utf8_str_P, strlen_P(utf8_str_P), read_byte_rom, max_length);
}

#endif // USE_HD44780
