//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                               License Agreement
//                Open Source C Container Library like STL in C++
//
//               Copyright (C) 2026, Kim Bomm, all rights reserved.
//
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of the copyright holders may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//

#ifndef OPENCSTL_STRING_H
#define OPENCSTL_STRING_H

#include "zalloc.h"
#include "verify.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include<stdbool.h>
// ============================================================
// string function implementations
// ============================================================

static char *__cstl_string_substr(char *src, int pos, int len) {
    verify(strlen(src) >= pos + len);
    char *ret = (char *) calloc(len + 1, sizeof(char));
    memcpy(ret, src + pos, len);
    return ret;
}

char **__cstl_string_split(const char *src, const char *sep, int *n) {
    int len = (int) strlen(src);
    int sep_len = (int) strlen(sep);

    if (sep_len == 0) {
        char **ret = (char **) malloc(sizeof(char *) + (len + 1));
        char *buf = (char *) ret + sizeof(char *);
        memcpy(buf, src, len + 1);
        ret[0] = buf;
        *n = 1;
        return ret;
    }

    // separator 개수 세기
    int count = 1;
    const char *p = src;
    while ((p = strstr(p, sep)) != NULL) {
        count++;
        p += sep_len;
    }

    char **ret = (char **) malloc(count * sizeof(char *) + (len + 1));
    char *buf = (char *) ret + count * sizeof(char *);
    memcpy(buf, src, len + 1);


    int idx = 0;
    ret[idx++] = buf;
    char *s = buf;
    while ((s = strstr(s, sep)) != NULL) {
        *s = '\0';
        s += sep_len;
        ret[idx++] = s;
    }

    *n = count;
    return ret;
}


char *__cstl_string_replace(char *src, char *from, char *to) {
    int len = strlen(src);
    int from_len = strlen(from);
    int to_len = strlen(to);

    int count = 0;
    char *p = src;
    while ((p = strstr(p, from)) != NULL) {
        count++;
        p += from_len;
    }

    char *ret = (char *) calloc(len + count * (to_len - from_len) + 1, 1);
    char *dst = ret;
    p = src;
    while (*p) {
        if (strncmp(p, from, from_len) == 0) {
            memcpy(dst, to, to_len);
            dst += to_len;
            p += from_len;
        } else {
            *dst++ = *p++;
        }
    }
    *dst = '\0';

    return ret;
}

char *__cstl_string_ltrim(const char *src) {
    const char *p = src;
    while (*p && isspace((unsigned char) *p)) p++;
    size_t len = strlen(p);
    char *ret = (char *) malloc(len + 1);
    memcpy(ret, p, len + 1);
    return ret;
}

char *__cstl_string_rtrim(const char *src) {
    size_t len = strlen(src);
    while (len > 0 && isspace((unsigned char) src[len - 1])) len--;
    char *ret = (char *) malloc(len + 1);
    memcpy(ret, src, len);
    ret[len] = '\0';
    return ret;
}

char *__cstl_string_trim(const char *src) {
    const char *begin = src;
    while (*begin && isspace((unsigned char) *begin)) begin++;
    const char *end = src + strlen(src);
    while (end > begin && isspace((unsigned char) *(end - 1))) end--;
    size_t len = end - begin;
    char *ret = (char *) malloc(len + 1);
    memcpy(ret, begin, len);
    ret[len] = '\0';
    return ret;
}

char *__cstl_string_to_upper(const char *src) {
    size_t len = strlen(src);
    char *ret = (char *) malloc(len + 1);
    for (size_t i = 0; i < len; i++) ret[i] = (char) toupper((unsigned char) src[i]);
    ret[len] = '\0';
    return ret;
}

char *__cstl_string_to_lower(const char *src) {
    size_t len = strlen(src);
    char *ret = (char *) malloc(len + 1);
    for (size_t i = 0; i < len; i++) ret[i] = (char) tolower((unsigned char) src[i]);
    ret[len] = '\0';
    return ret;
}

bool __cstl_string_starts_with(const char *src, const char *prefix) {
    size_t plen = strlen(prefix);
    return strncmp(src, prefix, plen) == 0;
}

bool __cstl_string_ends_with(const char *src, const char *suffix) {
    size_t slen = strlen(src);
    size_t flen = strlen(suffix);
    if (flen > slen) return false;
    return memcmp(src + slen - flen, suffix, flen) == 0;
}

int __cstl_string_count(const char *src, const char *sub) {
    int count = 0;
    size_t sub_len = strlen(sub);
    if (sub_len == 0) return 0;
    const char *p = src;
    while ((p = strstr(p, sub)) != NULL) {
        count++;
        p += sub_len;
    }
    return count;
}

char *__cstl_string_join(char **parts, int n, const char *delim) {
    if (n <= 0) {
        char *empty = (char *) malloc(1);
        *empty = '\0';
        return empty;
    }
    size_t dlen = strlen(delim);
    size_t total = 0;
    for (int i = 0; i < n; i++) total += strlen(parts[i]);
    total += dlen * (n - 1) + 1;

    char *ret = (char *) malloc(total);
    char *dst = ret;
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            memcpy(dst, delim, dlen);
            dst += dlen;
        }
        size_t len = strlen(parts[i]);
        memcpy(dst, parts[i], len);
        dst += len;
    }
    *dst = '\0';
    return ret;
}

char *__cstl_string_concat(const char *a, const char *b) {
    size_t alen = strlen(a);
    size_t blen = strlen(b);
    char *ret = (char *) malloc(alen + blen + 1);
    memcpy(ret, a, alen);
    memcpy(ret + alen, b, blen + 1);
    return ret;
}

char *__cstl_string_reverse(const char *src) {
    size_t len = strlen(src);
    char *ret = (char *) malloc(len + 1);
    for (size_t i = 0; i < len; i++) ret[i] = src[len - 1 - i];
    ret[len] = '\0';
    return ret;
}

bool __cstl_string_is_digit(const char *src) {
    if (!*src) return false;
    for (const char *p = src; *p; p++)
        if (!isdigit((unsigned char) *p)) return false;
    return true;
}

bool __cstl_string_is_alpha(const char *src) {
    if (!*src) return false;
    for (const char *p = src; *p; p++)
        if (!isalpha((unsigned char) *p)) return false;
    return true;
}

bool __cstl_string_is_alnum(const char *src) {
    if (!*src) return false;
    for (const char *p = src; *p; p++)
        if (!isalnum((unsigned char) *p)) return false;
    return true;
}

bool __cstl_string_is_space(const char *src) {
    if (!*src) return false;
    for (const char *p = src; *p; p++)
        if (!isspace((unsigned char) *p)) return false;
    return true;
}



// ============================================================
// function pointer types
// ============================================================
typedef char *(*string_substr_fn)(char *, int, int);

typedef char **(*string_split_fn)(const char *, const char *, int *);

typedef char *(*string_replace_fn)(char *, char *, char *);

typedef char *(*string_unary_fn)(const char *);

typedef bool (*string_match_fn)(const char *, const char *);

typedef int (*string_count_fn)(const char *, const char *);

typedef char *(*string_join_fn)(char **, int, const char *);

typedef char *(*string_concat_fn)(const char *, const char *);

typedef bool (*string_pred_fn)(const char *);

// ============================================================
// string namespace struct
// ============================================================
typedef struct {
    string_substr_fn substr;
    string_split_fn split;
    string_replace_fn replace;
    string_unary_fn ltrim;
    string_unary_fn rtrim;
    string_unary_fn trim;
    string_unary_fn to_upper;
    string_unary_fn to_lower;
    string_match_fn starts_with;
    string_match_fn ends_with;
    string_count_fn count;
    string_join_fn join;
    string_concat_fn concat;
    string_unary_fn reverse;
    string_pred_fn is_digit;
    string_pred_fn is_alpha;
    string_pred_fn is_alnum;
    string_pred_fn is_space;
} __STRING;

static __STRING string = {
    __cstl_string_substr,
    __cstl_string_split,
    __cstl_string_replace,
    __cstl_string_ltrim,
    __cstl_string_rtrim,
    __cstl_string_trim,
    __cstl_string_to_upper,
    __cstl_string_to_lower,
    __cstl_string_starts_with,
    __cstl_string_ends_with,
    __cstl_string_count,
    __cstl_string_join,
    __cstl_string_concat,
    __cstl_string_reverse,
    __cstl_string_is_digit,
    __cstl_string_is_alpha,
    __cstl_string_is_alnum,
    __cstl_string_is_space
};

#endif //OPENCSTL_STRING_H
