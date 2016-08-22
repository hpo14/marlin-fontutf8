/**
 * @file    fontutils.h
 * @brief   help functions for font and char
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @date    2016-08-19
 * @copyright GPL/BSD
 */
#ifndef _FONT_UTILS_H
#define _FONT_UTILS_H

#if ! defined(__AVR__)
#include <string.h>
//#define pgm_read_word_near(a) *((uint16_t *)(a))
#define pgm_read_word_near(a) (*(a))
#define pgm_read_byte_near(a) *((uint8_t *)(a))
#define strlen_P strlen
#define memcpy_P memcpy
#define PROGMEM

inline uint8_t
pgm_read_byte(uint8_t * str)
{
    return *str;
}
#else
#include <avr/pgmspace.h>
#endif

// read a byte from ROM or RAM
typedef uint8_t (* read_byte_cb_t)(uint8_t * str);

inline uint8_t
read_byte_ram(uint8_t * str)
{
    return *str;
}
#define read_byte_rom pgm_read_byte


#if defined(ARDUINO)
// there's overflow of the wchar_t due to the 2-byte size in Arduino
// sizeof(wchar_t)=2; sizeof(size_t)=2; sizeof(uint32_t)=4;
// sizeof(int)=2; sizeof(long)=4; sizeof(unsigned)=2;
#define wchar_t uint32_t
#else
#include <sys/types.h> // ssize_t
#include <stdint.h>
#include <assert.h>
// x86_64
// sizeof(wchar_t)=4; sizeof(size_t)=8; sizeof(uint32_t)=4;
// sizeof(int)=4; sizeof(long)=8; sizeof(unsigned)=4;
//#define wchar_t uint32_t
#define wchar_t size_t
#endif


//#define pixel_len_t u8g_uint_t
#define pixel_len_t uint16_t
//#define pixel_len_t uint8_t
#define PIXEL_LEN_NOLIMIT ((pixel_len_t)(-1))


#define UNUSED_VARIABLE(a) ((void)(a))
#define NUM_TYPE(a) (sizeof(a)/sizeof((a)[0]))
#define MIN(a,b) (((a)>(b))?(b):(a))

#ifdef __cplusplus
extern "C" {
#endif

typedef int (* pf_bsearch_cb_comp_t)(void *userdata, size_t idx, void * data_pin); /*"data_list[idx] - *data_pin"*/
int pf_bsearch_r (void *userdata, size_t num_data, pf_bsearch_cb_comp_t cb_comp, void *data_pinpoint, size_t *ret_idx);

//wchar_t get_val_utf82uni (uint8_t *pstart);
//uint8_t * get_utf8_value (uint8_t *pstart, wchar_t *pval);
//uint8_t * get_utf8_value_cb (uint8_t *pstart, uint8_t (*cb_read_byte)(uint8_t * str), wchar_t *pval);
uint8_t * get_utf8_value_cb (uint8_t *pstart, read_byte_cb_t cb_read_byte, wchar_t *pval);

#ifdef __cplusplus
}
#endif

#endif // _FONT_UTILS_H
