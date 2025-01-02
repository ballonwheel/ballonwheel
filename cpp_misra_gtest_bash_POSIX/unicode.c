/**
 * @file shared/unicode.c
 * @brief Unicode string manipulation.
 *
 * Copyright (C) 2010 Dension Audio Systems Kft.
 *
 * @addtogroup shared
 * @{
 */

#include <stdio.h>
#include <string.h>
#include "unicode.h"

/**
 * Get next UTF-8 character
 *
 * @param utf8_poi Pointer to utf8 string, will be incremented with character length
 *
 * @return Character value, 0 - 0xffff
 */
static uint16_t utf8_getchar(const uint8_t **utf8_poi)
{
    int nn = 1;
    uint16_t c, mask;
    const uint8_t *utf8 = *utf8_poi;

    c = *utf8;
    if (c & 0x80)
    {
        mask = 0x40; // start from here, mask 0x80 is already tested
        while (c & mask)
        { // nn = count of 1's
            if ((*(utf8 + nn) & 0xc0) != 0x80)
            { // utf8 format error
                nn = 1;
                break;
            };
            nn++;
            mask >>= 1;
        };
    };
    switch (nn)
    {
        case 1:
            break; // don't convert, single ascii char
        case 2:
            c = ((*utf8 & 0x1f) << 6) + (*(utf8 + 1) & 0x3f);
            break;
        case 3:
            c = ((*utf8 & 0xf) << 12) + ((*(utf8 + 1) & 0x3f) << 6) + (*(utf8 + 2) & 0x3f);
            break;
        default: // value larger than 0xffff
            c = (uint16_t) CHAR_UNKNOWN;
    };
    *utf8_poi += nn * sizeof(uint8_t);

    return(c);
}

/**
 * UTF-8 to UTF-16 conversion
 *
 * @param utf16 output buffer
 * @param utf8  input buffer
 * @param charnum Maximum number of characters to copy
 *
 * @return Number of characters copied
 */
int utf8_to_utf16(uint16_t *utf16, const uint8_t *utf8, int charnum)
{
    uint16_t c;
    int len = 0;

    while (charnum--)
    {
        c = utf8_getchar(&utf8);
        if (c == 0)
            break;
        *utf16++ = c;
        len++;
    };

    *utf16 = 0; // \0
    len++;

    return(len);
}

/**
 * UTF-8 to ASCII (latin) conversion.
 *
 * @param charnum Maximum length -1 of latin
 *
 * @return Number of characters copied
 */
int utf8_to_latin (char * latin, const uint8_t * utf8, int charnum)
{
    uint16_t c;
    int len = 0;

    while (charnum--)
    {
        c = utf8_getchar(&utf8);
        if (c == 0)
            break;
        if (c > 0xff)
            c = (uint16_t) CHAR_UNKNOWN;
        *latin++ = (char) c;
        len++;
    };
    *latin = '\0';
    return len;
}

/**
 * List of reserved FAT characters
 *
 * '/' is missing, it is used for identifying path parts here!
 */
static char fat_reserved[] = { '<', '>', ':', '"', '\\', '|', '?', '*', '\0' };

/**
 * Checks that the charactes is reservd or not by FAT
 *
 * @param c Character to examine
 *
 * @return non-zero if reserved
 */
static int _fat_is_reserved(char c)
{
    int i;
    char r;

    for (r = fat_reserved[i = 0]; r != '\0'; r = fat_reserved[i++]) {
        if (c == r)
            return 1;
    }

    return 0;
}

/**
 * UTF-8 to UTF-16 conversion
 *
 * Convert reserved FAT characters to '_'
 *
 * @param utf16 output buffer
 * @param utf8  input buffer
 * @param charnum Maximum number of characters to copy
 *
 * @return Number of characters copied
 */
int to_FAT(uint16_t *utf16, const uint8_t *utf8, int charnum)
{
    int i;
    uint8_t fat[charnum + 1];

    for (i = 0; i < charnum; i++) {
        if (_fat_is_reserved((char)utf8[i])) {
            fat[i] = (uint8_t)CHAR_UNKNOWN;
        } else {
            fat[i] = utf8[i];
        }
    }
    fat[charnum] = 0;

    return utf8_to_utf16(utf16, fat, charnum);
}

/**
 * Prints out utf16 strint to buf
 *
 * @param utf16 input string
 * @param buf output buffer
 * @param blen buf length
 */
void print_utf16(const uint16_t * const utf16, char * buf, size_t blen)
{
    int i;
    uint16_t c;
    char tmp[5];

    buf[0] = '\0';
    for (c = utf16[i = 0]; c != 0x0000; c = utf16[++i]) {
        (void)snprintf(tmp, 5, "%c", (char)c);
        strcat(buf, tmp);
        blen -= strlen(tmp);
        if (blen < 2) {
            return;
        }
    }
}

/**
 * Calculates the length of a UTF16 string.
 *
 * @return The length in bytes not including the terminating 0x0000.
 */
uint32_t strlen_utf16 (const uint16_t * string)
{
    uint32_t len = 0;
    const uint16_t * ptr = string;

    while (*ptr != 0x0000) {
        ++len;
        ++ptr;
    }
    return len;
}

/**
 * @}
 */
