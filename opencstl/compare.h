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

int IntCmp(CVP a, CVP b) {
    return (*(int *) a > *(int *) b) - (*(int *) a < *(int *) b);
}

int Int64Cmp(CVP a, CVP b) {
    return (*(long long *) a > *(long long *) b) - (*(long long *) a < *(long long *) b);
}

int FloatCmp(CVP a, CVP b) {
    return (*(float *) a > *(float *) b) - (*(float *) a < *(float *) b);
}

int DoubleCmp(CVP a, CVP b) {
    return (*(double *) a > *(double *) b) - (*(double *) a < *(double *) b);
}

int StringCmp(CVP a, CVP b) {
    return strcmp((char *) a, (char *) b);
}

#define GetCompare(T) _Generic((T){0}, \
int:     IntCmp,                    \
float:   FloatCmp,                  \
double:  DoubleCmp,                 \
char*:   StringCmp                  \
)

CompareFunc Compare(const char *type_str) {
    while (*type_str == ' ') type_str++;

    const char *end = type_str + strlen(type_str);
    while (end > type_str && *(end - 1) == ' ') end--;

    char buf[256];
    size_t len = end - type_str;
    if (len >= sizeof(buf)) return NULL;

    memcpy(buf, type_str, len);
    buf[len] = '\0';

    if (strcmp(buf, "float") == 0) return FloatCmp;
    if (strcmp(buf, "double") == 0) return DoubleCmp;
    if (strcmp(buf, "int") == 0) return IntCmp;
    if (strcmp(buf, "char*") == 0)return StringCmp;
    if (strcmp(buf, "char *") == 0)return StringCmp;

    return NULL;
}

#define COMPARE(TYPE) Compare(#TYPE)
#endif //OPENCSTL_COMPARE_H
