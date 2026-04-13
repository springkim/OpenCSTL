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

#ifndef OPENCSTL_COMPARE_H
#define OPENCSTL_COMPARE_H
#include <string.h>
typedef const void *CVP;

typedef int (*_OpenCSTLCompareFunc)(CVP a, CVP b);

#define _SAFE_COMPARE(TYPE,X,Y)  (*(TYPE *) (X) > *(TYPE *) (Y)) - (*(TYPE *) (X) < *(TYPE *) (Y))

int less_int32(CVP a, CVP b) {
    return _SAFE_COMPARE(int, a, b);
}

int less_uint32(CVP a, CVP b) {
    return _SAFE_COMPARE(unsigned int, a, b);
}

int less_int64(CVP a, CVP b) {
    return _SAFE_COMPARE(long long, a, b);
}

int less_uint64(CVP a, CVP b) {
    return _SAFE_COMPARE(unsigned long long, a, b);
}

int less_float(CVP a, CVP b) {
    return (*(float *) a > *(float *) b) - (*(float *) a < *(float *) b);
}

int less_double(CVP a, CVP b) {
    return (*(double *) a > *(double *) b) - (*(double *) a < *(double *) b);
}

int less_string(CVP a, CVP b) {
    return strcmp(*(char **) a, *(char **) b);
}

int greater_int32(CVP a, CVP b) {
    return less_int32(b, a);
}

int greater_uint32(CVP a, CVP b) {
    return less_uint32(b, a);
}

int greater_int64(CVP a, CVP b) {
    return less_int64(b, a);
}

int greater_uint64(CVP a, CVP b) {
    return less_uint64(b, a);
}

int greater_float(CVP a, CVP b) {
    return less_float(b, a);
}

int greater_double(CVP a, CVP b) {
    return less_double(b, a);
}

int greater_string(CVP a, CVP b) {
    return less_string(b, a);
}

_OpenCSTLCompareFunc CSTL_LESS(const char *type_str) {
    while (*type_str == ' ') type_str++;

    const char *end = type_str + strlen(type_str);
    while (end > type_str && *(end - 1) == ' ') end--;

    char buf[256];
    size_t len = end - type_str;
    if (len >= sizeof(buf)) return NULL;

    memcpy(buf, type_str, len);
    buf[len] = '\0';

    if (strcmp(buf, "float") == 0) return less_float;
    if (strcmp(buf, "double") == 0) return less_double;
    if (strcmp(buf, "int") == 0) return less_int32;
    if (strcmp(buf, "long long") == 0) return less_int64;
    if (strcmp(buf, "unsigned long long") == 0) return less_uint64;
    if (strcmp(buf, "unsigned int") == 0) return less_uint32;
    if (strcmp(buf, "char*") == 0) return less_string;
    if (strcmp(buf, "char *") == 0) return less_string;

    return NULL;
}

_OpenCSTLCompareFunc CSTL_GREATER(const char *type_str) {
    while (*type_str == ' ') type_str++;

    const char *end = type_str + strlen(type_str);
    while (end > type_str && *(end - 1) == ' ') end--;

    char buf[256];
    size_t len = end - type_str;
    if (len >= sizeof(buf)) return NULL;

    memcpy(buf, type_str, len);
    buf[len] = '\0';

    if (strcmp(buf, "float") == 0) return greater_float;
    if (strcmp(buf, "double") == 0) return greater_double;
    if (strcmp(buf, "int") == 0) return greater_int32;
    if (strcmp(buf, "long long") == 0) return greater_int64;
    if (strcmp(buf, "unsigned long long") == 0) return greater_uint64;
    if (strcmp(buf, "unsigned int") == 0) return greater_uint32;
    if (strcmp(buf, "char*") == 0) return greater_string;
    if (strcmp(buf, "char *") == 0) return greater_string;

    return NULL;
}

#define LESS(TYPE)    CSTL_LESS(#TYPE)
#define GREATER(TYPE) CSTL_GREATER(#TYPE)


int _memcmp1(const void *a, const void *b) { return memcmp(a, b, 1); }
int _memcmp2(const void *a, const void *b) { return memcmp(a, b, 2); }
int _memcmp3(const void *a, const void *b) { return memcmp(a, b, 3); }
int _memcmp4(const void *a, const void *b) { return memcmp(a, b, 4); }
int _memcmp5(const void *a, const void *b) { return memcmp(a, b, 5); }
int _memcmp6(const void *a, const void *b) { return memcmp(a, b, 6); }
int _memcmp7(const void *a, const void *b) { return memcmp(a, b, 7); }
int _memcmp8(const void *a, const void *b) { return memcmp(a, b, 8); }
int _memcmp9(const void *a, const void *b) { return memcmp(a, b, 9); }
int _memcmp10(const void *a, const void *b) { return memcmp(a, b, 10); }
int _memcmp11(const void *a, const void *b) { return memcmp(a, b, 11); }
int _memcmp12(const void *a, const void *b) { return memcmp(a, b, 12); }
int _memcmp13(const void *a, const void *b) { return memcmp(a, b, 13); }
int _memcmp14(const void *a, const void *b) { return memcmp(a, b, 14); }
int _memcmp15(const void *a, const void *b) { return memcmp(a, b, 15); }
int _memcmp16(const void *a, const void *b) { return memcmp(a, b, 16); }
int _memcmp17(const void *a, const void *b) { return memcmp(a, b, 17); }
int _memcmp18(const void *a, const void *b) { return memcmp(a, b, 18); }
int _memcmp19(const void *a, const void *b) { return memcmp(a, b, 19); }
int _memcmp20(const void *a, const void *b) { return memcmp(a, b, 20); }
int _memcmp21(const void *a, const void *b) { return memcmp(a, b, 21); }
int _memcmp22(const void *a, const void *b) { return memcmp(a, b, 22); }
int _memcmp23(const void *a, const void *b) { return memcmp(a, b, 23); }
int _memcmp24(const void *a, const void *b) { return memcmp(a, b, 24); }
int _memcmp25(const void *a, const void *b) { return memcmp(a, b, 25); }
int _memcmp26(const void *a, const void *b) { return memcmp(a, b, 26); }
int _memcmp27(const void *a, const void *b) { return memcmp(a, b, 27); }
int _memcmp28(const void *a, const void *b) { return memcmp(a, b, 28); }
int _memcmp29(const void *a, const void *b) { return memcmp(a, b, 29); }
int _memcmp30(const void *a, const void *b) { return memcmp(a, b, 30); }
int _memcmp31(const void *a, const void *b) { return memcmp(a, b, 31); }
int _memcmp32(const void *a, const void *b) { return memcmp(a, b, 32); }
int _memcmp33(const void *a, const void *b) { return memcmp(a, b, 33); }
int _memcmp34(const void *a, const void *b) { return memcmp(a, b, 34); }
int _memcmp35(const void *a, const void *b) { return memcmp(a, b, 35); }
int _memcmp36(const void *a, const void *b) { return memcmp(a, b, 36); }
int _memcmp37(const void *a, const void *b) { return memcmp(a, b, 37); }
int _memcmp38(const void *a, const void *b) { return memcmp(a, b, 38); }
int _memcmp39(const void *a, const void *b) { return memcmp(a, b, 39); }
int _memcmp40(const void *a, const void *b) { return memcmp(a, b, 40); }
int _memcmp41(const void *a, const void *b) { return memcmp(a, b, 41); }
int _memcmp42(const void *a, const void *b) { return memcmp(a, b, 42); }
int _memcmp43(const void *a, const void *b) { return memcmp(a, b, 43); }
int _memcmp44(const void *a, const void *b) { return memcmp(a, b, 44); }
int _memcmp45(const void *a, const void *b) { return memcmp(a, b, 45); }
int _memcmp46(const void *a, const void *b) { return memcmp(a, b, 46); }
int _memcmp47(const void *a, const void *b) { return memcmp(a, b, 47); }
int _memcmp48(const void *a, const void *b) { return memcmp(a, b, 48); }
int _memcmp49(const void *a, const void *b) { return memcmp(a, b, 49); }
int _memcmp50(const void *a, const void *b) { return memcmp(a, b, 50); }
int _memcmp51(const void *a, const void *b) { return memcmp(a, b, 51); }
int _memcmp52(const void *a, const void *b) { return memcmp(a, b, 52); }
int _memcmp53(const void *a, const void *b) { return memcmp(a, b, 53); }
int _memcmp54(const void *a, const void *b) { return memcmp(a, b, 54); }
int _memcmp55(const void *a, const void *b) { return memcmp(a, b, 55); }
int _memcmp56(const void *a, const void *b) { return memcmp(a, b, 56); }
int _memcmp57(const void *a, const void *b) { return memcmp(a, b, 57); }
int _memcmp58(const void *a, const void *b) { return memcmp(a, b, 58); }
int _memcmp59(const void *a, const void *b) { return memcmp(a, b, 59); }
int _memcmp60(const void *a, const void *b) { return memcmp(a, b, 60); }
int _memcmp61(const void *a, const void *b) { return memcmp(a, b, 61); }
int _memcmp62(const void *a, const void *b) { return memcmp(a, b, 62); }
int _memcmp63(const void *a, const void *b) { return memcmp(a, b, 63); }
int _memcmp64(const void *a, const void *b) { return memcmp(a, b, 64); }
int _memcmp65(const void *a, const void *b) { return memcmp(a, b, 65); }
int _memcmp66(const void *a, const void *b) { return memcmp(a, b, 66); }
int _memcmp67(const void *a, const void *b) { return memcmp(a, b, 67); }
int _memcmp68(const void *a, const void *b) { return memcmp(a, b, 68); }
int _memcmp69(const void *a, const void *b) { return memcmp(a, b, 69); }
int _memcmp70(const void *a, const void *b) { return memcmp(a, b, 70); }
int _memcmp71(const void *a, const void *b) { return memcmp(a, b, 71); }
int _memcmp72(const void *a, const void *b) { return memcmp(a, b, 72); }
int _memcmp73(const void *a, const void *b) { return memcmp(a, b, 73); }
int _memcmp74(const void *a, const void *b) { return memcmp(a, b, 74); }
int _memcmp75(const void *a, const void *b) { return memcmp(a, b, 75); }
int _memcmp76(const void *a, const void *b) { return memcmp(a, b, 76); }
int _memcmp77(const void *a, const void *b) { return memcmp(a, b, 77); }
int _memcmp78(const void *a, const void *b) { return memcmp(a, b, 78); }
int _memcmp79(const void *a, const void *b) { return memcmp(a, b, 79); }
int _memcmp80(const void *a, const void *b) { return memcmp(a, b, 80); }
int _memcmp81(const void *a, const void *b) { return memcmp(a, b, 81); }
int _memcmp82(const void *a, const void *b) { return memcmp(a, b, 82); }
int _memcmp83(const void *a, const void *b) { return memcmp(a, b, 83); }
int _memcmp84(const void *a, const void *b) { return memcmp(a, b, 84); }
int _memcmp85(const void *a, const void *b) { return memcmp(a, b, 85); }
int _memcmp86(const void *a, const void *b) { return memcmp(a, b, 86); }
int _memcmp87(const void *a, const void *b) { return memcmp(a, b, 87); }
int _memcmp88(const void *a, const void *b) { return memcmp(a, b, 88); }
int _memcmp89(const void *a, const void *b) { return memcmp(a, b, 89); }
int _memcmp90(const void *a, const void *b) { return memcmp(a, b, 90); }
int _memcmp91(const void *a, const void *b) { return memcmp(a, b, 91); }
int _memcmp92(const void *a, const void *b) { return memcmp(a, b, 92); }
int _memcmp93(const void *a, const void *b) { return memcmp(a, b, 93); }
int _memcmp94(const void *a, const void *b) { return memcmp(a, b, 94); }
int _memcmp95(const void *a, const void *b) { return memcmp(a, b, 95); }
int _memcmp96(const void *a, const void *b) { return memcmp(a, b, 96); }
int _memcmp97(const void *a, const void *b) { return memcmp(a, b, 97); }
int _memcmp98(const void *a, const void *b) { return memcmp(a, b, 98); }
int _memcmp99(const void *a, const void *b) { return memcmp(a, b, 99); }
int _memcmp100(const void *a, const void *b) { return memcmp(a, b, 100); }
int _memcmp101(const void *a, const void *b) { return memcmp(a, b, 101); }
int _memcmp102(const void *a, const void *b) { return memcmp(a, b, 102); }
int _memcmp103(const void *a, const void *b) { return memcmp(a, b, 103); }
int _memcmp104(const void *a, const void *b) { return memcmp(a, b, 104); }
int _memcmp105(const void *a, const void *b) { return memcmp(a, b, 105); }
int _memcmp106(const void *a, const void *b) { return memcmp(a, b, 106); }
int _memcmp107(const void *a, const void *b) { return memcmp(a, b, 107); }
int _memcmp108(const void *a, const void *b) { return memcmp(a, b, 108); }
int _memcmp109(const void *a, const void *b) { return memcmp(a, b, 109); }
int _memcmp110(const void *a, const void *b) { return memcmp(a, b, 110); }
int _memcmp111(const void *a, const void *b) { return memcmp(a, b, 111); }
int _memcmp112(const void *a, const void *b) { return memcmp(a, b, 112); }
int _memcmp113(const void *a, const void *b) { return memcmp(a, b, 113); }
int _memcmp114(const void *a, const void *b) { return memcmp(a, b, 114); }
int _memcmp115(const void *a, const void *b) { return memcmp(a, b, 115); }
int _memcmp116(const void *a, const void *b) { return memcmp(a, b, 116); }
int _memcmp117(const void *a, const void *b) { return memcmp(a, b, 117); }
int _memcmp118(const void *a, const void *b) { return memcmp(a, b, 118); }
int _memcmp119(const void *a, const void *b) { return memcmp(a, b, 119); }
int _memcmp120(const void *a, const void *b) { return memcmp(a, b, 120); }
int _memcmp121(const void *a, const void *b) { return memcmp(a, b, 121); }
int _memcmp122(const void *a, const void *b) { return memcmp(a, b, 122); }
int _memcmp123(const void *a, const void *b) { return memcmp(a, b, 123); }
int _memcmp124(const void *a, const void *b) { return memcmp(a, b, 124); }
int _memcmp125(const void *a, const void *b) { return memcmp(a, b, 125); }
int _memcmp126(const void *a, const void *b) { return memcmp(a, b, 126); }
int _memcmp127(const void *a, const void *b) { return memcmp(a, b, 127); }
int _memcmp128(const void *a, const void *b) { return memcmp(a, b, 128); }
_OpenCSTLCompareFunc _memcmp_funcs[128 + 1] = {
    NULL,
    _memcmp1,
    _memcmp2,
    _memcmp3,
    _memcmp4,
    _memcmp5,
    _memcmp6,
    _memcmp7,
    _memcmp8,
    _memcmp9,
    _memcmp10,
    _memcmp11,
    _memcmp12,
    _memcmp13,
    _memcmp14,
    _memcmp15,
    _memcmp16,
    _memcmp17,
    _memcmp18,
    _memcmp19,
    _memcmp20,
    _memcmp21,
    _memcmp22,
    _memcmp23,
    _memcmp24,
    _memcmp25,
    _memcmp26,
    _memcmp27,
    _memcmp28,
    _memcmp29,
    _memcmp30,
    _memcmp31,
    _memcmp32,
    _memcmp33,
    _memcmp34,
    _memcmp35,
    _memcmp36,
    _memcmp37,
    _memcmp38,
    _memcmp39,
    _memcmp40,
    _memcmp41,
    _memcmp42,
    _memcmp43,
    _memcmp44,
    _memcmp45,
    _memcmp46,
    _memcmp47,
    _memcmp48,
    _memcmp49,
    _memcmp50,
    _memcmp51,
    _memcmp52,
    _memcmp53,
    _memcmp54,
    _memcmp55,
    _memcmp56,
    _memcmp57,
    _memcmp58,
    _memcmp59,
    _memcmp60,
    _memcmp61,
    _memcmp62,
    _memcmp63,
    _memcmp64,
    _memcmp65,
    _memcmp66,
    _memcmp67,
    _memcmp68,
    _memcmp69,
    _memcmp70,
    _memcmp71,
    _memcmp72,
    _memcmp73,
    _memcmp74,
    _memcmp75,
    _memcmp76,
    _memcmp77,
    _memcmp78,
    _memcmp79,
    _memcmp80,
    _memcmp81,
    _memcmp82,
    _memcmp83,
    _memcmp84,
    _memcmp85,
    _memcmp86,
    _memcmp87,
    _memcmp88,
    _memcmp89,
    _memcmp90,
    _memcmp91,
    _memcmp92,
    _memcmp93,
    _memcmp94,
    _memcmp95,
    _memcmp96,
    _memcmp97,
    _memcmp98,
    _memcmp99,
    _memcmp100,
    _memcmp101,
    _memcmp102,
    _memcmp103,
    _memcmp104,
    _memcmp105,
    _memcmp106,
    _memcmp107,
    _memcmp108,
    _memcmp109,
    _memcmp110,
    _memcmp111,
    _memcmp112,
    _memcmp113,
    _memcmp114,
    _memcmp115,
    _memcmp116,
    _memcmp117,
    _memcmp118,
    _memcmp119,
    _memcmp120,
    _memcmp121,
    _memcmp122,
    _memcmp123,
    _memcmp124,
    _memcmp125,
    _memcmp126,
    _memcmp127,
    _memcmp128,
};


#endif //OPENCSTL_COMPARE_H
