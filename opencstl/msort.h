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


static void merge(char *arr, size_t len1, size_t len2, size_t sz,
                  int (*cmp)(const void *, const void *), char *buf) {
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
            memcpy(d, c1, (size_t)(e1 - c1));
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

static void msort(void *base, size_t nmemb, size_t size,
                  int (*compar)(const void *, const void *)) {
    if (nmemb < 2) return;
    char *arr = (char *) base;
    size_t sz = size;
    for (size_t i = 0; i < nmemb; i += MSORT_ISORT_THRESH) {
        size_t blk = nmemb - i;
        if (blk > MSORT_ISORT_THRESH) blk = MSORT_ISORT_THRESH;
        isort(arr + i * sz, blk, sz, compar);
    }
    char *buf = (char *) calloc(((nmemb + 1) / 2), sz);
    if (!buf) return;
    for (size_t width = MSORT_ISORT_THRESH; width < nmemb; width *= 2) {
        for (size_t i = 0; i + width < nmemb; i += 2 * width) {
            size_t len1 = width;
            size_t len2 = nmemb - i - width;
            if (len2 > width) len2 = width;
            merge(arr + i * sz, len1, len2, sz, compar, buf);
        }
    }
    free(buf);
}
#undef MSORT_ISORT_THRESH
#endif
