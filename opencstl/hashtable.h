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


#if !defined(_OPENCSTL_HASHTABLE_H)
#define _OPENCSTL_HASHTABLE_H
#include "zalloc.h"
#include "van_emde_boas_tree.h"
/* [already included: error.h] */

/* Linear-probing open-addressing hash table.
 * Control byte layout: 0 = empty, non-zero = occupied (7-bit hash fragment | 0x80).
 * Backward-shift deletion keeps the table tombstone-free. */
#define HT_CTRL_EMPTY 0u
#define HT_LOAD_NUM   7
#define HT_LOAD_DEN   8
#define HT_MIN_CAP    8u
/* iterator.h gates on HTM_VEBT to pick the veb-tree based htm lookup path. */
#define HTM_VEBT

/* threshold = cap * 7/8, computed via shifts (no fp) */
#define HT_THRESHOLD(cap) ((cap) - ((cap) >> 3))

#if defined(__GNUC__) || defined(__clang__)
#define OPENCSTL_ALWAYS_INLINE static inline __attribute__((always_inline))
#define OPENCSTL_LIKELY(x)   __builtin_expect(!!(x), 1)
#define OPENCSTL_UNLIKELY(x) __builtin_expect(!!(x), 0)
#define OPENCSTL_PREFETCH(p) __builtin_prefetch((p))
#elif defined(_MSC_VER)
/* MSVC's `static inline` is advisory; __forceinline is what the hot-path
 * helpers need so hash_mixer, __ht_key_eq, __ht_mum actually get inlined and
 * the per-insert dispatch cost does not multiply across 50M calls. */
#define OPENCSTL_ALWAYS_INLINE static __forceinline
#define OPENCSTL_LIKELY(x)   (x)
#define OPENCSTL_UNLIKELY(x) (x)
#define OPENCSTL_PREFETCH(p) ((void)0)
#else
#define OPENCSTL_ALWAYS_INLINE static inline
#define OPENCSTL_LIKELY(x)   (x)
#define OPENCSTL_UNLIKELY(x) (x)
#define OPENCSTL_PREFETCH(p) ((void)0)
#endif

/* MSVC x64 does not expose __uint128_t; use the _umul128 intrinsic instead of
 * the scalar 4-multiply fallback. This cuts the hash cost from ~15 cycles to
 * one full-width multiply. */
#if defined(_MSC_VER) && defined(_M_X64)
#include <intrin.h>
#pragma intrinsic(_umul128)
#endif

static inline size_type64 __ht_next_pow2(size_type64 n) {
    size_type64 p = HT_MIN_CAP;
    while (p < n) p <<= 1;
    return p;
}

OPENCSTL_ALWAYS_INLINE bool __ht_key_eq(const void *a, const void *b, size_type64 ks) {
    switch (ks) {
        case 1: return *(const uint8_t *) a == *(const uint8_t *) b;
        case 2: return *(const uint16_t *) a == *(const uint16_t *) b;
        case 4: return *(const uint32_t *) a == *(const uint32_t *) b;
        case 8: return *(const uint64_t *) a == *(const uint64_t *) b;
        default: return memcmp(a, b, ks) == 0;
    }
}

/* XX* primes retained for the byte-oriented hash() kept as public helper. */
#define OPENCSTL_XXH_ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#define OPENCSTL_XXH_ROTL64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))
#define OPENCSTL_XXH32_P2 2246822519U
#define OPENCSTL_XXH32_P3 3266489917U
#define OPENCSTL_XXH64_P1 11400714785074694791ULL
#define OPENCSTL_XXH64_P2 14029467366897019727ULL
#define OPENCSTL_XXH64_P3  1609587929392839161ULL
#define OPENCSTL_XXH64_P4  9650029242287828579ULL
#define OPENCSTL_XXH64_P5  2870177450012600261ULL

OPENCSTL_FUNC size_type64 hash32(void *_key) {
    unsigned int h = *(unsigned int *) _key;
    h ^= h >> 15;
    h *= OPENCSTL_XXH32_P2;
    h ^= h >> 13;
    h *= OPENCSTL_XXH32_P3;
    h ^= h >> 16;
    return (size_type64) h;
}

OPENCSTL_FUNC size_type64 hash64(void *_key) {
    unsigned long long x = *(unsigned long long *) _key;
    x ^= x >> 33;
    x *= OPENCSTL_XXH64_P2;
    x ^= x >> 29;
    x *= OPENCSTL_XXH64_P3;
    x ^= x >> 32;
    return (size_type64) x;
}

OPENCSTL_FUNC size_type64 hash(void *_key, size_type64 len) {
    const unsigned char *p = (const unsigned char *) _key;
    const unsigned char *end = p + len;
    unsigned long long h64;
    unsigned long long k;

    if (len >= 32) {
        unsigned long long v1 = OPENCSTL_XXH64_P1 + OPENCSTL_XXH64_P2;
        unsigned long long v2 = OPENCSTL_XXH64_P2;
        unsigned long long v3 = 0ULL;
        unsigned long long v4 = 0ULL - OPENCSTL_XXH64_P1;
        const unsigned char *limit = end - 32;

        do {
            memcpy(&k, p, 8);
            v1 += k * OPENCSTL_XXH64_P2;
            v1 = OPENCSTL_XXH_ROTL64(v1, 31) * OPENCSTL_XXH64_P1;
            p += 8;
            memcpy(&k, p, 8);
            v2 += k * OPENCSTL_XXH64_P2;
            v2 = OPENCSTL_XXH_ROTL64(v2, 31) * OPENCSTL_XXH64_P1;
            p += 8;
            memcpy(&k, p, 8);
            v3 += k * OPENCSTL_XXH64_P2;
            v3 = OPENCSTL_XXH_ROTL64(v3, 31) * OPENCSTL_XXH64_P1;
            p += 8;
            memcpy(&k, p, 8);
            v4 += k * OPENCSTL_XXH64_P2;
            v4 = OPENCSTL_XXH_ROTL64(v4, 31) * OPENCSTL_XXH64_P1;
            p += 8;
        } while (p <= limit);

        h64 = OPENCSTL_XXH_ROTL64(v1, 1)
              + OPENCSTL_XXH_ROTL64(v2, 7)
              + OPENCSTL_XXH_ROTL64(v3, 12)
              + OPENCSTL_XXH_ROTL64(v4, 18);

        v1 = OPENCSTL_XXH_ROTL64(v1, 31) * OPENCSTL_XXH64_P1;
        h64 ^= v1;
        h64 = h64 * OPENCSTL_XXH64_P1 + OPENCSTL_XXH64_P4;
        v2 = OPENCSTL_XXH_ROTL64(v2, 31) * OPENCSTL_XXH64_P1;
        h64 ^= v2;
        h64 = h64 * OPENCSTL_XXH64_P1 + OPENCSTL_XXH64_P4;
        v3 = OPENCSTL_XXH_ROTL64(v3, 31) * OPENCSTL_XXH64_P1;
        h64 ^= v3;
        h64 = h64 * OPENCSTL_XXH64_P1 + OPENCSTL_XXH64_P4;
        v4 = OPENCSTL_XXH_ROTL64(v4, 31) * OPENCSTL_XXH64_P1;
        h64 ^= v4;
        h64 = h64 * OPENCSTL_XXH64_P1 + OPENCSTL_XXH64_P4;
    } else {
        h64 = OPENCSTL_XXH64_P5;
    }

    h64 += (unsigned long long) len;

    while (p + 8 <= end) {
        memcpy(&k, p, 8);
        k = OPENCSTL_XXH_ROTL64(k * OPENCSTL_XXH64_P2, 31) * OPENCSTL_XXH64_P1;
        h64 ^= k;
        h64 = OPENCSTL_XXH_ROTL64(h64, 27) * OPENCSTL_XXH64_P1 + OPENCSTL_XXH64_P4;
        p += 8;
    }

    if (p + 4 <= end) {
        unsigned int k32;
        memcpy(&k32, p, 4);
        h64 ^= (unsigned long long) k32 * OPENCSTL_XXH64_P1;
        h64 = OPENCSTL_XXH_ROTL64(h64, 23) * OPENCSTL_XXH64_P2 + OPENCSTL_XXH64_P3;
        p += 4;
    }

    while (p < end) {
        h64 ^= (unsigned long long) (*p) * OPENCSTL_XXH64_P5;
        h64 = OPENCSTL_XXH_ROTL64(h64, 11) * OPENCSTL_XXH64_P1;
        p++;
    }

    h64 ^= h64 >> 33;
    h64 *= OPENCSTL_XXH64_P2;
    h64 ^= h64 >> 29;
    h64 *= OPENCSTL_XXH64_P3;
    h64 ^= h64 >> 32;

    return (size_type64) h64;
}

/* Single 64x64->128 multiply + fold: one instruction on x86/x64.
 *   - GCC/Clang: __uint128_t expands to a native `mulq`.
 *   - MSVC x64: the _umul128 intrinsic (same `mulq`, no scalar fallback).
 *   - Otherwise: a 4-multiply scalar fallback. */
OPENCSTL_ALWAYS_INLINE uint64_t __ht_mum(uint64_t a, uint64_t b) {
#if defined(__SIZEOF_INT128__)
    __uint128_t r = (__uint128_t) a * b;
    return (uint64_t) r ^ (uint64_t) (r >> 64);
#elif defined(_MSC_VER) && defined(_M_X64)
    uint64_t hi;
    uint64_t lo = _umul128(a, b, &hi);
    return lo ^ hi;
#else
    uint64_t ha = a >> 32, la = (uint32_t) a;
    uint64_t hb = b >> 32, lb = (uint32_t) b;
    return ha * hb ^ ((ha * lb + hb * la) >> 32) ^ (la * lb >> 32);
#endif
}

OPENCSTL_ALWAYS_INLINE size_type64 hash_mixer(void *key, size_type64 n) {
    static const uint64_t s0 = 0xa0761d6478bd642fULL;
    static const uint64_t s1 = 0xe7037ed1a0b428dbULL;
    if (n == 4) {
        uint32_t v;
        memcpy(&v, key, 4);
        return (size_type64) __ht_mum((uint64_t) v ^ s1, s0);
    }
    if (n == 8) {
        uint64_t v;
        memcpy(&v, key, 8);
        return (size_type64) __ht_mum(v ^ s1, s0);
    }
    return (size_type64) hash(key, n);
}

OPENCSTL_ALWAYS_INLINE uint8_t __ht_ctrl(size_type64 h) {
    /* Non-zero fragment in [1, 255] — high bit set keeps occupied ctrls well clear of empty. */
    return (uint8_t) ((h >> 56) | 0x80u);
}

#define __HASHTABLE_DEFAULT_SIZE__ HT_MIN_CAP

static uint8_t *__ht_alloc_ctrl(size_type64 cap) {
    /* calloc zeros the ctrl array; every slot starts as empty. */
    uint8_t *m = (uint8_t *) calloc(cap, 1);
    verify(m!=NULL);
    return m;
}

/* Reinsert a single key (and optional value) into a freshly-allocated table. */
static void __ht_reinsert(
    void *base, uint8_t *ctrl, size_type64 cap_mask,
    const void *key, const void *value,
    size_type64 key_size, size_type64 value_size, size_type64 type_size, size_type64 h
) {
    size_type64 idx = h & cap_mask;
    uint8_t c = __ht_ctrl(h);
    while (ctrl[idx] != HT_CTRL_EMPTY) idx = (idx + 1) & cap_mask;
    ctrl[idx] = c;
    memcpy((char *) base + idx * type_size, key, key_size);
    if (value_size && value)
        memcpy((char *) base + idx * type_size + key_size, value, value_size);
}

static void __ht_do_rehash(
    void **container, size_type64 header_sz,
    size_type64 key_size, size_type64 value_size, size_type64 type_size,
    size_type64 old_cap, uint8_t *old_ctrl
) {
    size_type64 new_cap = old_cap * 2;
    void *new_raw = calloc(header_sz + new_cap * type_size, 1);
    verify(new_raw!=NULL);
    memcpy(new_raw, (char *) *container - header_sz, header_sz);
    uint8_t *new_ctrl = __ht_alloc_ctrl(new_cap);
    void *nb = (char *) new_raw + header_sz;
    size_type64 new_mask = new_cap - 1;

    for (size_type64 i = 0; i < old_cap; i++) {
        if (old_ctrl[i] != HT_CTRL_EMPTY) {
            const void *ok = (const char *) *container + i * type_size;
            const void *ov = value_size > 0 ? (const char *) ok + key_size : NULL;
            size_type64 h = hash_mixer((void *) ok, key_size);
            __ht_reinsert(nb, new_ctrl, new_mask, ok, ov,
                          key_size, value_size, type_size, h);
        }
    }

    void *old_ptr = *container;
    free((char *) *container - header_sz);
    free(old_ctrl);
    *container = nb;
    OPENCSTL_NIDX(container, -7) = new_mask;
    OPENCSTL_NIDX(container, -6) = (size_type64) (uintptr_t) new_ctrl;
    htm_erase(htm, old_ptr);
    htm_insert(htm, *container,
               (char *) *container + type_size * new_cap,
               (char *) new_ctrl, (int) type_size);
}

OPENCSTL_FUNC

void __cstl_hashtable_insert(void **container, void *key, void *value) {
    size_type64 key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 value_size = OPENCSTL_NIDX(container, -4);
    size_type64 type_size = key_size + value_size;
    size_type64 cap_mask = OPENCSTL_NIDX(container, -7);
    uint8_t *ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float_key = OPENCSTL_NIDX(container, -8);
    size_type64 is_float_value = OPENCSTL_NIDX(container, -9);
    float kf = 0.f, vf = 0.f;
    if (is_float_key) {
        kf = (float) *(double *) key;
        key = &kf;
    }
    if (is_float_value) {
        vf = (float) *(double *) value;
        value = &vf;
    }
#endif

    size_type64 h = hash_mixer(key, key_size);
    uint8_t c = __ht_ctrl(h);
    size_type64 idx = h & cap_mask;
    char *base = (char *) *container;

    /* Hot loop: probe for duplicate or empty slot. Rehash threshold is only
     * consulted on the rare brand-new-slot exit below. */
    while (true) {
        uint8_t m = ctrl[idx];
        if (m == HT_CTRL_EMPTY) break;
        if (m == c && __ht_key_eq(base + idx * type_size, key, key_size))
            return;
        idx = (idx + 1) & cap_mask;
    }

    size_type64 length = OPENCSTL_NIDX(container, -1);
    if (OPENCSTL_UNLIKELY(length + 1 > HT_THRESHOLD(cap_mask + 1))) {
        size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
        __ht_do_rehash(container, header_sz, key_size, value_size, type_size,
                       cap_mask + 1, ctrl);
        cap_mask = OPENCSTL_NIDX(container, -7);
        ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
        base = (char *) *container;
        idx = h & cap_mask;
        while (ctrl[idx] != HT_CTRL_EMPTY) idx = (idx + 1) & cap_mask;
    }

    ctrl[idx] = c;
    memcpy(base + idx * type_size, key, key_size);
    if (value_size && value)
        memcpy(base + idx * type_size + key_size, value, value_size);
    OPENCSTL_NIDX(container, -1) = length + 1;
}

OPENCSTL_FUNC

void __cstl_hashtable_erase(void **container, void *key) {
    size_type64 key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 cap_mask = OPENCSTL_NIDX(container, -7);
    uint8_t *ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    size_type64 value_size = OPENCSTL_NIDX(container, -4);
    size_type64 type_size = key_size + value_size;
#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float_key = OPENCSTL_NIDX(container, -8);
    float kf = 0.f;
    if (is_float_key) {
        kf = (float) *(double *) key;
        key = &kf;
    }
#endif
    size_type64 h = hash_mixer(key, key_size);
    uint8_t c = __ht_ctrl(h);
    size_type64 idx = h & cap_mask;
    char *base = (char *) *container;

    while (true) {
        uint8_t m = ctrl[idx];
        if (m == HT_CTRL_EMPTY) return;
        if (m == c && __ht_key_eq(base + idx * type_size, key, key_size))
            break;
        idx = (idx + 1) & cap_mask;
    }

    /* Backward-shift deletion for plain linear probing. We scan forward until
     * the cluster's trailing empty slot, shifting any displaced entry whose
     * ideal home lies at or before the hole. Entries already at their home
     * stay put but we must keep scanning past them -- later slots may hold
     * entries from the same probe cluster whose chain would otherwise break. */
    size_type64 hole = idx;
    size_type64 j = (idx + 1) & cap_mask;
    while (ctrl[j] != HT_CTRL_EMPTY) {
        size_type64 nh = hash_mixer(base + j * type_size, key_size) & cap_mask;
        size_type64 dist_j = (j - nh) & cap_mask;
        size_type64 dist_hole = (hole - nh) & cap_mask;
        if (dist_hole < dist_j) {
            memcpy(base + hole * type_size, base + j * type_size, type_size);
            ctrl[hole] = ctrl[j];
            hole = j;
        }
        j = (j + 1) & cap_mask;
    }
    ctrl[hole] = HT_CTRL_EMPTY;
    OPENCSTL_NIDX(container, -1)--;
}

OPENCSTL_FUNC

void *__cstl_hashtable_find(void **container, void *key) {
    size_type64 key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 cap_mask = OPENCSTL_NIDX(container, -7);
    uint8_t *ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    size_type64 value_size = OPENCSTL_NIDX(container, -4);
    size_type64 type_size = key_size + value_size;
#if !defined(__linux__) && !defined(__APPLE__)
    size_type64 is_float_key = OPENCSTL_NIDX(container, -8);
    float kf = 0.f;
    if (is_float_key) {
        kf = (float) *(double *) key;
        key = &kf;
    }
#endif
    size_type64 h = hash_mixer(key, key_size);
    uint8_t c = __ht_ctrl(h);
    size_type64 idx = h & cap_mask;
    char *base = (char *) *container;

    while (true) {
        uint8_t m = ctrl[idx];
        if (m == HT_CTRL_EMPTY) return NULL;
        if (m == c && __ht_key_eq(base + idx * type_size, key, key_size))
            return base + idx * type_size;
        idx = (idx + 1) & cap_mask;
    }
}

OPENCSTL_FUNC

void __cstl_hashtable_clear(void **container) {
    size_type64 cap_mask = OPENCSTL_NIDX(container, -7);
    uint8_t *ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    memset(ctrl, 0, cap_mask + 1);
    OPENCSTL_NIDX(container, -1) = 0;
}

OPENCSTL_FUNC

void *__cstl_hashtable_begin(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE) + OPENCSTL_NIDX(container, -4);
    size_type64 cap_mask = OPENCSTL_NIDX(container, -7);
    uint8_t *ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    for (size_type64 i = 0; i <= cap_mask; i++)
        if (ctrl[i] != HT_CTRL_EMPTY)
            return (char *) *container + i * type_size;
    return NULL;
}

OPENCSTL_FUNC
void *__cstl_hashtable_end(void **container) { return NULL; }

OPENCSTL_FUNC

void *__cstl_hashtable_rbegin(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE) + OPENCSTL_NIDX(container, -4);
    size_type64 cap_mask = OPENCSTL_NIDX(container, -7);
    uint8_t *ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    for (int i = (int) cap_mask; i >= 0; i--)
        if (ctrl[i] != HT_CTRL_EMPTY)
            return (char *) *container + i * type_size;
    return NULL;
}

OPENCSTL_FUNC

void *__cstl_hashtable_rend(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE) + OPENCSTL_NIDX(container, -4);
    return (char *) *container - type_size;
}

OPENCSTL_FUNC

void *__cstl_hashtable_empty(void **container) {
    return __cstl_hashtable_rend(container);
}

OPENCSTL_FUNC size_type __cstl_hashtable_size(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -1);
}

OPENCSTL_FUNC size_type __cstl_hashtable_capacity(void **container) {
    return (size_type) OPENCSTL_NIDX(container, -7) + 1;
}

OPENCSTL_FUNC

void *__cstl_hashtable_next_prev(void *it, int n) {
    HashtableManager *chtm = htm_find(htm, it);

    size_type64 ts = (size_type64) chtm->type_size;
    size_type64 cap = ((char *) chtm->p2 - (char *) chtm->p1) / ts;
    uint8_t *ctrl = (uint8_t *) chtm->tombstone;

    if (n == -1) {
        size_type64 pos = ((char *) it - (char *) chtm->p1) / ts + 1;
        for (; pos < cap; pos++)
            if (ctrl[pos] != HT_CTRL_EMPTY)
                return (char *) chtm->p1 + pos * ts;
        return NULL;
    }
    if (n == -2) {
        size_type64 pos = ((char *) it - (char *) chtm->p1) / ts;
        if (pos == 0) return (char *) chtm->p1 - ts;

        for (size_type64 i = pos - 1; ; i--) {
            if (ctrl[i] != HT_CTRL_EMPTY)
                return (char *) chtm->p1 + i * ts;
            if (i == 0) break;
        }
        return (char *) chtm->p1 - ts;
    }
    return NULL;
}

OPENCSTL_FUNC

void __cstl_hashtable_free(void **container) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    uint8_t *ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);

    HashtableManager *chtm = htm_find(htm, *container);
    htm_erase(htm, chtm->p1);
    free(ctrl);
    free((char *) (*container) - header_sz);
    *container = NULL;
}

OPENCSTL_FUNC

void __cstl_hashtable_reserve(void **container, size_type64 n) {
    size_type64 header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_type64 key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 value_size = OPENCSTL_NIDX(container, -4);
    size_type64 type_size = key_size + value_size;
    size_type64 length = OPENCSTL_NIDX(container, -1);
    size_type64 cap_mask_old = OPENCSTL_NIDX(container, -7);
    uint8_t *old_ctrl = (uint8_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);

    size_type64 want = length + n;
    size_type64 min_cap = (want * HT_LOAD_DEN + (HT_LOAD_NUM - 1)) / HT_LOAD_NUM;
    if (min_cap < __HASHTABLE_DEFAULT_SIZE__) min_cap = __HASHTABLE_DEFAULT_SIZE__;
    size_type64 new_cap = __ht_next_pow2(min_cap);
    if (new_cap <= cap_mask_old + 1) return;

    void *new_raw = calloc(header_sz + new_cap * type_size, 1);
    verify(new_raw!=NULL);
    memcpy(new_raw, (char *) *container - header_sz, header_sz);
    uint8_t *new_ctrl = __ht_alloc_ctrl(new_cap);
    void *nb = (char *) new_raw + header_sz;
    size_type64 new_mask = new_cap - 1;

    for (size_type64 i = 0; i <= cap_mask_old; i++) {
        if (old_ctrl[i] != HT_CTRL_EMPTY) {
            const void *ok = (const char *) *container + i * type_size;
            const void *ov = value_size > 0 ? (const char *) ok + key_size : NULL;
            size_type64 h = hash_mixer((void *) ok, key_size);
            __ht_reinsert(nb, new_ctrl, new_mask, ok, ov,
                          key_size, value_size, type_size, h);
        }
    }

    void *old_ptr = *container;
    free((char *) *container - header_sz);
    free(old_ctrl);
    *container = nb;
    OPENCSTL_NIDX(container, -7) = new_mask;
    OPENCSTL_NIDX(container, -6) = (size_type64) (uintptr_t) new_ctrl;
    htm_erase(htm, old_ptr);
    htm_insert(htm, *container,
               (char *) *container + type_size * new_cap,
               (char *) new_ctrl, (int) type_size);
}

void __cstl_htm_destroy(void) {
    htm_free(htm);
}

// ██╗░░░██╗███╗░░██╗░█████╗░██████╗░██████╗░███████╗██████╗░███████╗██████╗░░░░░░░░██████╗███████╗████████╗
// ██║░░░██║████╗░██║██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔════╝██╔══██╗░░░░░░██╔════╝██╔════╝╚══██╔══╝
// ██║░░░██║██╔██╗██║██║░░██║██████╔╝██║░░██║█████╗░░██████╔╝█████╗░░██║░░██║░░░░░░╚█████╗░█████╗░░░░░██║░░░
// ██║░░░██║██║╚████║██║░░██║██╔══██╗██║░░██║██╔══╝░░██╔══██╗██╔══╝░░██║░░██║░░░░░░░╚═══██╗██╔══╝░░░░░██║░░░
// ╚██████╔╝██║░╚███║╚█████╔╝██║░░██║██████╔╝███████╗██║░░██║███████╗██████╔╝█████╗██████╔╝███████╗░░░██║░░░
// ░╚═════╝░╚═╝░░╚══╝░╚════╝░╚═╝░░╚═╝╚═════╝░╚══════╝╚═╝░░╚═╝╚══════╝╚═════╝░╚════╝╚═════╝░╚══════╝░░░╚═╝░░░

/* MSVC's classic preprocessor does not eat the trailing comma before an empty
 * `##__VA_ARGS__`, which would leave an empty FUNC parameter and produce a
 * bogus `(void*)()` cast. We avoid that by always padding with NULL and using
 * the EXPAND trick so MSVC correctly re-tokenises the forwarded __VA_ARGS__. */
#define _CSTL_USET_EXPAND(x) x
#define cstl_unordered_set _cstl_unordered_set
#define _cstl_unordered_set(...) _CSTL_USET_EXPAND(_CSTL_USET_DISPATCH(__VA_ARGS__, NULL, NULL))
#define _CSTL_USET_DISPATCH(KEY, FUNC, ...) __cstl_unordered_set(sizeof(KEY),#KEY,(void*)(FUNC))
OPENCSTL_FUNC

void *__cstl_unordered_set(size_type64 key_size, const char *type_key, void *hash_func) {
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    size_type64 cap = __HASHTABLE_DEFAULT_SIZE__;
    void *ptr = (char *) calloc(header_sz + key_size * cap, 1) + header_sz;
    void **c = &ptr;
    uint8_t *ctrl = __ht_alloc_ctrl(cap);
    OPENCSTL_NIDX(c, NIDX_CTYPE) = OPENCSTL_UNORDERED_SET;
    OPENCSTL_NIDX(c, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(c, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(c, -9) = 0;
    OPENCSTL_NIDX(c, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(c, -7) = cap - 1;
    OPENCSTL_NIDX(c, -6) = (size_type64) (uintptr_t) ctrl;
    OPENCSTL_NIDX(c, -4) = 0;
    OPENCSTL_NIDX(c, -3) = (size_type64) type_key;
    OPENCSTL_NIDX(c, -2) = (size_type64) hash_func;
    OPENCSTL_NIDX(c, -1) = 0;
    OPENCSTL_NIDX(c, 0) = 0;
    bool htm_init = false;
    if (htm == NULL) {
        htm = htm_new();
        htm_init = true;
    }
    htm_insert(htm, ptr, (char *) ptr + (key_size * cap), (char *) ctrl, (int) key_size);

    if (htm_init) {
        atexit(__cstl_htm_destroy);
    }
    return ptr;
}

// ██╗░░░██╗███╗░░██╗░█████╗░██████╗░██████╗░███████╗██████╗░███████╗██████╗░░░░░░░███╗░░░███╗░█████╗░██████╗░
// ██║░░░██║████╗░██║██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔════╝██╔══██╗░░░░░░████╗░████║██╔══██╗██╔══██╗
// ██║░░░██║██╔██╗██║██║░░██║██████╔╝██║░░██║█████╗░░██████╔╝█████╗░░██║░░██║░░░░░░██╔████╔██║███████║██████╔╝
// ██║░░░██║██║╚████║██║░░██║██╔══██╗██║░░██║██╔══╝░░██╔══██╗██╔══╝░░██║░░██║░░░░░░██║╚██╔╝██║██╔══██║██╔═══╝░
// ╚██████╔╝██║░╚███║╚█████╔╝██║░░██║██████╔╝███████╗██║░░██║███████╗██████╔╝█████╗██║░╚═╝░██║██║░░██║██║░░░░░
// ░╚═════╝░╚═╝░░╚══╝░╚════╝░╚═╝░░╚═╝╚═════╝░╚══════╝╚═╝░░╚═╝╚══════╝╚═════╝░╚════╝╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░░░░
/* Same MSVC-compatible pattern as unordered_set: always pad NULL, force a
 * re-scan via EXPAND, so the optional hash-function argument is allowed to be
 * absent on MSVC's classic preprocessor. */
#define _CSTL_UMAP_EXPAND(x) x
#define cstl_unordered_map(...) _CSTL_UMAP_EXPAND(_CSTL_UMAP_DISPATCH(__VA_ARGS__, NULL, NULL))
#define _cstl_unordered_map(...) _CSTL_UMAP_EXPAND(_CSTL_UMAP_DISPATCH(__VA_ARGS__, NULL, NULL))
#define _CSTL_UMAP_DISPATCH(KEY,VALUE,FUNC,...) __cstl_unordered_map(sizeof(KEY),sizeof(VALUE),#KEY,#VALUE,(void*)(FUNC))
OPENCSTL_FUNC

void *__cstl_unordered_map(size_type64 key_size, size_type64 value_size,
                           const char *type_key, const char *type_value,
                           void *hash_func) {
    size_type64 header_sz = sizeof(size_type64) * OPENCSTL_HEADER;
    size_type64 type_size = key_size + value_size;
    size_type64 cap = __HASHTABLE_DEFAULT_SIZE__;
    void *ptr = (char *) calloc(header_sz + type_size * cap, 1) + header_sz;
    void **c = &ptr;
    uint8_t *ctrl = __ht_alloc_ctrl(cap);
    OPENCSTL_NIDX(c, NIDX_CTYPE) = OPENCSTL_UNORDERED_MAP;
    OPENCSTL_NIDX(c, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(c, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(c, -9) = !strcmp(type_value, "float");
    OPENCSTL_NIDX(c, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(c, -7) = cap - 1;
    OPENCSTL_NIDX(c, -6) = (size_type64) (uintptr_t) ctrl;
    OPENCSTL_NIDX(c, -4) = value_size;
    OPENCSTL_NIDX(c, -3) = (size_type64) type_key;
    OPENCSTL_NIDX(c, -2) = (size_type64) hash_func;
    OPENCSTL_NIDX(c, -1) = 0;
    OPENCSTL_NIDX(c, 0) = 0;
    bool htm_init = false;
    if (htm == NULL) {
        htm = htm_new();
        htm_init = true;
    }
    htm_insert(htm, ptr, (char *) ptr + (type_size * cap), (char *) ctrl, (int) type_size);
    if (htm_init) {
        atexit(__cstl_htm_destroy);
    }
    return ptr;
}
#endif
