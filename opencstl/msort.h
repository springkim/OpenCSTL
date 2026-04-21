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
#pragma once
#if !defined(_OPENCSTL_MSORT_H)
#define _OPENCSTL_MSORT_H
#include <stdlib.h>
#include <string.h>
#include "isort.h"
#define MSORT_ISORT_THRESH 32


static void msort_merge(char *arr, size_t len1, size_t len2, size_t sz,
                        CSTL_COMPARE cmp, char *buf) {
    if (cmp(arr + (len1 - 1) * sz, arr + len1 * sz) <= 0) return;
    if (len1 <= len2) {
        memcpy(buf, arr, len1 * sz);
        char *c1 = buf, *e1 = buf + len1 * sz;
        char *c2 = arr + len1 * sz, *e2 = c2 + len2 * sz;
        char *d = arr;
        while (c1 < e1 && c2 < e2) {
            if (cmp(c2, c1) < 0) {
                memcpy(d, c2, sz);
                c2 += sz;
            } else {
                memcpy(d, c1, sz);
                c1 += sz;
            }
            d += sz;
        }
        if (c1 < e1)
            memcpy(d, c1, (size_t) (e1 - c1));
    } else {
        memcpy(buf, arr + len1 * sz, len2 * sz);
        size_t i = len1, j = len2, k = len1 + len2;
        while (i > 0 && j > 0) {
            k--;
            if (cmp(buf + (j - 1) * sz, arr + (i - 1) * sz) < 0) {
                memcpy(arr + k * sz, arr + (i - 1) * sz, sz);
                i--;
            } else {
                memcpy(arr + k * sz, buf + (j - 1) * sz, sz);
                j--;
            }
        }
        if (j > 0)
            memcpy(arr, buf, j * sz);
    }
}

static void msort(void *base, size_t number, size_t width, CSTL_COMPARE compare) {
    if (number < 2) return;
    char *arr = (char *) base;
    size_t sz = width;
    for (size_t i = 0; i < number; i += MSORT_ISORT_THRESH) {
        size_t blk = number - i;
        if (blk > MSORT_ISORT_THRESH) blk = MSORT_ISORT_THRESH;
        isort(arr + i * sz, blk, sz, compare);
    }
    char *buf = (char *) calloc(((number + 1) / 2), sz);
    if (!buf) return;
    for (size_t mb = MSORT_ISORT_THRESH; mb < number; mb *= 2) {
        for (size_t i = 0; i + mb < number; i += 2 * mb) {
            size_t len1 = mb;
            size_t len2 = number - i - mb;
            if (len2 > mb) len2 = mb;
            msort_merge(arr + i * sz, len1, len2, sz, compare, buf);
        }
    }
    free(buf);
}

#endif
