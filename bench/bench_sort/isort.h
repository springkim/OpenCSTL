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
#if !defined(_OPENCSTL_ISORT_H)
#define _OPENCSTL_ISORT_H
#include <stdlib.h>
#include <string.h>

static void isort(char *arr, size_t n, size_t sz, int (*cmp)(const void *, const void *)) {
    char sbuf[256];
    char *tmp = (sz <= sizeof(sbuf)) ? sbuf : (char *) malloc(sz);
    for (size_t i = 1; i < n; i++) {
        memcpy(tmp, arr + i * sz, sz);
        size_t lo = 0, hi = i;
        while (lo < hi) {
            size_t mid = lo + ((hi - lo) >> 1);
            if (cmp(tmp, arr + mid * sz) < 0) hi = mid;
            else lo = mid + 1;
        }
        if (lo < i) {
            memmove(arr + (lo + 1) * sz, arr + lo * sz, (i - lo) * sz);
            memcpy(arr + lo * sz, tmp, sz);
        }
    }
    if (tmp != sbuf) free(tmp);
}
#endif
