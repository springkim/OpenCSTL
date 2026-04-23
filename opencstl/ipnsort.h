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
// Port of Rust's ipnsort (Lukas Bergdoll):
//   - pattern detection (existing run)
//   - adaptive median-of-medians pivot (glidesort / Orson Peters)
//   - ancestor-pivot duplicate-key short-circuit
//   - heapsort fallback on depth exhaustion
// Augmented with pdqsort-style optimizations:
//   - small-sort with 3/4-element Bose-Nelson networks + insertion extend
//   - unguarded insertion sort when a sentinel (prior pivot) exists on the left
//   - partial-insertion-sort fast path for already-partitioned subarrays
//   - break-patterns shuffle for unbalanced partitions
// Fully allocation-free (no malloc/free on any path):
//   - partition keeps the pivot at base[0] and reads it in-place
//   - ancestor pivot is tracked by pointer into the array, not copied
//   - insertion sort uses a small stack-local scratch with a swap-based
//     fallback for oversized element types
//
#pragma once
#if !defined(_OPENCSTL_IPNSORT_H)
#define _OPENCSTL_IPNSORT_H
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"

#define IPN_INSERTION_SORT_THRESHOLD    20
#define IPN_SMALL_SORT_THRESHOLD        20
/* Recursive median-of-medians only pays off on very large subarrays when each
   comparison is an indirect call; below this, plain median-of-3 is cheaper. */
#define IPN_PSEUDO_MEDIAN_REC_THRESHOLD 1024
#define IPN_PARTIAL_LIMIT                8
/* Stack-local temp buffer size used by insertion paths. Element widths up to
   this get the fast tmp-based shift; larger widths fall back to swap-based
   insertion (still correct, no allocation). */
#define IPN_ISORT_TMP_BYTES            128
/* Max depth of the explicit work stack. 2 * log2(n) bounds the longest path
   before the heapsort fallback fires, so 128 safely covers n up to 2^64. */
#define IPN_STACK_MAX                  128

#if defined(_MSC_VER)
#define IPN_LIKELY(x)   (x)
#define IPN_UNLIKELY(x) (x)
#define IPN_ALWAYS_INLINE __forceinline
#else
#define IPN_LIKELY(x)   __builtin_expect(!!(x), 1)
#define IPN_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define IPN_ALWAYS_INLINE __attribute__((always_inline)) inline
#endif

static IPN_ALWAYS_INLINE void ipn__swap(unsigned char *a, unsigned char *b, size_type64 n) {
    if (a == b) return;
    if (IPN_LIKELY(n == 8)) {
        uint64_t t;
        memcpy(&t, a, 8);
        memcpy(a, b, 8);
        memcpy(b, &t, 8);
    } else if (n == 16) {
        uint64_t t0, t1;
        memcpy(&t0, a, 8);
        memcpy(&t1, a + 8, 8);
        memcpy(a, b, 16);
        memcpy(b, &t0, 8);
        memcpy(b + 8, &t1, 8);
    } else if (n == 4) {
        uint32_t t;
        memcpy(&t, a, 4);
        memcpy(a, b, 4);
        memcpy(b, &t, 4);
    } else {
        unsigned char buf[64];
        size_type64 left = n;
        while (left >= sizeof(buf)) {
            memcpy(buf, a, sizeof(buf));
            memcpy(a, b, sizeof(buf));
            memcpy(b, buf, sizeof(buf));
            a += sizeof(buf);
            b += sizeof(buf);
            left -= sizeof(buf);
        }
        if (left) {
            memcpy(buf, a, left);
            memcpy(a, b, left);
            memcpy(b, buf, left);
        }
    }
}

#define IPN_CSWAP(bp, ai, bi, w, cmp) do {                         \
    unsigned char *_ipn_a = (bp) + (ai) * (w);                     \
    unsigned char *_ipn_b = (bp) + (bi) * (w);                     \
    if ((cmp)(_ipn_a, _ipn_b) > 0) ipn__swap(_ipn_a, _ipn_b, (w)); \
} while (0)

static IPN_ALWAYS_INLINE uint32_t ipn_log2u(size_type64 v) {
    uint32_t r = 0;
    v |= 1;
    while (v > 1) {
        v >>= 1;
        ++r;
    }
    return r;
}

static IPN_ALWAYS_INLINE uint64_t ipn_xorshift(uint64_t *s) {
    uint64_t x = *s;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    return (*s = x);
}

static void ipn_reverse(unsigned char *base, size_type64 n, size_type64 width) {
    if (n < 2) return;
    unsigned char *lo = base;
    unsigned char *hi = base + (n - 1) * width;
    while (lo < hi) {
        ipn__swap(lo, hi, width);
        lo += width;
        hi -= width;
    }
}

/* Swap-based insertion shift fallback — used when width exceeds the stack
   tmp buffer. Does 3x the memory traffic of the tmp-based path, but needs
   no scratch. */
static void ipn_isort_shift_swap(unsigned char *base, size_type64 n, size_type64 offset,
                                 size_type64 width, CSTL_COMPARE cmp, int guarded) {
    if (guarded) {
        for (size_type64 i = offset; i < n; ++i) {
            unsigned char *j = base + i * width;
            while (j > base && cmp(j - width, j) > 0) {
                ipn__swap(j - width, j, width);
                j -= width;
            }
        }
    } else {
        for (size_type64 i = offset; i < n; ++i) {
            unsigned char *j = base + i * width;
            while (cmp(j - width, j) > 0) {
                ipn__swap(j - width, j, width);
                j -= width;
                if (j == base) break;
            }
        }
    }
}

/* Guarded insertion of base[offset..n] into sorted base[0..offset]. Safe when
   base[-1] may not exist. Stack-local tmp, no heap. */
static void ipn_isort_shift_left(unsigned char *base, size_type64 n, size_type64 offset,
                                 size_type64 width, CSTL_COMPARE cmp) {
    if (IPN_UNLIKELY(width > IPN_ISORT_TMP_BYTES)) {
        ipn_isort_shift_swap(base, n, offset, width, cmp, 1);
        return;
    }
    unsigned char tmp[IPN_ISORT_TMP_BYTES];
    for (size_type64 i = offset; i < n; ++i) {
        unsigned char *c = base + i * width;
        if (cmp(c - width, c) <= 0) continue;
        memcpy(tmp, c, width);
        unsigned char *j = c;
        do {
            memcpy(j, j - width, width);
            j -= width;
        } while (j > base && cmp(j - width, tmp) > 0);
        memcpy(j, tmp, width);
    }
}

/* Unguarded variant: requires base[-1] to exist and be <= any element (sentinel). */
static void ipn_isort_shift_left_unguard(unsigned char *base, size_type64 n, size_type64 offset,
                                         size_type64 width, CSTL_COMPARE cmp) {
    if (IPN_UNLIKELY(width > IPN_ISORT_TMP_BYTES)) {
        ipn_isort_shift_swap(base, n, offset, width, cmp, 0);
        return;
    }
    unsigned char tmp[IPN_ISORT_TMP_BYTES];
    for (size_type64 i = offset; i < n; ++i) {
        unsigned char *c = base + i * width;
        if (cmp(c - width, c) <= 0) continue;
        memcpy(tmp, c, width);
        unsigned char *j = c;
        do {
            memcpy(j, j - width, width);
            j -= width;
        } while (cmp(j - width, tmp) > 0);
        memcpy(j, tmp, width);
    }
}

/* 3-element sorting network. */
static IPN_ALWAYS_INLINE void ipn_sort3(unsigned char *b, size_type64 w, CSTL_COMPARE cmp) {
    IPN_CSWAP(b, 0, 1, w, cmp);
    IPN_CSWAP(b, 1, 2, w, cmp);
    IPN_CSWAP(b, 0, 1, w, cmp);
}

/* 4-element Bose-Nelson optimal (5 comparators, depth 3). */
static IPN_ALWAYS_INLINE void ipn_sort4(unsigned char *b, size_type64 w, CSTL_COMPARE cmp) {
    IPN_CSWAP(b, 0, 2, w, cmp);
    IPN_CSWAP(b, 1, 3, w, cmp);
    IPN_CSWAP(b, 0, 1, w, cmp);
    IPN_CSWAP(b, 2, 3, w, cmp);
    IPN_CSWAP(b, 1, 2, w, cmp);
}

/* small_sort: dispatches to network for n <= 4, then extends via insertion.
   When `unguarded`, skips the left-bound check on insertion shift. */
static void ipn_small_sort(unsigned char *base, size_type64 n, size_type64 width,
                           CSTL_COMPARE cmp, int unguarded) {
    if (n < 2) return;
    if (n == 2) {
        IPN_CSWAP(base, 0, 1, width, cmp);
        return;
    }
    if (n == 3) {
        ipn_sort3(base, width, cmp);
        return;
    }
    ipn_sort4(base, width, cmp);
    if (n == 4) return;
    if (unguarded)
        ipn_isort_shift_left_unguard(base, n, 4, width, cmp);
    else
        ipn_isort_shift_left(base, n, 4, width, cmp);
}

/* Top-level insertion sort (guarded). */
static IPN_ALWAYS_INLINE void ipn_isort(unsigned char *base, size_type64 n, size_type64 width,
                                        CSTL_COMPARE cmp) {
    if (n < 2) return;
    ipn_small_sort(base, n, width, cmp, 0);
}

/* Detect a strictly-descending or non-strictly-ascending run at index 0. Read-only. */
static size_type64 ipn_find_existing_run(const unsigned char *base, size_type64 n, size_type64 width,
                                    CSTL_COMPARE cmp, int *was_reversed) {
    *was_reversed = 0;
    if (n < 2) return n;
    size_type64 i = 1;
    if (cmp(base, base + width) > 0) {
        *was_reversed = 1;
        while (i < n && cmp(base + (i - 1) * width, base + i * width) > 0) ++i;
    } else {
        while (i < n && cmp(base + (i - 1) * width, base + i * width) <= 0) ++i;
    }
    return i;
}

static void ipn_sift(unsigned char *base, size_type64 root, size_type64 n, size_type64 width,
                     CSTL_COMPARE cmp) {
    for (;;) {
        size_type64 c = root * 2 + 1;
        if (c >= n) break;
        size_type64 mx = root;
        if (cmp(base + mx * width, base + c * width) < 0) mx = c;
        if (c + 1 < n && cmp(base + mx * width, base + (c + 1) * width) < 0) mx = c + 1;
        if (mx == root) break;
        ipn__swap(base + root * width, base + mx * width, width);
        root = mx;
    }
}

static void ipn_heapsort(unsigned char *base, size_type64 n, size_type64 width, CSTL_COMPARE cmp) {
    if (n < 2) return;
    for (size_type64 i = n / 2; i-- > 0;) ipn_sift(base, i, n, width, cmp);
    for (size_type64 e = n; e-- > 1;) {
        ipn__swap(base, base + e * width, width);
        ipn_sift(base, 0, e, width, cmp);
    }
}

static IPN_ALWAYS_INLINE unsigned char *ipn_median3(unsigned char *a, unsigned char *b,
                                                    unsigned char *c, CSTL_COMPARE cmp) {
    int x = (cmp(a, b) < 0);
    int y = (cmp(a, c) < 0);
    if (x == y) {
        int z = (cmp(b, c) < 0);
        return (z ^ x) ? c : b;
    }
    return a;
}

static unsigned char *ipn_median3_rec(unsigned char *a, unsigned char *b, unsigned char *c,
                                      size_type64 n, size_type64 width, CSTL_COMPARE cmp) {
    if (n * 8 >= IPN_PSEUDO_MEDIAN_REC_THRESHOLD) {
        size_type64 n8 = n / 8;
        a = ipn_median3_rec(a, a + n8 * 4 * width, a + n8 * 7 * width, n8, width, cmp);
        b = ipn_median3_rec(b, b + n8 * 4 * width, b + n8 * 7 * width, n8, width, cmp);
        c = ipn_median3_rec(c, c + n8 * 4 * width, c + n8 * 7 * width, n8, width, cmp);
    }
    return ipn_median3(a, b, c, cmp);
}

static size_type64 ipn_choose_pivot(unsigned char *base, size_type64 n, size_type64 width, CSTL_COMPARE cmp) {
    size_type64 n8 = n / 8;
    unsigned char *a = base;
    unsigned char *b = base + n8 * 4 * width;
    unsigned char *c = base + n8 * 7 * width;
    unsigned char *r;
    if (n < IPN_PSEUDO_MEDIAN_REC_THRESHOLD) {
        r = ipn_median3(a, b, c, cmp);
    } else {
        r = ipn_median3_rec(a, b, c, n8, width, cmp);
    }
    return (size_type64) (r - base) / width;
}

/* In-place Hoare partition. Pivot lives at base[0] for the entire scan and is
   read directly from there (no pivot buffer). *was_partitioned = 1 if no swaps
   were needed during scanning. */
static size_type64 ipn_partition(unsigned char *base, size_type64 n, size_type64 pivot_pos,
                            size_type64 width, CSTL_COMPARE cmp, int *was_partitioned) {
    if (pivot_pos != 0)
        ipn__swap(base, base + pivot_pos * width, width);
    unsigned char *piv = base;

    unsigned char *lo = base + width;
    unsigned char *hi = base + (n - 1) * width;

    while (lo <= hi && cmp(lo, piv) < 0) lo += width;
    while (lo <= hi && cmp(hi, piv) >= 0) hi -= width;

    *was_partitioned = (lo > hi);

    while (lo < hi) {
        ipn__swap(lo, hi, width);
        lo += width;
        hi -= width;
        while (cmp(lo, piv) < 0) lo += width;
        while (cmp(hi, piv) >= 0) hi -= width;
    }

    unsigned char *pp = lo - width;
    if (pp > base)
        ipn__swap(base, pp, width);

    return (size_type64) (pp - base) / width;
}

/* Equal-pivot partition: elements <= pivot go left. In-place pivot at base[0]. */
static size_type64 ipn_partition_eq(unsigned char *base, size_type64 n, size_type64 pivot_pos,
                               size_type64 width, CSTL_COMPARE cmp) {
    if (pivot_pos != 0)
        ipn__swap(base, base + pivot_pos * width, width);
    unsigned char *piv = base;

    unsigned char *lo = base + width;
    unsigned char *hi = base + (n - 1) * width;

    while (lo <= hi && cmp(lo, piv) <= 0) lo += width;
    while (lo <= hi && cmp(hi, piv) > 0) hi -= width;

    while (lo < hi) {
        ipn__swap(lo, hi, width);
        lo += width;
        hi -= width;
        while (cmp(lo, piv) <= 0) lo += width;
        while (cmp(hi, piv) > 0) hi -= width;
    }

    unsigned char *pp = lo - width;
    if (pp > base)
        ipn__swap(base, pp, width);

    return (size_type64) (pp - base) / width;
}

/* Insertion sort bounded by total shift count. Returns 1 if fully sorted, 0 if
   it gave up after IPN_PARTIAL_LIMIT shifts. Stack-local tmp. */
static int ipn_partial_isort(unsigned char *base, size_type64 n, size_type64 width,
                             CSTL_COMPARE cmp) {
    if (n < 2) return 1;
    if (IPN_UNLIKELY(width > IPN_ISORT_TMP_BYTES)) {
        /* Swap-based variant with shift budget. */
        size_type64 cnt = 0;
        for (size_type64 i = 1; i < n; ++i) {
            unsigned char *j = base + i * width;
            while (j > base && cmp(j - width, j) > 0) {
                ipn__swap(j - width, j, width);
                j -= width;
                if (++cnt > IPN_PARTIAL_LIMIT) return 0;
            }
        }
        return 1;
    }
    unsigned char tmp[IPN_ISORT_TMP_BYTES];
    size_type64 cnt = 0;
    for (size_type64 i = 1; i < n; ++i) {
        unsigned char *c = base + i * width;
        if (cmp(c - width, c) <= 0) continue;
        memcpy(tmp, c, width);
        unsigned char *j = c;
        do {
            memcpy(j, j - width, width);
            j -= width;
            ++cnt;
        } while (j > base && cmp(j - width, tmp) > 0);
        memcpy(j, tmp, width);
        if (cnt > IPN_PARTIAL_LIMIT) return 0;
    }
    return 1;
}

/* Pseudo-random shuffle of 3 elements near the middle to break adversarial patterns. */
static void ipn_break_patterns(unsigned char *base, size_type64 n, size_type64 width, uint64_t *rs) {
    if (n < 8) return;
    size_type64 q = n >> 2;
    size_type64 h = n >> 1;
    size_type64 span = (h > 0) ? h : 1;
    size_type64 r1 = (size_type64) (ipn_xorshift(rs) % span);
    size_type64 r2 = (size_type64) (ipn_xorshift(rs) % span);
    size_type64 r3 = (size_type64) (ipn_xorshift(rs) % span);
    ipn__swap(base + q * width,       base + (q + r1) * width, width);
    ipn__swap(base + h * width,       base + (q + r2) * width, width);
    ipn__swap(base + (q + h) * width, base + (q + r3) * width, width);
}

/* Frame on the explicit work stack. The ancestor is a pointer into the array
   at an earlier pivot's resting position; that slot stays untouched by every
   subarray that could still be on the stack when we pop this frame. */
typedef struct ipn_frame {
    unsigned char *base;
    const unsigned char *anc;
    size_type64 n;
    uint32_t limit;
    uint32_t flags; /* bit 0: leftmost, bit 1: has_anc */
} ipn_frame_t;

#define IPN_F_LEFTMOST 1u
#define IPN_F_HAS_ANC  2u

/* Iterative quicksort. Each popped frame drives an inner loop that continues
   on the right partition and pushes the left for later processing, mirroring
   the prior tail-recursive structure without using the C call stack. */
static void ipn_quicksort(unsigned char *base0, size_type64 n0, size_type64 width,
                          CSTL_COMPARE cmp, uint32_t limit0, uint64_t *rs) {
    ipn_frame_t stk[IPN_STACK_MAX];
    int sp = 0;
    stk[0].base = base0;
    stk[0].n = n0;
    stk[0].limit = limit0;
    stk[0].anc = NULL;
    stk[0].flags = IPN_F_LEFTMOST;
    sp = 1;

    while (sp > 0) {
        --sp;
        unsigned char *base = stk[sp].base;
        size_type64 n = stk[sp].n;
        uint32_t limit = stk[sp].limit;
        const unsigned char *anc = stk[sp].anc;
        int leftmost = (int) (stk[sp].flags & IPN_F_LEFTMOST);
        int has_anc  = (int) (stk[sp].flags & IPN_F_HAS_ANC);

        for (;;) {
            if (n <= IPN_SMALL_SORT_THRESHOLD) {
                ipn_small_sort(base, n, width, cmp, !leftmost);
                break;
            }
            if (limit == 0) {
                ipn_heapsort(base, n, width, cmp);
                break;
            }
            --limit;

            size_type64 pivot_pos = ipn_choose_pivot(base, n, width, cmp);

            if (has_anc && cmp(anc, base + pivot_pos * width) >= 0) {
                size_type64 num_le = ipn_partition_eq(base, n, pivot_pos, width, cmp);
                if (num_le + 1 >= n) break;
                anc = base + num_le * width;
                has_anc = 1;
                base += (num_le + 1) * width;
                n -= num_le + 1;
                leftmost = 0;
                continue;
            }

            int was_partitioned = 0;
            size_type64 num_lt = ipn_partition(base, n, pivot_pos, width, cmp, &was_partitioned);

            size_type64 lsz = num_lt;
            size_type64 rsz = n - num_lt - 1;
            int unbalanced = (lsz < (n >> 3)) || (rsz < (n >> 3));

            if (unbalanced) {
                if (lsz >= 8) ipn_break_patterns(base, lsz, width, rs);
                if (rsz >= 8) ipn_break_patterns(base + (num_lt + 1) * width, rsz, width, rs);
            } else if (was_partitioned) {
                int lok = ipn_partial_isort(base, lsz, width, cmp);
                int rok = ipn_partial_isort(base + (num_lt + 1) * width, rsz, width, cmp);
                if (lok && rok) break;
            }

            /* Push left partition for later; continue walking right in-loop.
               The pivot at base[num_lt] stays resident because neither the
               pushed left work nor any right-chain work ever reaches it. */
            if (lsz >= 2) {
                if (IPN_LIKELY(sp < IPN_STACK_MAX)) {
                    stk[sp].base = base;
                    stk[sp].n = lsz;
                    stk[sp].limit = limit;
                    stk[sp].anc = anc;
                    stk[sp].flags = (uint32_t) ((leftmost ? IPN_F_LEFTMOST : 0u) |
                                                (has_anc  ? IPN_F_HAS_ANC  : 0u));
                    ++sp;
                } else {
                    /* Stack exhausted — vanishingly rare; sort left in place
                       with heapsort to keep the worst-case bound. */
                    ipn_heapsort(base, lsz, width, cmp);
                }
            }

            if (rsz < 2) break;
            anc = base + num_lt * width;
            has_anc = 1;
            base += (num_lt + 1) * width;
            n = rsz;
            leftmost = 0;
        }
    }
}

static void ipnsort(void *base, size_type64 number, size_type64 width, CSTL_COMPARE compare) {
    if (!base || !compare || width == 0 || number < 2) return;

    unsigned char *arr = (unsigned char *) base;

    if (number <= IPN_INSERTION_SORT_THRESHOLD) {
        ipn_isort(arr, number, width, compare);
        return;
    }

    int was_reversed = 0;
    size_type64 run_len = ipn_find_existing_run(arr, number, width, compare, &was_reversed);
    if (run_len == number) {
        if (was_reversed) ipn_reverse(arr, number, width);
        return;
    }

    uint32_t limit = 2 * ipn_log2u(number);
    uint64_t rs = (uint64_t) number ^ 0x517cc1b727220a95ULL;
    ipn_quicksort(arr, number, width, compare, limit, &rs);
}

#undef IPN_INSERTION_SORT_THRESHOLD
#undef IPN_SMALL_SORT_THRESHOLD
#undef IPN_PSEUDO_MEDIAN_REC_THRESHOLD
#undef IPN_PARTIAL_LIMIT
#undef IPN_ISORT_TMP_BYTES
#undef IPN_STACK_MAX
#undef IPN_F_LEFTMOST
#undef IPN_F_HAS_ANC
#undef IPN_LIKELY
#undef IPN_UNLIKELY
#undef IPN_ALWAYS_INLINE
#undef IPN_CSWAP
#endif
