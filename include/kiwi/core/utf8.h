#ifndef KIWI_CORE_UTF8_H
#define KIWI_CORE_UTF8_H

#include <stdint.h>
#include <stdlib.h>
#include "kiwi/core/core-export.h"

#ifdef __cplusplus
extern "C" {
#endif

/* is c the start of a utf8 sequence? */
#define isutf(c) (((c)&0xC0)!=0x80)

/* convert UTF-8 data to wide character */
KIWI_CORE_EXPORT int u8_toucs(uint32_t *dest, int sz, char *src, int srcsz);

/* the opposite conversion */
KIWI_CORE_EXPORT int u8_toutf8(char *dest, int sz, uint32_t *src, int srcsz);

/* single character to UTF-8 */
KIWI_CORE_EXPORT int u8_wc_toutf8(char *dest, uint32_t ch);

/* character number to byte offset */
KIWI_CORE_EXPORT int u8_offset(char *str, int charnum);

/* byte offset to character number */
KIWI_CORE_EXPORT int u8_charnum(char *s, int offset);

/* return next character, updating an index variable */
KIWI_CORE_EXPORT uint32_t u8_nextchar(const char *s, int *i);

/* move to next character */
KIWI_CORE_EXPORT void u8_inc(const char *s, int *i);

/* move to previous character */
KIWI_CORE_EXPORT void u8_dec(const char *s, int *i);

/* returns length of next utf-8 sequence */
KIWI_CORE_EXPORT int u8_seqlen(char c);

/* assuming src points to the character after a backslash, read an
   escape sequence, storing the result in dest and returning the number of
   input characters processed */
KIWI_CORE_EXPORT int u8_read_escape_sequence(char *src, uint32_t *dest);

/* convert a string "src" containing escape sequences to UTF-8 */
KIWI_CORE_EXPORT int u8_unescape(char *buf, int sz, char *src);

/* utility predicates used by the above */
KIWI_CORE_EXPORT int octal_digit(char c);
KIWI_CORE_EXPORT int hex_digit(char c);

/* return a pointer to the first occurrence of ch in s, or NULL if not
   found. character index of found character returned in *charn. */
KIWI_CORE_EXPORT char *u8_strchr(char *s, uint32_t ch, int *charn);

/* same as the above, but searches a buffer of a given size instead of
   a NUL-terminated string. */
KIWI_CORE_EXPORT char *u8_memchr(char *s, uint32_t ch, size_t sz, int *charn);

/* count the number of characters in a UTF-8 string */
KIWI_CORE_EXPORT int u8_strlen(const char *s);

KIWI_CORE_EXPORT int u8_is_locale_utf8(char *locale);

#ifdef __cplusplus
}
#endif

#endif
