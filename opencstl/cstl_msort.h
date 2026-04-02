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
#if !defined(_OPENCSTL_SORT_H)
#define _OPENCSTL_SORT_H
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>


static void merge(const char *left, const char *mid, const char *right, const char *end, char *dest, size_t size,
                  int (*compar)(const void *, const void *)) {
    if (left == mid || mid == end) {
        memcpy(dest, left, end - left);
        return;
    }
    if (compar(mid - size, mid) <= 0) {
        memcpy(dest, left, end - left);
        return;
    }
    if (size == 1) {
        while (left < mid && right < end)
            *dest++ = (compar(left, right) < 0) ? *left++ : *right++;
        if (left < mid)
            memcpy(dest, left, mid - left);
        else
            memcpy(dest, right, end - right);
        return;
    }
    while (left < mid && right < end) {
        if (compar(left, right) < 0) {
            memcpy(dest, left, size);
            left += size;
        } else {
            memcpy(dest, right, size);
            right += size;
        }
        dest += size;
    }
    if (left < mid)
        memcpy(dest, left, mid - left);
    else
        memcpy(dest, right, end - right);
}

static void msort(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    if (nmemb < 2)return;
    char *src = base;
    char *buf = malloc(nmemb * size);
    if (!buf) return;
    for (size_t width = 1; width < nmemb; width *= 2) {
        for (size_t i = 0; i < nmemb; i += 2 * width) {
            size_t left = i;
            size_t mid = (i + width < nmemb) ? i + width : nmemb;
            size_t right = (i + 2 * width < nmemb) ? i + 2 * width : nmemb;
            merge(src + left * size, src + mid * size, src + mid * size, src + right * size,
                  buf + left * size, size, compar);
        }
        char *tmp = src;
        src = buf;
        buf = tmp;
    }
    if (src != base) {
        memcpy(base, src, nmemb * size);
        free(src);
    } else {
        free(buf);
    }
}

#define sort        qsort
#define stable_sort msort
#endif //_OPENCSTL_SORT_H
