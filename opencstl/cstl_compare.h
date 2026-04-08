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

typedef int (*CompareFunc)(CVP a, CVP b);

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

CompareFunc CSTL_LESS(const char *type_str) {
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

CompareFunc CSTL_GREATER(const char *type_str) {
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
#endif //OPENCSTL_COMPARE_H
