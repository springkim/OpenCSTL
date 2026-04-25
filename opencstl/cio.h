#pragma once
#ifndef OPENCSTL_CSTLIO_H
#define OPENCSTL_CSTLIO_H

#include "crossplatform.h"

// cstlio availability gate.
// C11+        : native _Generic
// C99 GCC/Clang: __builtin_choose_expr + __builtin_types_compatible_p
// otherwise   : not available (no portable C99 dispatch on MSVC etc.)
#if defined(OCSTL_C_VERSION_11) || defined(OCSTL_C_VERSION_17) || defined(OCSTL_C_VERSION_23)
#  define OCSTL_CSTLIO_DISPATCH_GENERIC 1
#elif defined(OCSTL_C_VERSION_99) && (defined(__GNUC__) || defined(__clang__))
#  define OCSTL_CSTLIO_DISPATCH_BUILTIN 1
#endif

#if defined(OCSTL_CSTLIO_DISPATCH_GENERIC) || defined(OCSTL_CSTLIO_DISPATCH_BUILTIN)
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "defines.h"
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
} ocstl_cout_type_t;

// Anonymous union is C11; widely supported as extension in C99 by
// GCC/Clang/MSVC/TCC/Pelles. Kept as-is for source compatibility.
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

// C11+ path: native _Generic dispatch.
// Note: char, signed char, unsigned char are three distinct types in C.
// Same for _Bool. Each must have its own arm or compilation fails.
#define OCSTL_VAL(x) _Generic((x),                  \
    _Bool:              ocstl_mk_bool,              \
    signed char:        ocstl_mk_schar,             \
    unsigned char:      ocstl_mk_uchar,             \
    short:              ocstl_mk_short,             \
    unsigned short:     ocstl_mk_ushort,            \
    int:                ocstl_mk_int,               \
    unsigned int:       ocstl_mk_uint,              \
    long:               ocstl_mk_long,              \
    unsigned long:      ocstl_mk_ulong,             \
    long long:          ocstl_mk_llong,             \
    unsigned long long: ocstl_mk_ullong,            \
    float:              ocstl_mk_float,             \
    double:             ocstl_mk_double,            \
    char:               ocstl_mk_char,              \
    char *:             ocstl_mk_str,               \
    const char *:       ocstl_mk_str                \
)(x)

#elif defined(OCSTL_CSTLIO_DISPATCH_BUILTIN)

// C99 path: GCC/Clang extensions emulate _Generic.
//
// Limitation: in C99, _Generic's automatic array-to-pointer decay is not
// available via __builtin_types_compatible_p. We could try `1 ? (x) : (x)`
// to force decay, but that also triggers integer/float promotion on small
// integer arms, breaking _Bool/char/short matching. Wrapping the cast in
// __builtin_choose_expr does not help either: GCC type-checks the
// unselected branch and rejects e.g. `(const char *)(float_expr)`.
//
// Conclusion: C99 path matches char* and const char* by exact type only.
//   const char *p = "hello"; print("{}", p);            // OK
//   print("{}", (const char *)"hello");                 // OK
//   print("{}", "hello");                               // ERROR in C99 path
//                                                       // (works in C11+)
// For best compatibility, build with -std=c11 or later.
#define OCSTL_TYPEEQ(x, T) __builtin_types_compatible_p(__typeof__(x), T)
#define OCSTL_AS(x, T)                                          \
    (*(T*)__builtin_choose_expr(OCSTL_TYPEEQ(x, T),             \
        (void*)&(__typeof__(x)){(x)},                           \
        (void*)0))

#define OCSTL_VAL(x)                                                                                                \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, _Bool),              ocstl_mk_bool  (OCSTL_AS(x, _Bool)),                 \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, signed char),        ocstl_mk_schar (OCSTL_AS(x, signed char)),           \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned char),      ocstl_mk_uchar (OCSTL_AS(x, unsigned char)),         \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, short),              ocstl_mk_short (OCSTL_AS(x, short)),                 \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned short),     ocstl_mk_ushort(OCSTL_AS(x, unsigned short)),        \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, int),                ocstl_mk_int   (OCSTL_AS(x, int)),                   \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned int),       ocstl_mk_uint  (OCSTL_AS(x, unsigned int)),          \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, long),               ocstl_mk_long  (OCSTL_AS(x, long)),                  \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned long),      ocstl_mk_ulong (OCSTL_AS(x, unsigned long)),         \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, long long),          ocstl_mk_llong (OCSTL_AS(x, long long)),             \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, unsigned long long), ocstl_mk_ullong(OCSTL_AS(x, unsigned long long)),    \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, float),              ocstl_mk_float (OCSTL_AS(x, float)),                 \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, double),             ocstl_mk_double(OCSTL_AS(x, double)),                \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, char),               ocstl_mk_char  (OCSTL_AS(x, char)),                  \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, char *),             ocstl_mk_str   (OCSTL_AS(x, char *)),                \
    __builtin_choose_expr(OCSTL_TYPEEQ(x, const char *),       ocstl_mk_str   (OCSTL_AS(x, const char *)),          \
        ocstl_mk_int(0)))))))))))))))))

#endif

#define OCSTL_MAP1(f,a)      f(a)
#define OCSTL_MAP2(f,a,...)  f(a), OCSTL_MAP1(f, __VA_ARGS__)
#define OCSTL_MAP3(f,a,...)  f(a), OCSTL_MAP2(f, __VA_ARGS__)
#define OCSTL_MAP4(f,a,...)  f(a), OCSTL_MAP3(f, __VA_ARGS__)
#define OCSTL_MAP5(f,a,...)  f(a), OCSTL_MAP4(f, __VA_ARGS__)
#define OCSTL_MAP6(f,a,...)  f(a), OCSTL_MAP5(f, __VA_ARGS__)
#define OCSTL_MAP7(f,a,...)  f(a), OCSTL_MAP6(f, __VA_ARGS__)
#define OCSTL_MAP8(f,a,...)  f(a), OCSTL_MAP7(f, __VA_ARGS__)
#define OCSTL_MAP9(f,a,...)  f(a), OCSTL_MAP8(f, __VA_ARGS__)
#define OCSTL_MAP10(f,a,...) f(a), OCSTL_MAP9(f, __VA_ARGS__)
#define OCSTL_MAP11(f,a,...) f(a), OCSTL_MAP10(f, __VA_ARGS__)
#define OCSTL_MAP12(f,a,...) f(a), OCSTL_MAP11(f, __VA_ARGS__)
#define OCSTL_MAP13(f,a,...) f(a), OCSTL_MAP12(f, __VA_ARGS__)
#define OCSTL_MAP14(f,a,...) f(a), OCSTL_MAP13(f, __VA_ARGS__)
#define OCSTL_MAP15(f,a,...) f(a), OCSTL_MAP14(f, __VA_ARGS__)
#define OCSTL_MAP16(f,a,...) f(a), OCSTL_MAP15(f, __VA_ARGS__)
#define OCSTL_MAP17(f,a,...) f(a), OCSTL_MAP16(f, __VA_ARGS__)
#define OCSTL_MAP18(f,a,...) f(a), OCSTL_MAP17(f, __VA_ARGS__)
#define OCSTL_MAP19(f,a,...) f(a), OCSTL_MAP18(f, __VA_ARGS__)
#define OCSTL_MAP20(f,a,...) f(a), OCSTL_MAP19(f, __VA_ARGS__)
#define OCSTL_MAP21(f,a,...) f(a), OCSTL_MAP20(f, __VA_ARGS__)
#define OCSTL_MAP22(f,a,...) f(a), OCSTL_MAP21(f, __VA_ARGS__)
#define OCSTL_MAP23(f,a,...) f(a), OCSTL_MAP22(f, __VA_ARGS__)
#define OCSTL_MAP24(f,a,...) f(a), OCSTL_MAP23(f, __VA_ARGS__)
#define OCSTL_MAP25(f,a,...) f(a), OCSTL_MAP24(f, __VA_ARGS__)
#define OCSTL_MAP26(f,a,...) f(a), OCSTL_MAP25(f, __VA_ARGS__)
#define OCSTL_MAP27(f,a,...) f(a), OCSTL_MAP26(f, __VA_ARGS__)
#define OCSTL_MAP28(f,a,...) f(a), OCSTL_MAP27(f, __VA_ARGS__)
#define OCSTL_MAP29(f,a,...) f(a), OCSTL_MAP28(f, __VA_ARGS__)
#define OCSTL_MAP30(f,a,...) f(a), OCSTL_MAP29(f, __VA_ARGS__)
#define OCSTL_MAP31(f,a,...) f(a), OCSTL_MAP30(f, __VA_ARGS__)
#define OCSTL_MAP32(f,a,...) f(a), OCSTL_MAP31(f, __VA_ARGS__)
#define OCSTL_MAP_SEL( \
    _1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16, \
    _17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32, N,...) \
    OCSTL_MAP##N
#define OCSTL_MAP(f,...) \
    OCSTL_MAP_SEL(__VA_ARGS__, \
        32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17, \
        16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1)(f, __VA_ARGS__)

typedef struct {
    char fill;
    char align;
    char sign;
    bool alt;
    bool zero_pad;
    int width;
    int precision;
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
    if (*p && *p != '}') {
        spec->type = *p;
        p++;
    }
    return p;
}

static void ocstl_emit_pad(char ch, int n) {
    for (int i = 0; i < n; i++) putchar(ch);
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
    for (int i = 0; i < plen; i++) buf[pos++] = prefix[i];
    *sign_prefix_len = pos;
    for (int i = tlen - 1; i >= 0 && pos < bufsz - 1; i--) buf[pos++] = tmp[i];
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

static void ocstl_print_val(const ocstl_val_t *v, const ocstl_fmt_spec_t *spec) {
    char buf[128];
    int len = 0;
    int sign_prefix_len = -1;
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
            const char *s = v->s ? v->s : "(null)";
            ocstl_emit_padded(s, (int) strlen(s), spec, -1);
            return;
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
#define OCSTL_PRINT_11(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_12(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_13(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_14(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_15(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_16(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_17(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_18(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_19(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_20(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_21(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_22(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_23(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_24(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_25(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_26(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_27(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_28(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_29(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_30(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_31(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PRINT_32(...) OCSTL_PRINT_NX(__VA_ARGS__)
#define OCSTL_PASTE2(a, b) a##b
#define OCSTL_PASTE(a, b)  OCSTL_PASTE2(a, b)
#define print(...)   OCSTL_PASTE(OCSTL_PRINT_, ARGN(__VA_ARGS__))(__VA_ARGS__)
#define println(...) do { print(__VA_ARGS__); putchar('\n'); } while (0)
#endif


#ifndef print
#define print(...)   __noop
#pragma message("warning: print/println is not defined in MSVC C99; use /std:c11 or newer")
#endif
#ifndef println
#define println(...) __noop
#endif

#endif
