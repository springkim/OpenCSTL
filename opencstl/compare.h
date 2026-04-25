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
#include "types.h"
#include <float.h>
#include "memcmps.h"
typedef const void *CVP;


#define _SAFE_COMPARE(TYPE,X,Y)  (*(TYPE *) (X) > *(TYPE *) (Y)) - (*(TYPE *) (X) < *(TYPE *) (Y))

static int less_int32(CVP a, CVP b) {
    return _SAFE_COMPARE(int, a, b);
}

static int less_uint32(CVP a, CVP b) {
    return _SAFE_COMPARE(unsigned int, a, b);
}

static int less_int64(CVP a, CVP b) {
    return _SAFE_COMPARE(long long, a, b);
}

static int less_uint64(CVP a, CVP b) {
    return _SAFE_COMPARE(unsigned long long, a, b);
}

static int less_float(CVP a, CVP b) {
    return (*(float *) a > *(float *) b) - (*(float *) a < *(float *) b);
}

static int less_double(CVP a, CVP b) {
    return (*(double *) a > *(double *) b) - (*(double *) a < *(double *) b);
}

static int less_string(CVP a, CVP b) {
    return strcmp(*(char **) a, *(char **) b);
}

static int greater_int32(CVP a, CVP b) {
    return less_int32(b, a);
}

static int greater_uint32(CVP a, CVP b) {
    return less_uint32(b, a);
}

static int greater_int64(CVP a, CVP b) {
    return less_int64(b, a);
}

static int greater_uint64(CVP a, CVP b) {
    return less_uint64(b, a);
}

static int greater_float(CVP a, CVP b) {
    return less_float(b, a);
}

static int greater_double(CVP a, CVP b) {
    return less_double(b, a);
}

static int greater_string(CVP a, CVP b) {
    return less_string(b, a);
}

static CSTL_COMPARE CSTL_LESS(const char *type_str) {
    while (*type_str == ' ') type_str++;

    const char *end = type_str + strlen(type_str);
    while (end > type_str && *(end - 1) == ' ') end--;

    char buf[256];
    size_type64 len = end - type_str;
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

static CSTL_COMPARE CSTL_GREATER(const char *type_str) {
    while (*type_str == ' ') type_str++;

    const char *end = type_str + strlen(type_str);
    while (end > type_str && *(end - 1) == ' ') end--;

    char buf[256];
    size_type64 len = end - type_str;
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


// ███████╗░██████╗░██╗░░░██╗░█████╗░██╗░░░░░░██████╗
// ██╔════╝██╔═══██╗██║░░░██║██╔══██╗██║░░░░░██╔════╝
// █████╗░░██║██╗██║██║░░░██║███████║██║░░░░░╚█████╗░
// ██╔══╝░░╚██████╔╝██║░░░██║██╔══██║██║░░░░░░╚═══██╗
// ███████╗░╚═██╔═╝░╚██████╔╝██║░░██║███████╗██████╔╝
// ╚══════╝░░░╚═╝░░░░╚═════╝░╚═╝░░╚═╝╚══════╝╚═════╝░
#define CSTL_FLOAT_EPS   1e-6f
#define CSTL_DOUBLE_EPS  1e-9

static int _cstl_equals_float(CVP a, CVP b, size_t dummy) {
    float a1 = *(float *) a;
    float b1 = *(float *) b;

    float diff = a1 - b1;
    if (diff < 0)diff = -diff;
    return diff < CSTL_FLOAT_EPS;
}

static int _cstl_equals_double(CVP a, CVP b, size_t dummy) {
    double a1 = *(double *) a;
    double b1 = *(double *) b;
    double diff = a1 - b1;
    if (diff < 0)diff = -diff;
    return diff < CSTL_DOUBLE_EPS;
}

static int _cstl_equals_str(CVP a, CVP b, size_t dummy) {
    return strcmp(*(char **) a, *(char **) b) == 0;
}

static CSTL_COMPARE_BYTES CSTL_EQUALS(const char *type_str) {
    while (*type_str == ' ') type_str++;

    const char *end = type_str + strlen(type_str);
    while (end > type_str && *(end - 1) == ' ') end--;

    char buf[256];
    size_type64 len = end - type_str;
    if (len >= sizeof(buf)) return NULL;

    memcpy(buf, type_str, len);
    buf[len] = '\0';

    if (strcmp(buf, "float") == 0) return _cstl_equals_float;
    if (strcmp(buf, "double") == 0) return _cstl_equals_double;
    if (strcmp(buf, "char*") == 0) return _cstl_equals_str;
    if (strcmp(buf, "char *") == 0) return _cstl_equals_str;


    return memcmp;
}
#endif //OPENCSTL_COMPARE_H
