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
#if !defined(_OPENCSTL_PDQSORT_H)
#define _OPENCSTL_PDQSORT_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "swap.h"
#define PDQ_ISORT_THRESH      24
#define PDQ_NINTHER_THRESH   128
#define PDQ_PARTIAL_LIMIT      8
#define PDQ_MAX_STACK         64


#if defined(_MSC_VER)
#define PDQ_LIKELY(x)   (x)
#define PDQ_UNLIKELY(x) (x)
#else
#define PDQ_LIKELY(x)   __builtin_expect(!!(x), 1)
#define PDQ_UNLIKELY(x) __builtin_expect(!!(x), 0)
#endif

static  void pdq__swap(unsigned char *a, unsigned char *b, size_type64 n) {
    if (PDQ_LIKELY(n == 8)) {
        uint64_t t;
        memcpy(&t, a, 8);
        memcpy(a, b, 8);
        memcpy(b, &t, 8);
    } else if (PDQ_UNLIKELY(n == 4)) {
        uint32_t t;
        memcpy(&t, a, 4);
        memcpy(a, b, 4);
        memcpy(b, &t, 4);
    } else {
        unsigned char t;
        for (size_type64 i = 0; i < n; ++i) {
            t = a[i];
            a[i] = b[i];
            b[i] = t;
        }
    }
}

#define PDQ_ELEM(base, i) ((base) + (i) * sz)

static inline size_type64 pdq_log2(size_type64 n) {
    size_type64 r = 0;
    while (n > 1) {
        n >>= 1;
        ++r;
    }
    return r;
}

static void pdq_isort(unsigned char *base, size_type64 n, size_type64 sz,
                      int (*cmp)(const void *, const void *), unsigned char *tmp) {
    if (n < 2) return;
    for (size_type64 i = 1; i < n; ++i)
        if (cmp(PDQ_ELEM(base, i), base) < 0) {
            pdq__swap(PDQ_ELEM(base, i), base, sz);
        }
    for (size_type64 i = 2; i < n; ++i) {
        unsigned char *c = PDQ_ELEM(base, i);
        if (cmp(c - sz, c) <= 0) continue;
        memcpy(tmp, c, sz);
        unsigned char *j = c;
        do {
            memcpy(j, j - sz, sz);
            j -= sz;
        } while (cmp(j - sz, tmp) > 0);
        memcpy(j, tmp, sz);
    }
}

static void pdq_isort_unguard(unsigned char *base, size_type64 n, size_type64 sz,
                              int (*cmp)(const void *, const void *), unsigned char *tmp) {
    for (size_type64 i = 1; i < n; ++i) {
        unsigned char *c = PDQ_ELEM(base, i);
        if (cmp(c - sz, c) <= 0) continue;
        memcpy(tmp, c, sz);
        unsigned char *j = c;
        do {
            memcpy(j, j - sz, sz);
            j -= sz;
        } while (j > base && cmp(j - sz, tmp) > 0);
        memcpy(j, tmp, sz);
    }
}

static int pdq_partial_isort(unsigned char *base, size_type64 n, size_type64 sz,
                             int (*cmp)(const void *, const void *), unsigned char *tmp) {
    if (n < 2) return 1;
    size_type64 cnt = 0;
    for (size_type64 i = 1; i < n; ++i) {
        unsigned char *c = PDQ_ELEM(base, i);
        if (cmp(c - sz, c) <= 0) continue;
        memcpy(tmp, c, sz);
        unsigned char *j = c;
        do {
            memcpy(j, j - sz, sz);
            j -= sz;
            if (++cnt > PDQ_PARTIAL_LIMIT) return 0;
        } while (j > base && cmp(j - sz, tmp) > 0);
        memcpy(j, tmp, sz);
    }
    return 1;
}

static inline int pdq_s2(unsigned char *base, size_type64 a, size_type64 b,
                         size_type64 sz, int (*cmp)(const void *, const void *)) {
    if (cmp(PDQ_ELEM(base, b), PDQ_ELEM(base, a)) < 0) {
        pdq__swap(PDQ_ELEM(base, a), PDQ_ELEM(base, b), sz);
        return 1;
    }
    return 0;
}

static inline int pdq_s3(unsigned char *base, size_type64 a, size_type64 b, size_type64 c,
                         size_type64 sz, int (*cmp)(const void *, const void *)) {
    int s = pdq_s2(base, a, b, sz, cmp);
    s += pdq_s2(base, b, c, sz, cmp);
    s += pdq_s2(base, a, b, sz, cmp);
    return s;
}

static int pdq_pick_pivot(unsigned char *base, size_type64 n, size_type64 sz,
                          int (*cmp)(const void *, const void *)) {
    size_type64 mid = n >> 1;
    int sw = 0;
    if (n >= PDQ_NINTHER_THRESH) {
        size_type64 s = n >> 3;
        sw += pdq_s3(base, 0, s, s * 2, sz, cmp);
        sw += pdq_s3(base, mid - s, mid, mid + s, sz, cmp);
        sw += pdq_s3(base, n - 1 - s * 2, n - 1 - s, n - 1, sz, cmp);
        sw += pdq_s3(base, s, mid, n - 1 - s, sz, cmp);
    } else {
        sw += pdq_s3(base, 0, mid, n - 1, sz, cmp);
    }
    pdq__swap(base, PDQ_ELEM(base, mid), sz);
    return sw;
}

static void pdq_sift(unsigned char *base, size_type64 root, size_type64 n,
                     size_type64 sz, int (*cmp)(const void *, const void *)) {
    for (;;) {
        size_type64 c = root * 2 + 1;
        if (c >= n) break;
        size_type64 mx = root;
        if (cmp(PDQ_ELEM(base, mx), PDQ_ELEM(base, c)) < 0) mx = c;
        if (c + 1 < n && cmp(PDQ_ELEM(base, mx), PDQ_ELEM(base, c + 1)) < 0)
            mx = c + 1;
        if (mx == root) break;
        pdq__swap(PDQ_ELEM(base, root), PDQ_ELEM(base, mx), sz);
        root = mx;
    }
}

static void pdq_heap(unsigned char *base, size_type64 n, size_type64 sz, int (*cmp)(const void *, const void *)) {
    if (n < 2) return;
    for (size_type64 i = n / 2; i-- > 0;) pdq_sift(base, i, n, sz, cmp);
    for (size_type64 e = n; e-- > 1;) {
        pdq__swap(base, PDQ_ELEM(base, e), sz);
        pdq_sift(base, 0, e, sz, cmp);
    }
}

static inline uint64_t pdq_rng(uint64_t *s) {
    uint64_t x = *s;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return (*s = x);
}

static void pdq_break(unsigned char *base, size_type64 n, size_type64 sz, uint64_t *rs) {
    if (n < 8) return;
    size_type64 h = n >> 1, q = n >> 2;
    pdq__swap(PDQ_ELEM(base, q), PDQ_ELEM(base, q + pdq_rng(rs) % (h?h:1)), sz);
    pdq__swap(PDQ_ELEM(base, h), PDQ_ELEM(base, q + pdq_rng(rs) % (h?h:1)), sz);
    pdq__swap(PDQ_ELEM(base, q + h), PDQ_ELEM(base, q + pdq_rng(rs) % (h?h:1)), sz);
}

static inline size_type64 pdq_part_r(unsigned char *base, size_type64 n, size_type64 sz,
                                int (*cmp)(const void *, const void *), unsigned char *piv,
                                int *ap) {
    unsigned char *lo = base + sz;
    unsigned char *hi = base + (n - 1) * sz;
    while (lo <= hi && cmp(lo, piv) < 0) lo += sz;
    while (lo <= hi && cmp(hi, piv) >= 0) hi -= sz;
    int was_partitioned = (lo > hi);
    while (lo < hi) {
        pdq__swap(lo, hi, sz);
        lo += sz;
        hi -= sz;
        while (cmp(lo, piv) < 0) lo += sz;
        while (cmp(hi, piv) >= 0) hi -= sz;
    }
    unsigned char *pp = lo - sz;
    if (pp > base)
        memcpy(base, pp, sz);
    memcpy(pp, piv, sz);
    *ap = was_partitioned;
    return (size_type64) (pp - base) / sz;
}

static inline size_type64 pdq_part_l(unsigned char *base, size_type64 n, size_type64 sz,
                                int (*cmp)(const void *, const void *), unsigned char *piv) {
    memcpy(piv, base, sz);
    unsigned char *lo = base + sz;
    unsigned char *hi = base + (n - 1) * sz;
    while (lo <= hi && cmp(piv, hi) < 0) hi -= sz;
    while (lo <= hi && cmp(lo, piv) <= 0) lo += sz;
    while (lo < hi) {
        pdq__swap(lo, hi, sz);
        lo += sz;
        hi -= sz;
        while (cmp(lo, piv) <= 0) lo += sz;
        while (cmp(piv, hi) < 0) hi -= sz;
    }
    if (hi > base)
        memcpy(base, hi, sz);
    memcpy(hi, piv, sz);
    return (size_type64) (hi - base) / sz;
}

static void pdqsort(void *base, size_type64 number, size_type64 width, CSTL_COMPARE cmp) {
    if (!base || !cmp || width == 0 || number < 2) return;
    const size_type64 sz = width;

    unsigned char *arr = (unsigned char *) base;
    unsigned char sbuf[512];
    unsigned char *scratch;
    size_type64 need = sz * 2;
    scratch = (need <= sizeof(sbuf)) ? sbuf : (unsigned char *) malloc(need);
    if (!scratch) return;
    unsigned char *tmp = scratch;
    unsigned char *piv = scratch + sz;
    uint64_t rs = (uint64_t) number ^ 0x517cc1b727220a95ULL;
    struct pdq_frame {
        unsigned char *base;
        size_type64 n;
        size_type64 bad;
        int left;
    };
    struct pdq_frame stk[PDQ_MAX_STACK];
    int sp = 0;
    stk[sp].base = arr;
    stk[sp].n = number;
    stk[sp].bad = pdq_log2(number) * 2 + 1;
    stk[sp].left = 1;
    ++sp;
    while (sp > 0) {
        --sp;
        unsigned char *base = stk[sp].base;
        size_type64 n = stk[sp].n;
        size_type64 bad = stk[sp].bad;
        int leftmost = stk[sp].left;
    again:
        if (n <= PDQ_ISORT_THRESH) {
            if (leftmost)
                pdq_isort(base, n, sz, cmp, tmp);
            else
                pdq_isort_unguard(base, n, sz, cmp, tmp);
            continue;
        }
        if (bad == 0) {
            pdq_heap(base, n, sz, cmp);
            continue;
        }
        if (!leftmost && cmp(base - sz, base) >= 0) {
            size_type64 pp = pdq_part_l(base, n, sz, cmp, piv);
            base += (pp + 1) * sz;
            n -= pp + 1;
            goto again;
        }
        pdq_pick_pivot(base, n, sz, cmp);
        memcpy(piv, base, sz);
        int ap = 0;
        size_type64 pp = pdq_part_r(base, n, sz, cmp, piv, &ap);
        size_type64 lsz = pp;
        size_type64 rsz = n - pp - 1;
        int unbal = (lsz < n / 8 || rsz < n / 8);
        if (unbal) {
            bad--;
            if (lsz >= PDQ_ISORT_THRESH) pdq_break(base, lsz, sz, &rs);
            if (rsz >= PDQ_ISORT_THRESH) pdq_break(base + (pp + 1) * sz, rsz, sz, &rs);
        } else if (ap) {
            int lok = pdq_partial_isort(base, lsz, sz, cmp, tmp);
            int rok = pdq_partial_isort(base + (pp + 1) * sz, rsz, sz, cmp, tmp);
            if (lok && rok) continue;
        }
        unsigned char *rbase = base + (pp + 1) * sz;
        if (lsz < rsz) {
            if (rsz > 1 && sp < PDQ_MAX_STACK) {
                stk[sp].base = rbase;
                stk[sp].n = rsz;
                stk[sp].bad = bad;
                stk[sp].left = 0;
                ++sp;
            }
            n = lsz;
        } else {
            if (lsz > 1 && sp < PDQ_MAX_STACK) {
                stk[sp].base = base;
                stk[sp].n = lsz;
                stk[sp].bad = bad;
                stk[sp].left = leftmost;
                ++sp;
            }
            base = rbase;
            n = rsz;
            leftmost = 0;
        }
        goto again;
    }
    if (scratch != sbuf)
        free(scratch);
}
#undef PDQ_ELEM
#undef PDQ_ISORT_THRESH
#undef PDQ_NINTHER_THRESH
#undef PDQ_PARTIAL_LIMIT
#undef PDQ_MAX_STACK
#endif
