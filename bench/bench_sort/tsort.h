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
#if !defined(_OPENCSTL_TSORT_H)
#define _OPENCSTL_TSORT_H
#include <stdlib.h>
#include <string.h>

#define TS_MIN_MERGE   32
#define TS_MIN_GALLOP   7
#define TS_MAX_STACK   85

struct ts_run {
    size_t base;
    size_t len;
};

static inline size_t ts_minrun(size_t n) {
    size_t r = 0;
    while (n >= TS_MIN_MERGE) {
        r |= (n & 1);
        n >>= 1;
    }
    return n + r;
}

static void ts_binsort(char *arr, size_t lo, size_t hi, size_t start,
                       size_t sz, int (*cmp)(const void *, const void *)) {
    char sbuf[256];
    char *tmp = (sz <= sizeof(sbuf)) ? sbuf : (char *) malloc(sz);
    for (size_t i = start; i < hi; i++) {
        memcpy(tmp, arr + i * sz, sz);
        size_t left = lo, right = i;
        while (left < right) {
            size_t mid = left + ((right - left) >> 1);
            if (cmp(tmp, arr + mid * sz) < 0) right = mid;
            else left = mid + 1;
        }
        if (left < i) {
            memmove(arr + (left + 1) * sz, arr + left * sz, (i - left) * sz);
            memcpy(arr + left * sz, tmp, sz);
        }
    }
    if (tmp != sbuf) free(tmp);
}

static size_t ts_count_run(char *arr, size_t lo, size_t hi,
                           size_t sz, int (*cmp)(const void *, const void *)) {
    if (hi - lo < 2) return hi - lo;
    size_t run_hi = lo + 1;
    if (cmp(arr + run_hi * sz, arr + lo * sz) < 0) {
        while (run_hi + 1 < hi &&
               cmp(arr + (run_hi + 1) * sz, arr + run_hi * sz) < 0)
            run_hi++;
        run_hi++;
        char rbuf[256];
        char *t = (sz <= sizeof(rbuf)) ? rbuf : (char *) malloc(sz);
        size_t a = lo, b = run_hi - 1;
        while (a < b) {
            memcpy(t, arr + a * sz, sz);
            memcpy(arr + a * sz, arr + b * sz, sz);
            memcpy(arr + b * sz, t, sz);
            a++;
            b--;
        }
        if (t != rbuf) free(t);
    } else {
        while (run_hi + 1 < hi &&
               cmp(arr + (run_hi + 1) * sz, arr + run_hi * sz) >= 0)
            run_hi++;
        run_hi++;
    }
    return run_hi - lo;
}

static size_t ts_gallop_right(const char *key, const char *a, size_t n,
                              size_t sz, int (*cmp)(const void *, const void *)) {
    if (n == 0 || cmp(key, a) < 0) return 0;
    size_t last = 0, ofs = 1;
    while (ofs < n && cmp(key, a + ofs * sz) >= 0) {
        last = ofs;
        ofs = (ofs << 1) + 1;
    }
    if (ofs > n) ofs = n;
    size_t lo = last + 1, hi = ofs;
    while (lo < hi) {
        size_t m = lo + ((hi - lo) >> 1);
        if (cmp(key, a + m * sz) >= 0) lo = m + 1;
        else hi = m;
    }
    return lo;
}

static size_t ts_gallop_left(const char *key, const char *a, size_t n,
                             size_t sz, int (*cmp)(const void *, const void *)) {
    if (n == 0 || cmp(key, a) <= 0) return 0;
    size_t last = 0, ofs = 1;
    while (ofs < n && cmp(key, a + ofs * sz) > 0) {
        last = ofs;
        ofs = (ofs << 1) + 1;
    }
    if (ofs > n) ofs = n;
    size_t lo = last + 1, hi = ofs;
    while (lo < hi) {
        size_t m = lo + ((hi - lo) >> 1);
        if (cmp(key, a + m * sz) > 0) lo = m + 1;
        else hi = m;
    }
    return lo;
}

static void ts_merge_lo(char *base, size_t len1, size_t len2,
                        size_t sz, int (*cmp)(const void *, const void *), char *buf) {
    memcpy(buf, base, len1 * sz);
    char *c1 = buf, *e1 = buf + len1 * sz;
    char *c2 = base + len1 * sz, *e2 = c2 + len2 * sz;
    char *d = base;
    size_t mg = TS_MIN_GALLOP;
    for (;;) {
        size_t cnt1 = 0, cnt2 = 0;
        do {
            if (cmp(c2, c1) < 0) {
                memcpy(d, c2, sz);
                c2 += sz;
                d += sz;
                cnt2++;
                cnt1 = 0;
                if (c2 >= e2) goto tail_lo;
            } else {
                memcpy(d, c1, sz);
                c1 += sz;
                d += sz;
                cnt1++;
                cnt2 = 0;
                if (c1 >= e1) return;
            }
        } while ((cnt1 | cnt2) < mg);
        do {
            cnt1 = ts_gallop_right(c2, c1, (size_t) (e1 - c1) / sz, sz, cmp);
            if (cnt1) {
                memcpy(d, c1, cnt1 * sz);
                c1 += cnt1 * sz;
                d += cnt1 * sz;
            }
            if (c1 >= e1) return;
            memcpy(d, c2, sz);
            c2 += sz;
            d += sz;
            if (c2 >= e2) goto tail_lo;
            cnt2 = ts_gallop_left(c1, c2, (size_t) (e2 - c2) / sz, sz, cmp);
            if (cnt2) {
                memmove(d, c2, cnt2 * sz);
                c2 += cnt2 * sz;
                d += cnt2 * sz;
            }
            if (c2 >= e2) goto tail_lo;
            memcpy(d, c1, sz);
            c1 += sz;
            d += sz;
            if (c1 >= e1) return;
            if (mg > 1) mg--;
        } while ((cnt1 | cnt2) >= TS_MIN_GALLOP);
        mg += 2;
    }
tail_lo:
    if (c1 < e1)
        memcpy(d, c1, (size_t) (e1 - c1));
}

static void ts_merge_hi(char *base, size_t len1, size_t len2,
                        size_t sz, int (*cmp)(const void *, const void *), char *buf) {
    memcpy(buf, base + len1 * sz, len2 * sz);
    size_t i = len1, j = len2;
    size_t k = len1 + len2;
    while (i > 0 && j > 0) {
        k--;
        if (cmp(buf + (j - 1) * sz, base + (i - 1) * sz) < 0) {
            memcpy(base + k * sz, base + (i - 1) * sz, sz);
            i--;
        } else {
            memcpy(base + k * sz, buf + (j - 1) * sz, sz);
            j--;
        }
    }
    if (j > 0)
        memcpy(base, buf, j * sz);
}

static inline void ts_do_merge(char *arr, size_t base1, size_t len1,
                               size_t len2, size_t sz, int (*cmp)(const void *, const void *),
                               char *buf) {
    size_t mid = base1 + len1;
    if (cmp(arr + (mid - 1) * sz, arr + mid * sz) <= 0) return;
    if (len1 <= len2)
        ts_merge_lo(arr + base1 * sz, len1, len2, sz, cmp, buf);
    else
        ts_merge_hi(arr + base1 * sz, len1, len2, sz, cmp, buf);
}

static void ts_merge_collapse(char *arr, struct ts_run *stk, size_t *sp,
                              size_t sz, int (*cmp)(const void *, const void *), char *buf) {
    while (*sp > 1) {
        size_t n = *sp - 2;
        int need_merge = 0;
        if (n > 0 && stk[n - 1].len <= stk[n].len + stk[n + 1].len) {
            if (stk[n - 1].len < stk[n + 1].len) n--;
            need_merge = 1;
        } else if (stk[n].len <= stk[n + 1].len) {
            need_merge = 1;
        }
        if (!need_merge) break;
        ts_do_merge(arr, stk[n].base, stk[n].len, stk[n + 1].len,
                    sz, cmp, buf);
        stk[n].len += stk[n + 1].len;
        for (size_t i = n + 1; i + 1 < *sp; i++)
            stk[i] = stk[i + 1];
        (*sp)--;
    }
}

static void ts_merge_force(char *arr, struct ts_run *stk, size_t *sp,
                           size_t sz, int (*cmp)(const void *, const void *), char *buf) {
    while (*sp > 1) {
        size_t n = *sp - 2;
        if (n > 0 && stk[n - 1].len < stk[n + 1].len) n--;
        ts_do_merge(arr, stk[n].base, stk[n].len, stk[n + 1].len,
                    sz, cmp, buf);
        stk[n].len += stk[n + 1].len;
        for (size_t i = n + 1; i + 1 < *sp; i++)
            stk[i] = stk[i + 1];
        (*sp)--;
    }
}

void tsort(void *mem, const size_t len, const size_t size_elem,
           int (*cmp)(const void *, const void *)) {
    if (len < 2) return;
    char *arr = (char *) mem;
    size_t sz = size_elem;
    if (len < TS_MIN_MERGE) {
        size_t run = ts_count_run(arr, 0, len, sz, cmp);
        ts_binsort(arr, 0, len, run, sz, cmp);
        return;
    }
    char *buf = (char *) malloc(len * sz);
    if (!buf) return;
    struct ts_run stk[TS_MAX_STACK];
    size_t sp = 0;
    size_t minrun = ts_minrun(len);
    size_t lo = 0;
    while (lo < len) {
        size_t run_len = ts_count_run(arr, lo, len, sz, cmp);
        if (run_len < minrun) {
            size_t force = len - lo;
            if (force > minrun) force = minrun;
            ts_binsort(arr, lo, lo + force, lo + run_len, sz, cmp);
            run_len = force;
        }
        stk[sp].base = lo;
        stk[sp].len = run_len;
        sp++;
        ts_merge_collapse(arr, stk, &sp, sz, cmp, buf);
        lo += run_len;
    }
    ts_merge_force(arr, stk, &sp, sz, cmp, buf);
    free(buf);
}
#undef TS_MIN_MERGE
#undef TS_MIN_GALLOP
#undef TS_MAX_STACK
#endif
