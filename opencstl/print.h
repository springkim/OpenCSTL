#pragma once
#ifndef HG_B241F16A3823AF916C83CBA69E7B6C6E7D27AFA0F5B136ECFDB943F532C49237_H
#define HG_B241F16A3823AF916C83CBA69E7B6C6E7D27AFA0F5B136ECFDB943F532C49237_H
#include "crossplatform.h"
#if defined(OCSTL_C_VERSION_11) || defined(OCSTL_C_VERSION_17) || defined(OCSTL_C_VERSION_23) || defined(OCSTL_CC_TCC)
#  define OCSTL_CSTLIO_DISPATCH_GENERIC 1
#elif defined(OCSTL_C_VERSION_99) && (defined(OCSTL_CC_GCC) || defined(OCSTL_CC_CLANG))
#  define OCSTL_CSTLIO_DISPATCH_BUILTIN 1
#elif defined(OCSTL_CC_MSVC) && !defined(OCSTL_C_VERSION_PRE_ANSI)
#  define OCSTL_CSTLIO_DISPATCH_SIZEOF 1
#else
#  define OCSTL_CSTLIO_DISPATCH_VARARGS 1
#endif
#if defined(OCSTL_CSTLIO_DISPATCH_GENERIC) || defined(OCSTL_CSTLIO_DISPATCH_BUILTIN) || defined(OCSTL_CSTLIO_DISPATCH_VARARGS) || defined(OCSTL_CSTLIO_DISPATCH_SIZEOF)
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
#define OCSTL_STR_INNER(x) #x
#define OCSTL_STR(x)       OCSTL_STR_INNER(x)
#ifdef OCSTL_OS_WINDOWS
#if defined(OCSTL_CC_MSVC) || defined(OCSTL_CC_GCC)
#include <codecapi.h>
#include <processenv.h>
#else
#include <WinBase.h>
#endif
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif
#ifndef CP_UTF8
#define CP_UTF8 65001
#endif
static int s_cio_unicode_init = -1;

static void ocstl_ensure_unicode(void) {
    HANDLE h;
    DWORD mode = 0;
    if (s_cio_unicode_init != -1) return;
    SetConsoleOutputCP(CP_UTF8);
    h = GetStdHandle(STD_OUTPUT_HANDLE);
    if (GetConsoleMode(h, &mode))
        SetConsoleMode(h, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
    s_cio_unicode_init = 1;
}
#else
#define ocstl_ensure_unicode() ((void)0)
#endif
#if defined(OCSTL_CSTLIO_DISPATCH_GENERIC) || defined(OCSTL_CSTLIO_DISPATCH_BUILTIN) || defined(OCSTL_CSTLIO_DISPATCH_SIZEOF)
typedef enum {
    OCSTL_COUT_BOOL,
    OCSTL_COUT_SCHAR,
    OCSTL_COUT_UCHAR,
    OCSTL_COUT_SHORT,
    OCSTL_COUT_USHORT,
    OCSTL_COUT_INT,
    OCSTL_COUT_UINT,
    OCSTL_COUT_LONG,
    OCSTL_COUT_ULONG,
    OCSTL_COUT_LLONG,
    OCSTL_COUT_ULLONG,
    OCSTL_COUT_FLOAT,
    OCSTL_COUT_DOUBLE,
    OCSTL_COUT_CHAR,
    OCSTL_COUT_STR,
    OCSTL_COUT_INTPTR,
} ocstl_cout_type_t;

typedef struct {
    ocstl_cout_type_t type;

    union {
        _Bool b;
        signed char sc;
        unsigned char uc;
        short h;
        unsigned short uh;
        int i;
        unsigned int u;
        long l;
        unsigned long ul;
        long long ll;
        unsigned long long ull;
        float f;
        double d;
        char c;
        const char *s;
        void *p;
    };
} ocstl_val_t;

static ocstl_val_t ocstl_mk_bool(_Bool x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_BOOL;
    v.b = x;
    return v;
}

static ocstl_val_t ocstl_mk_schar(signed char x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_SCHAR;
    v.sc = x;
    return v;
}

static ocstl_val_t ocstl_mk_uchar(unsigned char x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_UCHAR;
    v.uc = x;
    return v;
}

static ocstl_val_t ocstl_mk_short(short x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_SHORT;
    v.h = x;
    return v;
}

static ocstl_val_t ocstl_mk_ushort(unsigned short x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_USHORT;
    v.uh = x;
    return v;
}

static ocstl_val_t ocstl_mk_int(int x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_INT;
    v.i = x;
    return v;
}

static ocstl_val_t ocstl_mk_intptr(int *x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_INTPTR;
    v.p = x;
    return v;
}

static ocstl_val_t ocstl_mk_uint(unsigned int x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_UINT;
    v.u = x;
    return v;
}

static ocstl_val_t ocstl_mk_long(long x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_LONG;
    v.l = x;
    return v;
}

static ocstl_val_t ocstl_mk_ulong(unsigned long x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_ULONG;
    v.ul = x;
    return v;
}

static ocstl_val_t ocstl_mk_llong(long long x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_LLONG;
    v.ll = x;
    return v;
}

static ocstl_val_t ocstl_mk_ullong(unsigned long long x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_ULLONG;
    v.ull = x;
    return v;
}

static ocstl_val_t ocstl_mk_float(float x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_FLOAT;
    v.f = x;
    return v;
}

static ocstl_val_t ocstl_mk_double(double x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_DOUBLE;
    v.d = x;
    return v;
}

static ocstl_val_t ocstl_mk_char(char x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_CHAR;
    v.c = x;
    return v;
}

static ocstl_val_t ocstl_mk_str(const char *x) {
    ocstl_val_t v;
    v.type = OCSTL_COUT_STR;
    v.s = x;
    return v;
}
#if defined(OCSTL_CSTLIO_DISPATCH_GENERIC)
#define OCSTL_VAL(x) ((OCSTL_STR(x)[0] == '\'')                      \
    ? ocstl_mk_char((char)((intptr_t)(x)))                           \
    : _Generic((x),                                                  \
        _Bool:              ocstl_mk_bool,                           \
        signed char:        ocstl_mk_schar,                          \
        unsigned char:      ocstl_mk_uchar,                          \
        short:              ocstl_mk_short,                          \
        unsigned short:     ocstl_mk_ushort,                         \
        int:                ocstl_mk_int,                            \
        unsigned int:       ocstl_mk_uint,                           \
        long:               ocstl_mk_long,                           \
        unsigned long:      ocstl_mk_ulong,                          \
        long long:          ocstl_mk_llong,                          \
        unsigned long long: ocstl_mk_ullong,                         \
        float:              ocstl_mk_float,                          \
        double:             ocstl_mk_double,                         \
        char:               ocstl_mk_char,                           \
        char *:             ocstl_mk_str,                            \
        const char *:       ocstl_mk_str,                             \
        int*:                ocstl_mk_intptr                             \
      )(x))
#elif defined(OCSTL_CSTLIO_DISPATCH_BUILTIN)
#define OCSTL_TYPEEQ(x, T) __builtin_types_compatible_p(__typeof__(x), T)
#define OCSTL_AS(x, T)                                          \
    (*(T*)__builtin_choose_expr(OCSTL_TYPEEQ(x, T),             \
        (void*)&(__typeof__(x)){(x)},                           \
        (void*)0))
#define OCSTL_VAL(x) ((OCSTL_STR(x)[0] == '\'')                                                                     \
    ? ocstl_mk_char((char)((intptr_t)(x)))                                                                          \
    : (__builtin_choose_expr(OCSTL_TYPEEQ(x, _Bool),              ocstl_mk_bool  (OCSTL_AS(x, _Bool)),               \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, signed char),        ocstl_mk_schar (OCSTL_AS(x, signed char)),         \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned char),      ocstl_mk_uchar (OCSTL_AS(x, unsigned char)),       \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, short),              ocstl_mk_short (OCSTL_AS(x, short)),               \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned short),     ocstl_mk_ushort(OCSTL_AS(x, unsigned short)),      \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, int),                ocstl_mk_int   (OCSTL_AS(x, int)),                 \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned int),       ocstl_mk_uint  (OCSTL_AS(x, unsigned int)),        \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, long),               ocstl_mk_long  (OCSTL_AS(x, long)),                \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned long),      ocstl_mk_ulong (OCSTL_AS(x, unsigned long)),       \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, long long),          ocstl_mk_llong (OCSTL_AS(x, long long)),           \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned long long), ocstl_mk_ullong(OCSTL_AS(x, unsigned long long)),  \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, float),              ocstl_mk_float (OCSTL_AS(x, float)),               \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, double),             ocstl_mk_double(OCSTL_AS(x, double)),              \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, char),               ocstl_mk_char  (OCSTL_AS(x, char)),                \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, char *),             ocstl_mk_str   (OCSTL_AS(x, char *)),              \
       __builtin_choose_expr(OCSTL_TYPEEQ(x, const char *),       ocstl_mk_str   (OCSTL_AS(x, const char *)),        \
           ocstl_mk_int(0)))))))))))))))))))
#elif defined(OCSTL_CSTLIO_DISPATCH_SIZEOF)
#if defined(OCSTL_CC_TCC) || defined(OCSTL_CC_GCC) || defined(OCSTL_CC_CLANG)
#  define OCSTL_ADDR(x) (&((__typeof__(x)){(x)}))
#  define OCSTL_VAL(x) (                                                       \
    (OCSTL_STR(x)[0] == '"')                                                   \
        ? ocstl_mk_str((const char *) (uintptr_t) (x)) :                       \
    (OCSTL_STR(x)[0] == '\'')                                                  \
        ? ocstl_mk_char((char) (intptr_t) (x)) :                               \
    ((const void *) OCSTL_ADDR(x) == (const void *) (uintptr_t) (x))           \
        ? ocstl_mk_str((const char *) (uintptr_t) (x)) :                       \
    (sizeof(x) == 1)                                                           \
        ? ocstl_mk_char(*(const char *) (const void *) OCSTL_ADDR(x)) :        \
    (sizeof(x) == 2)                                                           \
        ? ocstl_mk_short(*(const short *) (const void *) OCSTL_ADDR(x)) :      \
    (sizeof(x) == 4) ? (                                                       \
        ((*(const int *) (const void *) OCSTL_ADDR(x)) == (int) (intptr_t) (x))\
            ? ocstl_mk_int(*(const int *) (const void *) OCSTL_ADDR(x))        \
            : ocstl_mk_float(*(const float *) (const void *) OCSTL_ADDR(x))    \
    ) :                                                                        \
    (sizeof(x) == 8) ? (                                                       \
        ((*(const long long *) (const void *) OCSTL_ADDR(x))                   \
            == (long long) (intptr_t) (x))                                     \
            ? ocstl_mk_str(*(const char * const *) (const void *) OCSTL_ADDR(x))\
            : ocstl_mk_double(*(const double *) (const void *) OCSTL_ADDR(x)) \
    ) :                                                                        \
    ocstl_mk_int(0)                                                            \
)
#else
#  define OCSTL_VAL(x) (                                                       \
    (OCSTL_STR(x)[0] == '"')                                                   \
        ? ocstl_mk_str((const char *) (uintptr_t) (x)) :                       \
    (OCSTL_STR(x)[0] == '\'')                                                  \
        ? ocstl_mk_char((char) (intptr_t) (x)) :                               \
    (sizeof(x) == 1)                                                           \
        ? ocstl_mk_char((char) (intptr_t) (x)) :                               \
    (sizeof(x) == 2)                                                           \
        ? ocstl_mk_short((short) (intptr_t) (x)) :                             \
    (sizeof(x) == 4) ? (                                                       \
        ((int) (intptr_t) (x) == (x))                                          \
            ? ocstl_mk_int((int) (intptr_t) (x))                               \
            : ocstl_mk_float((float) (x))                                      \
    ) :                                                                        \
    (sizeof(x) == 8) ? (                                                       \
        ((long long) (intptr_t) (x) == (x))                                    \
            ? ocstl_mk_str((const char *) (uintptr_t) (x))                     \
            : ocstl_mk_double((double) (intptr_t) (x))                         \
    ) :                                                                        \
            \
    ocstl_mk_str((const char *) (uintptr_t) (x))                               \
)
#endif
#endif
#endif
typedef struct {
    char fill;
    char align;
    char sign;
    bool alt;
    bool zero_pad;
    int width;
    int precision;
    int length;
    char type;
} ocstl_fmt_spec_t;

static void ocstl_spec_init(ocstl_fmt_spec_t *s) {
    s->fill = ' ';
    s->align = 0;
    s->sign = 0;
    s->alt = false;
    s->zero_pad = false;
    s->width = -1;
    s->precision = -1;
    s->length = 0;
    s->type = 0;
}

static bool ocstl_is_align(char c) { return c == '<' || c == '>' || c == '^'; }

static const char *ocstl_parse_spec(const char *p, ocstl_fmt_spec_t *spec) {
    if (p[0] && p[1] && ocstl_is_align(p[1])) {
        spec->fill = p[0];
        spec->align = p[1];
        p += 2;
    } else if (ocstl_is_align(p[0])) {
        spec->align = p[0];
        p += 1;
    }
    if (*p == '+') {
        spec->sign = '+';
        p++;
    }
    if (*p == '#') {
        spec->alt = true;
        p++;
    }
    if (*p == '0') {
        spec->zero_pad = true;
        p++;
    }
    if (*p >= '0' && *p <= '9') {
        int w = 0;
        while (*p >= '0' && *p <= '9') {
            w = w * 10 + (*p - '0');
            p++;
        }
        spec->width = w;
    }
    if (*p == '.') {
        p++;
        int pr = 0;
        while (*p >= '0' && *p <= '9') {
            pr = pr * 10 + (*p - '0');
            p++;
        }
        spec->precision = pr;
    }
    if (*p == 'l') {
        p++;
        if (*p == 'l') {
            p++;
            spec->length = 2;
        } else {
            spec->length = 1;
        }
    }
    if (*p && *p != '}') {
        spec->type = *p;
        p++;
    }
    return p;
}

static void ocstl_emit_pad(char ch, int n) {
    { int i; for (i = 0; i < n; i++) putchar(ch); }
}

static void ocstl_emit_padded(const char *core, int core_len,
                              const ocstl_fmt_spec_t *spec,
                              int sign_prefix_len) {
    int width = spec->width;
    if (width < 0 || width <= core_len) {
        fwrite(core, 1, core_len, stdout);
        return;
    }
    int pad = width - core_len;
    char align = spec->align;
    char fill = spec->fill;
    if (align == 0 && spec->zero_pad && sign_prefix_len >= 0) {
        fwrite(core, 1, sign_prefix_len, stdout);
        ocstl_emit_pad('0', pad);
        fwrite(core + sign_prefix_len, 1, core_len - sign_prefix_len, stdout);
        return;
    }
    if (align == 0) align = (sign_prefix_len >= 0) ? '>' : '<';
    if (align == '<') {
        fwrite(core, 1, core_len, stdout);
        ocstl_emit_pad(fill, pad);
    } else if (align == '>') {
        ocstl_emit_pad(fill, pad);
        fwrite(core, 1, core_len, stdout);
    } else {
        int left = pad / 2;
        int right = pad - left;
        ocstl_emit_pad(fill, left);
        fwrite(core, 1, core_len, stdout);
        ocstl_emit_pad(fill, right);
    }
}

static int ocstl_format_int(char *buf, int bufsz,
                            unsigned long long uval, bool negative,
                            const ocstl_fmt_spec_t *spec,
                            int *sign_prefix_len) {
    int base = 10;
    bool upper = false;
    const char *prefix = "";
    switch (spec->type) {
        case 'x': base = 16;
            break;
        case 'X': base = 16;
            upper = true;
            break;
        case 'o': base = 8;
            break;
        case 'b': base = 2;
            break;
        default: break;
    }
    if (spec->alt) {
        if (spec->type == 'x') prefix = "0x";
        else if (spec->type == 'X') prefix = "0X";
        else if (spec->type == 'b') prefix = "0b";
        else if (spec->type == 'o') prefix = "0o";
    }
    char tmp[80];
    int tlen = 0;
    if (uval == 0) tmp[tlen++] = '0';
    while (uval > 0) {
        unsigned digit = (unsigned) (uval % (unsigned long long) base);
        char c = (digit < 10)
                     ? (char) ('0' + digit)
                     : (char) ((upper ? 'A' : 'a') + (digit - 10));
        tmp[tlen++] = c;
        uval /= base;
    }
    int pos = 0;
    if (negative) buf[pos++] = '-';
    else if (spec->sign == '+' && base == 10) buf[pos++] = '+';
    int plen = (int) strlen(prefix);
    { int i; for (i = 0; i < plen; i++) buf[pos++] = prefix[i]; }
    *sign_prefix_len = pos;
    { int i; for (i = tlen - 1; i >= 0 && pos < bufsz - 1; i--) buf[pos++] = tmp[i]; }
    buf[pos] = '\0';
    return pos;
}

static int ocstl_format_float(char *buf, int bufsz, double dval,
                              const ocstl_fmt_spec_t *spec,
                              int *sign_prefix_len) {
    bool negative = dval < 0.0;
    if (negative) dval = -dval;
    int prec = spec->precision >= 0 ? spec->precision : 6;
    char fmt[16];
    char type = spec->type ? spec->type : 'f';
    if (type != 'f' && type != 'e' && type != 'E' && type != 'g' && type != 'G') type = 'f';
    snprintf(fmt, sizeof fmt, "%%.%d%c", prec, type);
    int pos = 0;
    if (negative) buf[pos++] = '-';
    else if (spec->sign == '+') buf[pos++] = '+';
    *sign_prefix_len = pos;
    int n = snprintf(buf + pos, bufsz - pos, fmt, dval);
    if (n < 0) n = 0;
    return pos + n;
}
#if defined(OCSTL_CSTLIO_DISPATCH_GENERIC) || defined(OCSTL_CSTLIO_DISPATCH_BUILTIN) || defined(OCSTL_CSTLIO_DISPATCH_SIZEOF)
static void ocstl_print_val(const ocstl_val_t *v, const ocstl_fmt_spec_t *spec) {
    char buf[128];
    int len = 0;
    int sign_prefix_len = -1;
    if (spec->type == 'c') {
        long long ch = 0;
        int handled = 1;
        switch (v->type) {
            case OCSTL_COUT_BOOL: ch = v->b;
                break;
            case OCSTL_COUT_SCHAR: ch = v->sc;
                break;
            case OCSTL_COUT_UCHAR: ch = v->uc;
                break;
            case OCSTL_COUT_SHORT: ch = v->h;
                break;
            case OCSTL_COUT_USHORT: ch = v->uh;
                break;
            case OCSTL_COUT_INT: ch = v->i;
                break;
            case OCSTL_COUT_UINT: ch = v->u;
                break;
            case OCSTL_COUT_LONG: ch = v->l;
                break;
            case OCSTL_COUT_ULONG: ch = v->ul;
                break;
            case OCSTL_COUT_LLONG: ch = v->ll;
                break;
            case OCSTL_COUT_ULLONG: ch = (long long) v->ull;
                break;
            case OCSTL_COUT_CHAR: ch = v->c;
                break;
            case OCSTL_COUT_INTPTR: ch = (long long) v->p;
                break;
            default: handled = 0;
                break;
        }
        if (handled) {
            buf[0] = (char) ch;
            ocstl_emit_padded(buf, 1, spec, -1);
            return;
        }
    }
    switch (v->type) {
        case OCSTL_COUT_BOOL:
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) v->b, false,
                                   spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_SCHAR: {
            long long sv = v->sc;
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) (sv < 0 ? -sv : sv),
                                   sv < 0, spec, &sign_prefix_len);
            break;
        }
        case OCSTL_COUT_UCHAR:
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) v->uc, false,
                                   spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_SHORT: {
            long long sv = v->h;
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) (sv < 0 ? -sv : sv),
                                   sv < 0, spec, &sign_prefix_len);
            break;
        }
        case OCSTL_COUT_USHORT:
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) v->uh, false,
                                   spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_INT: {
            long long sv = v->i;
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) (sv < 0 ? -sv : sv),
                                   sv < 0, spec, &sign_prefix_len);
            break;
        }
        case OCSTL_COUT_INTPTR: {
            long long sv = (long long)v->p;
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) (sv < 0 ? -sv : sv),
                                   sv < 0, spec, &sign_prefix_len);
            break;
        }
        case OCSTL_COUT_LONG: {
            long long sv = v->l;
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) (sv < 0 ? -sv : sv),
                                   sv < 0, spec, &sign_prefix_len);
            break;
        }
        case OCSTL_COUT_LLONG: {
            long long sv = v->ll;
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) (sv < 0 ? -sv : sv),
                                   sv < 0, spec, &sign_prefix_len);
            break;
        }
        case OCSTL_COUT_UINT:
            len = ocstl_format_int(buf, sizeof buf, (unsigned long long) v->u, false, spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_ULONG:
            len = ocstl_format_int(buf, sizeof buf, (unsigned long long) v->ul, false, spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_ULLONG:
            len = ocstl_format_int(buf, sizeof buf, v->ull, false, spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_FLOAT:
            len = ocstl_format_float(buf, sizeof buf, (double) v->f, spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_DOUBLE:
            len = ocstl_format_float(buf, sizeof buf, v->d, spec, &sign_prefix_len);
            break;
        case OCSTL_COUT_CHAR: {
            buf[0] = v->c;
            buf[1] = '\0';
            len = 1;
            break;
        }
        case OCSTL_COUT_STR: {
            switch (spec->type) {
                case 'd':
                case 'i': {
                    long long sv = v->ll;
                    len = ocstl_format_int(buf, sizeof buf,
                                           (unsigned long long) (sv < 0 ? -sv : sv),
                                           sv < 0, spec, &sign_prefix_len);
                    break;
                }
                case 'u':
                case 'x':
                case 'X':
                case 'o':
                case 'b':
                    len = ocstl_format_int(buf, sizeof buf, v->ull, false,
                                           spec, &sign_prefix_len);
                    break;
                case 'f':
                case 'e':
                case 'E':
                case 'g':
                case 'G':
                    len = ocstl_format_float(buf, sizeof buf, v->d,
                                             spec, &sign_prefix_len);
                    break;
                default: {
                    const char *s = v->s ? v->s : "(null)";
                    ocstl_emit_padded(s, (int) strlen(s), spec, -1);
                    return;
                }
            }
            break;
        }
    }
    ocstl_emit_padded(buf, len, spec, sign_prefix_len);
}

static void ocstl_print_impl(const char *fmt, const ocstl_val_t *args, int n) {
    int idx = 0;
    const char *p = fmt;
    ocstl_ensure_unicode();
    while (*p) {
        if (p[0] == '{' && p[1] == '{') {
            putchar('{');
            p += 2;
        } else if (p[0] == '}' && p[1] == '}') {
            putchar('}');
            p += 2;
        } else if (p[0] == '{') {
            const char *q = p + 1;
            ocstl_fmt_spec_t spec;
            ocstl_spec_init(&spec);
            if (*q == ':') {
                q = ocstl_parse_spec(q + 1, &spec);
            }
            if (*q == '}') {
                if (idx < n) ocstl_print_val(&args[idx++], &spec);
                p = q + 1;
            } else {
                putchar((unsigned char) *p++);
            }
        } else {
            putchar((unsigned char) *p++);
        }
    }
}

#define OCSTL_PRINT_N1(fmt) \
    do { ocstl_ensure_unicode(); ocstl_print_impl((fmt), NULL, 0); } while (0)
#define OCSTL_PRINT_1(fmt) OCSTL_PRINT_N1(fmt)
#define OCSTL_PRINT_2(fmt, _a) do {                                                       \
    ocstl_val_t _args[1];                                                                  \
    _args[0] = OCSTL_VAL(_a);                                                              \
    ocstl_print_impl((fmt), _args, 1);                                                     \
} while (0)
#define OCSTL_PRINT_3(fmt, _a, _b) do {                                                   \
    ocstl_val_t _args[2];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b);                                    \
    ocstl_print_impl((fmt), _args, 2);                                                     \
} while (0)
#define OCSTL_PRINT_4(fmt, _a, _b, _c) do {                                               \
    ocstl_val_t _args[3];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b); _args[2] = OCSTL_VAL(_c);          \
    ocstl_print_impl((fmt), _args, 3);                                                     \
} while (0)
#define OCSTL_PRINT_5(fmt, _a, _b, _c, _d) do {                                           \
    ocstl_val_t _args[4];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b);                                    \
    _args[2] = OCSTL_VAL(_c); _args[3] = OCSTL_VAL(_d);                                    \
    ocstl_print_impl((fmt), _args, 4);                                                     \
} while (0)
#define OCSTL_PRINT_6(fmt, _a, _b, _c, _d, _e) do {                                       \
    ocstl_val_t _args[5];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b);                                    \
    _args[2] = OCSTL_VAL(_c); _args[3] = OCSTL_VAL(_d); _args[4] = OCSTL_VAL(_e);          \
    ocstl_print_impl((fmt), _args, 5);                                                     \
} while (0)
#define OCSTL_PRINT_7(fmt, _a, _b, _c, _d, _e, _f) do {                                   \
    ocstl_val_t _args[6];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b); _args[2] = OCSTL_VAL(_c);          \
    _args[3] = OCSTL_VAL(_d); _args[4] = OCSTL_VAL(_e); _args[5] = OCSTL_VAL(_f);          \
    ocstl_print_impl((fmt), _args, 6);                                                     \
} while (0)
#define OCSTL_PRINT_8(fmt, _a, _b, _c, _d, _e, _f, _g) do {                               \
    ocstl_val_t _args[7];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b); _args[2] = OCSTL_VAL(_c);          \
    _args[3] = OCSTL_VAL(_d); _args[4] = OCSTL_VAL(_e); _args[5] = OCSTL_VAL(_f);          \
    _args[6] = OCSTL_VAL(_g);                                                              \
    ocstl_print_impl((fmt), _args, 7);                                                     \
} while (0)
#define OCSTL_PRINT_9(fmt, _a, _b, _c, _d, _e, _f, _g, _h) do {                           \
    ocstl_val_t _args[8];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b); _args[2] = OCSTL_VAL(_c);          \
    _args[3] = OCSTL_VAL(_d); _args[4] = OCSTL_VAL(_e); _args[5] = OCSTL_VAL(_f);          \
    _args[6] = OCSTL_VAL(_g); _args[7] = OCSTL_VAL(_h);                                    \
    ocstl_print_impl((fmt), _args, 8);                                                     \
} while (0)
#define OCSTL_PRINT_10(fmt, _a, _b, _c, _d, _e, _f, _g, _h, _i) do {                      \
    ocstl_val_t _args[9];                                                                  \
    _args[0] = OCSTL_VAL(_a); _args[1] = OCSTL_VAL(_b); _args[2] = OCSTL_VAL(_c);          \
    _args[3] = OCSTL_VAL(_d); _args[4] = OCSTL_VAL(_e); _args[5] = OCSTL_VAL(_f);          \
    _args[6] = OCSTL_VAL(_g); _args[7] = OCSTL_VAL(_h); _args[8] = OCSTL_VAL(_i);          \
    ocstl_print_impl((fmt), _args, 9);                                                     \
} while (0)
#define OCSTL_PASTE2(a, b) a##b
#define OCSTL_PASTE(a, b)  OCSTL_PASTE2(a, b)
#define OCSTL_EXPAND(x) x
#define print(...)   OCSTL_EXPAND(OCSTL_PASTE(OCSTL_PRINT_, ARGN(__VA_ARGS__))(__VA_ARGS__))
#define println(...) do { print(__VA_ARGS__); putchar('\n'); } while (0)
#elif defined(OCSTL_CSTLIO_DISPATCH_VARARGS)
#include <stdarg.h>
#include <ctype.h>
#if defined(OCSTL_OS_WINDOWS) && defined(OCSTL_CC_MSVC)
#ifndef OPENCSTL_MSVC_C99_OPTIMIZE
#pragma warning(disable:4081)
#pragma comment(compiler, "/Zi")
#pragma comment(linker, "/DEBUG")
#pragma comment(linker, "/DEBUGTYPE:cv")
#pragma optimize("", off)
#endif
#  define OCSTL_VA_USE_DBGHELP 1
#  include <intrin.h>
#  define _NO_CVCONST_H
#  include <dbghelp.h>
#  pragma comment(lib, "dbghelp.lib")
enum {
    OCSTL_DH_btVoid = 1, OCSTL_DH_btChar = 2, OCSTL_DH_btWChar = 3,
    OCSTL_DH_btInt = 6, OCSTL_DH_btUInt = 7, OCSTL_DH_btFloat = 8,
    OCSTL_DH_btBool = 10, OCSTL_DH_btLong = 13, OCSTL_DH_btULong = 14
};
#endif
typedef enum {
    OCSTL_VAK_UNKNOWN = 0,
    OCSTL_VAK_BOOL,
    OCSTL_VAK_CHAR, OCSTL_VAK_UCHAR,
    OCSTL_VAK_SHORT, OCSTL_VAK_USHORT,
    OCSTL_VAK_INT, OCSTL_VAK_UINT,
    OCSTL_VAK_LONG, OCSTL_VAK_ULONG,
    OCSTL_VAK_LLONG, OCSTL_VAK_ULLONG,
    OCSTL_VAK_FLOAT, OCSTL_VAK_DOUBLE,
    OCSTL_VAK_STR
} ocstl_vak_t;
#ifdef OCSTL_VA_USE_DBGHELP
static HANDLE g_ocstl_dh_proc = NULL;
static int g_ocstl_dh_ready = 0;

static void ocstl_dh_init(void) {
    if (g_ocstl_dh_ready) return;
    g_ocstl_dh_proc = GetCurrentProcess();
    SymInitialize(g_ocstl_dh_proc, NULL, TRUE);
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME);
    g_ocstl_dh_ready = 1;
}

static ocstl_vak_t ocstl_dh_classify_base(DWORD base, ULONG64 size) {
    switch (base) {
        case OCSTL_DH_btBool: return OCSTL_VAK_BOOL;
        case OCSTL_DH_btChar: return OCSTL_VAK_CHAR;
        case OCSTL_DH_btInt:
            if (size == 1) return OCSTL_VAK_CHAR;
            if (size == 2) return OCSTL_VAK_SHORT;
            if (size == 8) return OCSTL_VAK_LLONG;
            return OCSTL_VAK_INT;
        case OCSTL_DH_btUInt:
            if (size == 1) return OCSTL_VAK_UCHAR;
            if (size == 2) return OCSTL_VAK_USHORT;
            if (size == 8) return OCSTL_VAK_ULLONG;
            return OCSTL_VAK_UINT;
        case OCSTL_DH_btFloat: return (size == 8) ? OCSTL_VAK_DOUBLE : OCSTL_VAK_FLOAT;
        case OCSTL_DH_btLong: return OCSTL_VAK_LONG;
        case OCSTL_DH_btULong: return OCSTL_VAK_ULONG;
        default: return OCSTL_VAK_UNKNOWN;
    }
}

static ocstl_vak_t ocstl_dh_resolve_typeidx(ULONG64 mod_base, DWORD type_idx, int depth) {
    DWORD tag = 0;
    ULONG64 size = 0;
    if (depth > 4) return OCSTL_VAK_UNKNOWN;
    SymGetTypeInfo(g_ocstl_dh_proc, mod_base, type_idx, TI_GET_SYMTAG, &tag);
    SymGetTypeInfo(g_ocstl_dh_proc, mod_base, type_idx, TI_GET_LENGTH, &size);
    if (tag == SymTagBaseType) {
        DWORD base = 0;
        SymGetTypeInfo(g_ocstl_dh_proc, mod_base, type_idx, TI_GET_BASETYPE, &base);
        return ocstl_dh_classify_base(base, size);
    }
    if (tag == SymTagPointerType) {
        DWORD child = 0, ctag = 0, cbase = 0;
        SymGetTypeInfo(g_ocstl_dh_proc, mod_base, type_idx, TI_GET_TYPE, &child);
        SymGetTypeInfo(g_ocstl_dh_proc, mod_base, child, TI_GET_SYMTAG, &ctag);
        if (ctag == SymTagBaseType) {
            SymGetTypeInfo(g_ocstl_dh_proc, mod_base, child, TI_GET_BASETYPE, &cbase);
            if (cbase == OCSTL_DH_btChar) return OCSTL_VAK_STR;
        }
        return OCSTL_VAK_ULLONG;
    }
    if (tag == SymTagArrayType) {
        DWORD child = 0, cbase = 0;
        SymGetTypeInfo(g_ocstl_dh_proc, mod_base, type_idx, TI_GET_TYPE, &child);
        SymGetTypeInfo(g_ocstl_dh_proc, mod_base, child, TI_GET_BASETYPE, &cbase);
        if (cbase == OCSTL_DH_btChar) return OCSTL_VAK_STR;
        return OCSTL_VAK_ULLONG;
    }
    if (tag == SymTagTypedef) {
        DWORD child = 0;
        SymGetTypeInfo(g_ocstl_dh_proc, mod_base, type_idx, TI_GET_TYPE, &child);
        return ocstl_dh_resolve_typeidx(mod_base, child, depth + 1);
    }
    return OCSTL_VAK_UNKNOWN;
}

typedef struct {
    const char *target;
    ocstl_vak_t result;
    int found;
} ocstl_dh_lookup_t;

static BOOL CALLBACK ocstl_dh_enum_cb(SYMBOL_INFO *sym, ULONG sz, PVOID user) {
    ocstl_dh_lookup_t *ctx = (ocstl_dh_lookup_t *) user;
    (void) sz;
    if (strcmp(sym->Name, ctx->target) != 0) return TRUE;
    ctx->result = ocstl_dh_resolve_typeidx(sym->ModBase, sym->TypeIndex, 0);
    ctx->found = 1;
    return FALSE;
}
#endif
static ocstl_vak_t ocstl_va_classify_literal(const char *t) {
    const char *p;
    int has_dot = 0, is_hex = 0, u = 0, l = 0;
    size_t n;
    char last, prev, prev2;
    if (!t || !t[0]) return OCSTL_VAK_UNKNOWN;
    if (t[0] == '"') return OCSTL_VAK_STR;
    if (t[0] == '\'') return OCSTL_VAK_CHAR;
    p = t;
    if (*p == '+' || *p == '-') p++;
    if (!((*p >= '0' && *p <= '9') || *p == '.')) return OCSTL_VAK_UNKNOWN;
    if (p[0] == '0' && (p[1] == 'x' || p[1] == 'X')) is_hex = 1;
    while (*p) {
        char c = *p++;
        if (!is_hex && (c == '.' || c == 'e' || c == 'E')) has_dot = 1;
        if (is_hex && (c == 'p' || c == 'P')) has_dot = 1;
    }
    n = strlen(t);
    last = (char) ((unsigned char) t[n - 1] | 0x20);
    prev = (n >= 2) ? (char) ((unsigned char) t[n - 2] | 0x20) : 0;
    prev2 = (n >= 3) ? (char) ((unsigned char) t[n - 3] | 0x20) : 0;
    if (last == 'f' && !is_hex) return OCSTL_VAK_FLOAT;
    if (has_dot) return OCSTL_VAK_DOUBLE;
    if (last == 'u') {
        u = 1;
        if (prev == 'l') l = (prev2 == 'l') ? 2 : 1;
    } else if (last == 'l') {
        l = (prev == 'l') ? 2 : 1;
        if ((l == 2 ? prev2 : prev) == 'u') u = 1;
    }
    if (l == 2) return u ? OCSTL_VAK_ULLONG : OCSTL_VAK_LLONG;
    if (l == 1) return u ? OCSTL_VAK_ULONG : OCSTL_VAK_LONG;
    if (u) return OCSTL_VAK_UINT;
    return OCSTL_VAK_INT;
}

static ocstl_vak_t ocstl_va_lookup_token(const char *token, void *caller_pc) {
    (void) caller_pc;
    if (!token || !token[0]) return OCSTL_VAK_UNKNOWN;
    if (token[0] == '"') return OCSTL_VAK_STR;
    if (token[0] == '\'') return OCSTL_VAK_CHAR;
    if ((token[0] >= '0' && token[0] <= '9') ||
        token[0] == '-' || token[0] == '+' || token[0] == '.') {
        return ocstl_va_classify_literal(token);
    }
#ifdef OCSTL_VA_USE_DBGHELP
{
        IMAGEHLP_STACK_FRAME isf;
        ocstl_dh_lookup_t ctx;
        memset(&isf, 0, sizeof isf);
        ocstl_dh_init();
        isf.InstructionOffset = (DWORD64) (uintptr_t) caller_pc;
        SymSetContext(g_ocstl_dh_proc, &isf, NULL);
        ctx.target = token;
        ctx.result = OCSTL_VAK_UNKNOWN;
        ctx.found = 0;
        SymEnumSymbols(g_ocstl_dh_proc, 0, token, ocstl_dh_enum_cb, &ctx);
        return ctx.result;
    }
#else
return OCSTL_VAK_UNKNOWN;
#endif
}

typedef struct {
    ocstl_vak_t kind;
    long long ll;
    unsigned long long ull;
    double d;
    const char *s;
} ocstl_va_arg_t;
#ifdef OCSTL_VA_USE_DBGHELP
__declspec(noinline)
#endif
static void ocstl_print_impl_va(const char *fmt, int n_args, ...) {
    ocstl_va_arg_t args[16];
    va_list ap;
    const char *p;
    void *caller_pc = NULL;
    int i, idx;
#ifdef OCSTL_VA_USE_DBGHELP
caller_pc = _ReturnAddress();
#endif
va_start(ap, n_args);
ocstl_ensure_unicode();
    if (n_args> 16) n_args = 16;
    for (i =0; i<n_args; i++) {
        const char *token = va_arg(ap, const char *);
        ocstl_vak_t k = ocstl_va_lookup_token(token, caller_pc);
        args[i].kind = k;
        args[i].ll = 0;
        args[i].ull = 0;
        args[i].d = 0.0;
        args[i].s = NULL;
        switch (k) {
            case OCSTL_VAK_BOOL: args[i].ll = (int) va_arg(ap, int);
                break;
            case OCSTL_VAK_CHAR: args[i].ll = (signed char) va_arg(ap, int);
                break;
            case OCSTL_VAK_UCHAR: args[i].ull = (unsigned char) va_arg(ap, int);
                break;
            case OCSTL_VAK_SHORT: args[i].ll = (short) va_arg(ap, int);
                break;
            case OCSTL_VAK_USHORT: args[i].ull = (unsigned short) va_arg(ap, int);
                break;
            case OCSTL_VAK_INT: args[i].ll = va_arg(ap, int);
                break;
            case OCSTL_VAK_UINT: args[i].ull = va_arg(ap, unsigned int);
                break;
            case OCSTL_VAK_LONG: args[i].ll = va_arg(ap, long);
                break;
            case OCSTL_VAK_ULONG: args[i].ull = va_arg(ap, unsigned long);
                break;
            case OCSTL_VAK_LLONG: args[i].ll = va_arg(ap, long long);
                break;
            case OCSTL_VAK_ULLONG: args[i].ull = va_arg(ap, unsigned long long);
                break;
            case OCSTL_VAK_FLOAT: args[i].d = va_arg(ap, double);
                break;
            case OCSTL_VAK_DOUBLE: args[i].d = va_arg(ap, double);
                break;
            case OCSTL_VAK_STR: args[i].s = va_arg(ap, const char *);
                break;
            default:
                args[i].kind = OCSTL_VAK_INT;
                args[i].ll = va_arg(ap, int);
                break;
        }
    }
va_end(ap);
p = fmt;
idx = 0;
    while (*p) {
    char buf[128];
    int len = 0, sign_prefix_len = -1;
    const char *q;
    ocstl_fmt_spec_t spec;
    ocstl_vak_t k;
    char t;
    if (p[0] == '{' && p[1] == '{') {
        putchar('{');
        p += 2;
        continue;
    }
    if (p[0] == '}' && p[1] == '}') {
        putchar('}');
        p += 2;
        continue;
    }
    if (p[0] != '{') {
        putchar((unsigned char) *p++);
        continue;
    }
    q = p + 1;
    ocstl_spec_init(&spec);
    if (*q == ':') q = ocstl_parse_spec(q + 1, &spec);
    if (*q != '}') {
        putchar((unsigned char) *p++);
        continue;
    }
    if (idx >= n_args) {
        p = q + 1;
        continue;
    }
    k = args[idx].kind;
    t = spec.type;
    if (t == 's') k = OCSTL_VAK_STR;
    else if (t == 'c') k = OCSTL_VAK_CHAR;
    else if (t == 'f' || t == 'e' || t == 'E' || t == 'g' || t == 'G') k = OCSTL_VAK_DOUBLE;
    else if (t == 'd' || t == 'i') {
        if (k == OCSTL_VAK_FLOAT || k == OCSTL_VAK_DOUBLE) {
            args[idx].ll = (long long) args[idx].d;
            k = OCSTL_VAK_LLONG;
        } else if (k == OCSTL_VAK_UCHAR || k == OCSTL_VAK_USHORT ||
                   k == OCSTL_VAK_UINT || k == OCSTL_VAK_ULONG ||
                   k == OCSTL_VAK_ULLONG) {
            args[idx].ll = (long long) args[idx].ull;
            k = OCSTL_VAK_LLONG;
        }
    } else if (t == 'u' || t == 'x' || t == 'X' || t == 'o' || t == 'b') {
        if (k == OCSTL_VAK_FLOAT || k == OCSTL_VAK_DOUBLE) {
            args[idx].ull = (unsigned long long) args[idx].d;
            k = OCSTL_VAK_ULLONG;
        } else if (k == OCSTL_VAK_CHAR || k == OCSTL_VAK_SHORT ||
                   k == OCSTL_VAK_INT || k == OCSTL_VAK_LONG ||
                   k == OCSTL_VAK_LLONG) {
            args[idx].ull = (unsigned long long) args[idx].ll;
            k = OCSTL_VAK_ULLONG;
        }
    }
    switch (k) {
        case OCSTL_VAK_STR: {
            const char *s = args[idx].s ? args[idx].s : "(null)";
            ocstl_emit_padded(s, (int) strlen(s), &spec, -1);
            idx++;
            p = q + 1;
            continue;
        }
        case OCSTL_VAK_CHAR:
        case OCSTL_VAK_UCHAR: {
            if (t == 0 || t == 'c') {
                buf[0] = (k == OCSTL_VAK_UCHAR)
                             ? (char) args[idx].ull
                             : (char) args[idx].ll;
                ocstl_emit_padded(buf, 1, &spec, -1);
                idx++;
                p = q + 1;
                continue;
            }
            {
                long long sv = (k == OCSTL_VAK_CHAR)
                                   ? args[idx].ll
                                   : (long long) args[idx].ull;
                len = ocstl_format_int(buf, sizeof buf,
                                       (unsigned long long) (sv < 0 ? -sv : sv),
                                       sv < 0, &spec, &sign_prefix_len);
            }
            break;
        }
        case OCSTL_VAK_FLOAT:
        case OCSTL_VAK_DOUBLE:
            len = ocstl_format_float(buf, sizeof buf, args[idx].d,
                                     &spec, &sign_prefix_len);
            break;
        case OCSTL_VAK_BOOL:
        case OCSTL_VAK_SHORT:
        case OCSTL_VAK_INT:
        case OCSTL_VAK_LONG:
        case OCSTL_VAK_LLONG: {
            long long sv = args[idx].ll;
            len = ocstl_format_int(buf, sizeof buf,
                                   (unsigned long long) (sv < 0 ? -sv : sv),
                                   sv < 0, &spec, &sign_prefix_len);
            break;
        }
        case OCSTL_VAK_USHORT:
        case OCSTL_VAK_UINT:
        case OCSTL_VAK_ULONG:
        case OCSTL_VAK_ULLONG:
            len = ocstl_format_int(buf, sizeof buf, args[idx].ull, 0,
                                   &spec, &sign_prefix_len);
            break;
        default:
            len = ocstl_format_int(buf, sizeof buf, 0, 0,
                                   &spec, &sign_prefix_len);
            break;
    }
    ocstl_emit_padded(buf, len, &spec, sign_prefix_len);
    idx++;
    p = q + 1;
}
}

#define OCSTL_PASTE2(a, b) a##b
#define OCSTL_PASTE(a, b)  OCSTL_PASTE2(a, b)
#define OCSTL_EXPAND(x)    x
#define OCSTL_VA_PRINT_1(fmt) \
    ocstl_print_impl_va((fmt), 0)
#define OCSTL_VA_PRINT_2(fmt, _a) \
    ocstl_print_impl_va((fmt), 1, #_a, (_a))
#define OCSTL_VA_PRINT_3(fmt, _a, _b) \
    ocstl_print_impl_va((fmt), 2, #_a, (_a), #_b, (_b))
#define OCSTL_VA_PRINT_4(fmt, _a, _b, _c) \
    ocstl_print_impl_va((fmt), 3, #_a, (_a), #_b, (_b), #_c, (_c))
#define OCSTL_VA_PRINT_5(fmt, _a, _b, _c, _d) \
    ocstl_print_impl_va((fmt), 4, #_a, (_a), #_b, (_b), #_c, (_c), #_d, (_d))
#define OCSTL_VA_PRINT_6(fmt, _a, _b, _c, _d, _e) \
    ocstl_print_impl_va((fmt), 5, #_a, (_a), #_b, (_b), #_c, (_c), #_d, (_d), \
                                  #_e, (_e))
#define OCSTL_VA_PRINT_7(fmt, _a, _b, _c, _d, _e, _f) \
    ocstl_print_impl_va((fmt), 6, #_a, (_a), #_b, (_b), #_c, (_c), #_d, (_d), \
                                  #_e, (_e), #_f, (_f))
#define OCSTL_VA_PRINT_8(fmt, _a, _b, _c, _d, _e, _f, _g) \
    ocstl_print_impl_va((fmt), 7, #_a, (_a), #_b, (_b), #_c, (_c), #_d, (_d), \
                                  #_e, (_e), #_f, (_f), #_g, (_g))
#define OCSTL_VA_PRINT_9(fmt, _a, _b, _c, _d, _e, _f, _g, _h) \
    ocstl_print_impl_va((fmt), 8, #_a, (_a), #_b, (_b), #_c, (_c), #_d, (_d), \
                                  #_e, (_e), #_f, (_f), #_g, (_g), #_h, (_h))
#define OCSTL_VA_PRINT_10(fmt, _a, _b, _c, _d, _e, _f, _g, _h, _i) \
    ocstl_print_impl_va((fmt), 9, #_a, (_a), #_b, (_b), #_c, (_c), #_d, (_d), \
                                  #_e, (_e), #_f, (_f), #_g, (_g), #_h, (_h), \
                                  #_i, (_i))
#define print(...)   OCSTL_EXPAND(OCSTL_PASTE(OCSTL_VA_PRINT_, ARGN(__VA_ARGS__))(__VA_ARGS__))
#define println(...) do { print(__VA_ARGS__); putchar('\n'); } while (0)
#endif
#endif
#endif
