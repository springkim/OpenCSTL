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

static void isort(void *__base, size_t __nel, size_t __width, int (*__compar)(const void *, const void *)) {
    char sbuf[256];
    char *arr = (char *) __base;
    char *tmp = (__width <= sizeof(sbuf)) ? sbuf : (char *) malloc(__width);
    for (size_t i = 1; i < __nel; i++) {
        memcpy(tmp, arr + i * __width, __width);
        size_t lo = 0, hi = i;
        while (lo < hi) {
            size_t mid = lo + ((hi - lo) >> 1);
            if (__compar(tmp, arr + mid * __width) < 0) hi = mid;
            else lo = mid + 1;
        }
        if (lo < i) {
            memmove(arr + (lo + 1) * __width, arr + lo * __width, (i - lo) * __width);
            memcpy(arr + lo * __width, tmp, __width);
        }
    }
    if (tmp != sbuf) free(tmp);
}

// #if defined(__GNUC__) || defined(__clang__)
// #define SORT_LIKELY(x)   __builtin_expect(!!(x), 1)
// #define SORT_NOINLINE     __attribute__((noinline))
// #else
// #define SORT_LIKELY(x)   (x)
// #define SORT_NOINLINE
// #endif
//
// typedef int (*sort_cmp)(const void *, const void *);
//
// // 4-byte 특화 (int, float)
// static SORT_NOINLINE void isort_w4(void *base, size_t nel, sort_cmp cmp) {
//     unsigned int *a = (unsigned int *) base;
//     for (size_t i = 1; i < nel; i++) {
//         unsigned int tmp = a[i];
//         if (SORT_LIKELY(cmp(&tmp, &a[i - 1]) >= 0)) continue;
//         size_t lo = 0, hi = i;
//         while (lo < hi) {
//             size_t mid = lo + ((hi - lo) >> 1);
//             if (cmp(&tmp, &a[mid]) < 0) hi = mid;
//             else lo = mid + 1;
//         }
//         for (size_t j = i; j > lo; j--)
//             a[j] = a[j - 1];
//         a[lo] = tmp;
//     }
// }
//
// // 8-byte 특화 (int64, double, pointer)
// static SORT_NOINLINE void isort_w8(void *base, size_t nel, sort_cmp cmp) {
//     unsigned long long *a = (unsigned long long *) base;
//     for (size_t i = 1; i < nel; i++) {
//         unsigned long long tmp = a[i];
//         if (SORT_LIKELY(cmp(&tmp, &a[i - 1]) >= 0)) continue;
//         size_t lo = 0, hi = i;
//         while (lo < hi) {
//             size_t mid = lo + ((hi - lo) >> 1);
//             if (cmp(&tmp, &a[mid]) < 0) hi = mid;
//             else lo = mid + 1;
//         }
//         for (size_t j = i; j > lo; j--)
//             a[j] = a[j - 1];
//         a[lo] = tmp;
//     }
// }
//
// // generic path
// static SORT_NOINLINE void isort_generic(void *base, size_t nel, size_t width, sort_cmp cmp) {
//     char sbuf[256];
//     char *arr = (char *) base;
//     char *tmp = (width <= sizeof(sbuf)) ? sbuf : (char *) malloc(width);
//
//     for (size_t i = 1; i < nel; i++) {
//         char *cur = arr + i * width;
//         memcpy(tmp, cur, width);
//         if (SORT_LIKELY(cmp(tmp, cur - width) >= 0)) continue;
//
//         size_t lo = 0, hi = i;
//         while (lo < hi) {
//             size_t mid = lo + ((hi - lo) >> 1);
//             if (cmp(tmp, arr + mid * width) < 0) hi = mid;
//             else lo = mid + 1;
//         }
//         char *dst = arr + lo * width;
//         memmove(dst + width, dst, (i - lo) * width);
//         memcpy(dst, tmp, width);
//     }
//
//     if (tmp != sbuf) free(tmp);
// }
//
// static void isort(void *base, size_t nel, size_t width, sort_cmp cmp) {
//     if (nel <= 1) return;
//     if (width == 4) {
//         isort_w4(base, nel, cmp);
//         return;
//     }
//     if (width == 8) {
//         isort_w8(base, nel, cmp);
//         return;
//     }
//     isort_generic(base, nel, width, cmp);
// }
#endif
