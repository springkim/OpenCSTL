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

#ifndef OPENCSTL_RSORT_H
#define OPENCSTL_RSORT_H
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

static void rsort32(int32_t *__base, size_type64 n) {
    if (__base == NULL || n <= 1) {
        return;
    }

    int32_t *tmp = (int32_t *) malloc(n * sizeof(int32_t));
    if (tmp == NULL) {
        return;
    }

    {
        size_type64 count[256];
        size_type64 pass, i;
        uint32_t sign_mask = (uint32_t) 1u << (sizeof(int32_t) * CHAR_BIT - 1);

        for (pass = 0; pass < sizeof(int32_t); ++pass) {
            size_type64 shift = pass * 8;

            for (i = 0; i < 256; ++i) {
                count[i] = 0;
            }

            for (i = 0; i < n; ++i) {
                uint32_t key = ((uint32_t) __base[i]) ^ sign_mask;
                uint32_t digit = (key >> shift) & 0xFFu;
                count[digit]++;
            }

            for (i = 1; i < 256; ++i) {
                count[i] += count[i - 1];
            }

            for (i = n; i > 0; --i) {
                uint32_t key = ((uint32_t) __base[i - 1]) ^ sign_mask;
                uint32_t digit = (key >> shift) & 0xFFu;
                tmp[--count[digit]] = __base[i - 1];
            }

            for (i = 0; i < n; ++i) {
                __base[i] = tmp[i];
            }
        }
    }

    free(tmp);
}

static void rsort64(int64_t *__base, size_type64 n) {
    if (__base == NULL || n <= 1) {
        return;
    }

    int64_t *tmp = (int64_t *) malloc(n * sizeof(int64_t));
    if (tmp == NULL) {
        return;
    }

    {
        size_type64 count[256];
        size_type64 pass, i;
        uint64_t sign_mask = (uint64_t) 1ULL << (sizeof(int64_t) * CHAR_BIT - 1);

        for (pass = 0; pass < sizeof(int64_t); ++pass) {
            size_type64 shift = pass * 8;

            for (i = 0; i < 256; ++i) {
                count[i] = 0;
            }

            for (i = 0; i < n; ++i) {
                uint64_t key = ((uint64_t) __base[i]) ^ sign_mask;
                uint32_t digit = (uint32_t) ((key >> shift) & 0xFFULL);
                count[digit]++;
            }

            for (i = 1; i < 256; ++i) {
                count[i] += count[i - 1];
            }

            for (i = n; i > 0; --i) {
                uint64_t key = ((uint64_t) __base[i - 1]) ^ sign_mask;
                uint32_t digit = (uint32_t) ((key >> shift) & 0xFFULL);
                tmp[--count[digit]] = __base[i - 1];
            }

            for (i = 0; i < n; ++i) {
                __base[i] = tmp[i];
            }
        }
    }

    free(tmp);
}


#define RSORT_STATIC_TYPECHECK(base) \
    ((void)sizeof(char[(sizeof(*(base)) == 4 || sizeof(*(base)) == 8) ? 1 : -1]))


#define rsort(base, n)                                                     \
    do {                                                                   \
        RSORT_STATIC_TYPECHECK(base);                                      \
        if (sizeof(*(base)) == sizeof(int32_t)) {                          \
            rsort32((int32_t *)(base), (n));                               \
        } else {                                                           \
            rsort64((int64_t *)(base), (n));                               \
        }                                                                  \
    } while (0)

#endif //OPENCSTL_RSORT_H
