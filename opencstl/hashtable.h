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
#define HT_EMPTY     0x0000U
#define HT_FRAG_MASK 0xF000U
#define HT_IN_HOME   0x0800U
#define HT_DISP_MASK 0x07FFU
#define HT_DISP_END  0x07FFU
#define HT_LOAD      0.875
#define HT_MIN_CAP   8U
/* threshold = cap * 7/8, computed via shifts (no fp) */
#define HT_THRESHOLD(cap) ((cap) - ((cap) >> 3))
#define HTM_VEBT

static inline size_t __ht_next_pow2(size_t n) {
    size_t p = HT_MIN_CAP;
    while (p < n) p <<= 1;
    return p;
}

static inline bool __ht_key_eq(const void *a, const void *b, size_t ks) {
    switch (ks) {
        case 1: return *(const uint8_t *) a == *(const uint8_t *) b;
        case 2: return *(const uint16_t *) a == *(const uint16_t *) b;
        case 4: return *(const uint32_t *) a == *(const uint32_t *) b;
        case 8: return *(const uint64_t *) a == *(const uint64_t *) b;
        default: return memcmp(a, b, ks) == 0;
    }
}

static inline uint16_t __ht_frag(size_t h) {
    return (uint16_t) ((h >> ((sizeof(size_t) - sizeof(uint16_t)) * 8)) & HT_FRAG_MASK);
}

static inline size_t __ht_quad(uint16_t d) {
    return ((size_t) d * d + d) / 2;
}


#define OPENCSTL_XXH_ROTL32(x, r) (((x) << (r)) | ((x) >> (32 - (r))))
#define OPENCSTL_XXH_ROTL64(x, r) (((x) << (r)) | ((x) >> (64 - (r))))

// XXH32 primes
#define OPENCSTL_XXH32_P2 2246822519U     // 0x85EBCA77
#define OPENCSTL_XXH32_P3 3266489917U     // 0xC2B2AE3D

// XXH64 primes
#define OPENCSTL_XXH64_P1 11400714785074694791ULL  // 0x9E3779B185EBCA87
#define OPENCSTL_XXH64_P2 14029467366897019727ULL  // 0xC2B2AE3D27D4EB4F
#define OPENCSTL_XXH64_P3  1609587929392839161ULL  // 0x165667B19E3779F9
#define OPENCSTL_XXH64_P4  9650029242287828579ULL  // 0x85EBCA77C2B2AE63
#define OPENCSTL_XXH64_P5  2870177450012600261ULL  // 0x27D4EB2F165667C5

OPENCSTL_FUNC size_t hash32(void *_key) {
    unsigned int h = *(unsigned int *) _key;
    // XXH32 avalanche
    h ^= h >> 15;
    h *= OPENCSTL_XXH32_P2;
    h ^= h >> 13;
    h *= OPENCSTL_XXH32_P3;
    h ^= h >> 16;
    return (size_t) h;
}

OPENCSTL_FUNC size_t hash64(void *_key) {
    unsigned long long x = *(unsigned long long *) _key;
    // XXH64 avalanche
    x ^= x >> 33;
    x *= OPENCSTL_XXH64_P2;
    x ^= x >> 29;
    x *= OPENCSTL_XXH64_P3;
    x ^= x >> 32;
    return (size_t) x;
}

OPENCSTL_FUNC size_t hash(void *_key, size_t len) {
    const unsigned char *p = (const unsigned char *) _key;
    const unsigned char *end = p + len;
    unsigned long long h64;
    unsigned long long k;

    // --- stripe loop (32 bytes/iter, 4-lane unrolled) ---
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

        // merge accumulators
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

    // --- tail: 8-byte chunks ---
    while (p + 8 <= end) {
        memcpy(&k, p, 8);
        k = OPENCSTL_XXH_ROTL64(k * OPENCSTL_XXH64_P2, 31) * OPENCSTL_XXH64_P1;
        h64 ^= k;
        h64 = OPENCSTL_XXH_ROTL64(h64, 27) * OPENCSTL_XXH64_P1 + OPENCSTL_XXH64_P4;
        p += 8;
    }

    // --- tail: 4-byte chunk ---
    if (p + 4 <= end) {
        unsigned int k32;
        memcpy(&k32, p, 4);
        h64 ^= (unsigned long long) k32 * OPENCSTL_XXH64_P1;
        h64 = OPENCSTL_XXH_ROTL64(h64, 23) * OPENCSTL_XXH64_P2 + OPENCSTL_XXH64_P3;
        p += 4;
    }

    // --- tail: remaining bytes ---
    while (p < end) {
        h64 ^= (unsigned long long) (*p) * OPENCSTL_XXH64_P5;
        h64 = OPENCSTL_XXH_ROTL64(h64, 11) * OPENCSTL_XXH64_P1;
        p++;
    }

    // --- final avalanche ---
    h64 ^= h64 >> 33;
    h64 *= OPENCSTL_XXH64_P2;
    h64 ^= h64 >> 29;
    h64 *= OPENCSTL_XXH64_P3;
    h64 ^= h64 >> 32;

    return (size_t) h64;
}

// OPENCSTL_FUNC size_t hash32(void *_key) {
//     unsigned int h = *(unsigned int *) _key;
//     h ^= h >> 16;
//     h *= 0x85ebca6b;
//     h ^= h >> 13;
//     h *= 0xc2b2ae35;
//     h ^= h >> 16;
//     return (size_t) h;
// }
//
// OPENCSTL_FUNC size_t hash64(void *_key) {
//     unsigned long long x = *(unsigned long long *) _key;
//     x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
//     x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
//     return (size_t) (x ^ (x >> 31));
// }
//
// OPENCSTL_FUNC size_t hash(void *_key, size_t n) {
//     unsigned char *key = (unsigned char *) _key;
//     size_t ret = 0xCBF29CE484222325ULL;
//     while (n >= sizeof(size_t)) {
//         ret ^= (sizeof(size_t) == 8) ? hash64(key) : hash32(key);
//         ret *= 0x100000001b3ULL;
//         key += sizeof(size_t);
//         n -= sizeof(size_t);
//     }
//     return ret;
// }

static inline uint64_t __ht_mum(uint64_t a, uint64_t b) {
#ifdef __SIZEOF_INT128__
    __uint128_t r = (__uint128_t) a * b;
    return (uint64_t) r ^ (uint64_t) (r >> 64);
#else
    uint64_t ha = a >> 32, la = (uint32_t) a;
    uint64_t hb = b >> 32, lb = (uint32_t) b;
    return ha * hb ^ ((ha * lb + hb * la) >> 32) ^ (la * lb >> 32);
#endif
}

#if defined(__GNUC__) || defined(__clang__)
#define OPENCSTL_ALWAYS_INLINE static inline __attribute__((always_inline))
#define OPENCSTL_LIKELY(x)   __builtin_expect(!!(x), 1)
#define OPENCSTL_UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
#define OPENCSTL_ALWAYS_INLINE static inline
#define OPENCSTL_LIKELY(x)   (x)
#define OPENCSTL_UNLIKELY(x) (x)
#endif

OPENCSTL_ALWAYS_INLINE size_t hash_mixer(void *key, size_t n) {
    static const uint64_t s0 = 0xa0761d6478bd642fULL;
    static const uint64_t s1 = 0xe7037ed1a0b428dbULL;
    if (n == 4) {
        uint32_t v;
        memcpy(&v, key, 4);
        return (size_t) __ht_mum((uint64_t) v ^ s1, s0);
    }
    if (n == 8) {
        uint64_t v;
        memcpy(&v, key, 8);
        return (size_t) __ht_mum(v ^ s1, s0);
    }
    const uint8_t *p = (const uint8_t *) key;
    uint64_t a, b;
    if (n <= 16 && n >= 4) {
        uint32_t v0, v1, v2, v3;
        memcpy(&v0, p, 4);
        memcpy(&v1, p + ((n >> 3) << 2), 4);
        memcpy(&v2, p + n - 4, 4);
        memcpy(&v3, p + n - 4 - ((n >> 3) << 2), 4);
        a = ((uint64_t) v0 << 32) | v1;
        b = ((uint64_t) v2 << 32) | v3;
    } else if (n < 4 && n > 0) {
        a = ((uint64_t) p[0] << 16) | ((uint64_t) p[n >> 1] << 8) | p[n - 1];
        b = 0;
    } else {
        uint64_t h = s0;
        while (n >= 8) {
            uint64_t k;
            memcpy(&k, p, 8);
            k ^= k >> 30;
            k *= 0xbf58476d1ce4e5b9ULL;
            k ^= k >> 27;
            k *= 0x94d049bb133111ebULL;
            k ^= k >> 31;
            h ^= k;
            h *= s0;
            p += 8;
            n -= 8;
        }
        return (size_t) h;
    }
    return (size_t) __ht_mum(s1 ^ (uint64_t) n, __ht_mum(a ^ s1, b ^ s0));
}

OPENCSTL_ALWAYS_INLINE bool __ht_find_empty(
    uint16_t *meta, size_t cap_mask, size_t home,
    size_t *empty, uint16_t *disp
) {
    uint16_t d = 1;
    size_t linear = 1;
    while (true) {
        size_t slot = (home + linear) & cap_mask;
        if (meta[slot] == HT_EMPTY) {
            *empty = slot;
            *disp = d;
            return true;
        }
        if (++d == HT_DISP_END) return false;
        linear += d;
    }
}

OPENCSTL_ALWAYS_INLINE size_t __ht_find_chain_pos(
    uint16_t *meta, size_t cap_mask, size_t home, uint16_t disp_to_empty
) {
    size_t candidate = home;
    while (true) {
        uint16_t d = meta[candidate] & HT_DISP_MASK;
        if (d > disp_to_empty) return candidate;
        candidate = (home + __ht_quad(d)) & cap_mask;
    }
}

static inline bool __ht_evict(
    void *base, uint16_t *meta, size_t cap_mask,
    size_t bucket, size_t type_size, size_t key_size
) {
    size_t h = hash_mixer((char *) base + bucket * type_size, key_size);
    size_t home = h & cap_mask;
    size_t prev = home;
    while (true) {
        uint16_t d = meta[prev] & HT_DISP_MASK;
        size_t nx = (home + __ht_quad(d)) & cap_mask;
        if (nx == bucket) break;
        prev = nx;
    }
    meta[prev] = (uint16_t) ((meta[prev] & ~HT_DISP_MASK) | (meta[bucket] & HT_DISP_MASK));
    size_t empty;
    uint16_t new_disp;
    if (!__ht_find_empty(meta, cap_mask, home, &empty, &new_disp)) return false;
    size_t ins_prev = __ht_find_chain_pos(meta, cap_mask, home, new_disp);
    memcpy((char *) base + empty * type_size,
           (char *) base + bucket * type_size, type_size);
    meta[empty] = (uint16_t) ((meta[bucket] & HT_FRAG_MASK) | (meta[ins_prev] & HT_DISP_MASK));
    meta[ins_prev] = (uint16_t) ((meta[ins_prev] & ~HT_DISP_MASK) | new_disp);
    return true;
}

static inline bool __ht_reinsert(
    void *base, uint16_t *meta, size_t cap_mask,
    void *key, void *value, size_t key_size, size_t value_size, size_t key_hash
) {
    size_t type_size = key_size + value_size;
    uint16_t frag = __ht_frag(key_hash);
    size_t home = key_hash & cap_mask;
    if (!(meta[home] & HT_IN_HOME)) {
        if (meta[home] != HT_EMPTY &&
            !__ht_evict(base, meta, cap_mask, home, type_size, key_size))
            return false;
        memcpy((char *) base + home * type_size, key, key_size);
        if (value)
            memcpy((char *) base + home * type_size + key_size, value, value_size);
        meta[home] = frag | HT_IN_HOME | HT_DISP_END;
        return true;
    }
    size_t empty;
    uint16_t disp;
    if (!__ht_find_empty(meta, cap_mask, home, &empty, &disp)) return false;
    size_t prev = __ht_find_chain_pos(meta, cap_mask, home, disp);
    memcpy((char *) base + empty * type_size, key, key_size);
    if (value)
        memcpy((char *) base + empty * type_size + key_size, value, value_size);
    meta[empty] = (uint16_t) (frag | (meta[prev] & HT_DISP_MASK));
    meta[prev] = (uint16_t) ((meta[prev] & ~HT_DISP_MASK) | disp);
    return true;
}

// 
//
// #define HTM_SIZE 1024
//
// typedef struct {
//     void *p1, *p2;
//     char *tombstone;
//     int type_size;
//     bool used;
// } HashtableManager;
//
// static HashtableManager htm[HTM_SIZE] = {0};
// static size_t htm_length = 0;
//
// void __htm_append(void *ptr, size_t sz, char *tombstone, int type_size) {
//     htm[htm_length].p1 = ptr;
//     htm[htm_length].p2 = (char *) ptr + sz;
//     htm[htm_length].tombstone = tombstone;
//     htm[htm_length].used = true;
//     htm[htm_length].type_size = type_size;
//     htm_length++;
// }

#define __HASHTABLE_DEFAULT_SIZE__ HT_MIN_CAP

static uint16_t *__ht_alloc_meta(size_t cap) {
    uint16_t *m = (uint16_t *) calloc(cap + 4, sizeof(uint16_t));
    if (!m)
        cstl_error("Allocation failed (metadata)");
    m[cap] = 0x0001;
    return m;
}

static void __ht_do_rehash(
    void **container, size_t header_sz,
    size_t key_size, size_t value_size, size_t type_size,
    size_t length, size_t old_cap_mask,
    uint16_t *old_meta
) {
    size_t new_cap = (old_cap_mask + 1) * 2;
    while (true) {
        void *new_raw = calloc(header_sz + new_cap * type_size, 1);
        if (!new_raw)
            cstl_error("Allocation failed (rehash)");
        memcpy(new_raw, (char *) *container - header_sz, header_sz);
        uint16_t *new_meta = __ht_alloc_meta(new_cap);
        void *nb = (char *) new_raw + header_sz;
        size_t new_mask = new_cap - 1;
        size_t done = 0;
        for (size_t i = 0; i <= old_cap_mask; i++) {
            if (old_meta[i] != HT_EMPTY) {
                void *ok = (char *) *container + i * type_size;
                void *ov = value_size > 0 ? (char *) ok + key_size : NULL;
                size_t h = hash_mixer(ok, key_size);
                if (__ht_reinsert(nb, new_meta, new_mask, ok, ov, key_size, value_size, h))
                    done++;
            }
        }
        if (done < length) {
            free(new_raw);
            free(new_meta);
            new_cap *= 2;
            continue;
        }
        void *old_ptr = *container;
        free((char *) *container - header_sz);
        free(old_meta);
        *container = nb;
        OPENCSTL_NIDX(container, -7) = new_mask;
        OPENCSTL_NIDX(container, -6) = (size_t) (uintptr_t) new_meta;
        htm_erase(htm, old_ptr);
        htm_insert(htm, *container,
                   (char *) *container + type_size * new_cap,
                   (char *) new_meta, (int) type_size);
        break;
    }
}

OPENCSTL_FUNC

void __cstl_hashtable_insert(void **container, void *key, void *value) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    size_t type_size = key_size + value_size;
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t cap_mask = OPENCSTL_NIDX(container, -7);
    uint16_t *meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t is_float_value = OPENCSTL_NIDX(container, -9);
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
    size_t key_hash = hash_mixer(key, key_size);
    uint16_t frag = __ht_frag(key_hash);
    size_t home0 = key_hash & cap_mask;
    if (meta[home0] & HT_IN_HOME) {
        size_t b = home0;
        while (true) {
            if ((meta[b] & HT_FRAG_MASK) == frag &&
                __ht_key_eq((char *) *container + b * type_size, key, key_size))
                return;
            uint16_t d = meta[b] & HT_DISP_MASK;
            if (d == HT_DISP_END) break;
            b = (home0 + __ht_quad(d)) & cap_mask;
        }
    }
    while (true) {
        size_t cap = cap_mask + 1;
        size_t home = key_hash & cap_mask;
        if (length + 1 > HT_THRESHOLD(cap)) goto do_rehash;
        if (!(meta[home] & HT_IN_HOME)) {
            if (meta[home] != HT_EMPTY &&
                !__ht_evict(*container, meta, cap_mask, home, type_size, key_size))
                goto do_rehash;
            memcpy((char *) *container + home * type_size, key, key_size);
            if (value)
                memcpy((char *) *container + home * type_size + key_size, value, value_size);
            meta[home] = frag | HT_IN_HOME | HT_DISP_END;
            OPENCSTL_NIDX(container, -1) = length + 1;
            return;
        }
        {
            size_t empty;
            uint16_t disp;
            if (!__ht_find_empty(meta, cap_mask, home, &empty, &disp))
                goto do_rehash;
            size_t prev = __ht_find_chain_pos(meta, cap_mask, home, disp);
            memcpy((char *) *container + empty * type_size, key, key_size);
            if (value)
                memcpy((char *) *container + empty * type_size + key_size, value, value_size);
            meta[empty] = (uint16_t) (frag | (meta[prev] & HT_DISP_MASK));
            meta[prev] = (uint16_t) ((meta[prev] & ~HT_DISP_MASK) | disp);
            OPENCSTL_NIDX(container, -1) = length + 1;
            return;
        }
    do_rehash:;
        __ht_do_rehash(container, header_sz, key_size, value_size, type_size,
                       length, cap_mask, meta);
        cap_mask = OPENCSTL_NIDX(container, -7);
        meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    }
}

OPENCSTL_FUNC

void __cstl_hashtable_erase(void **container, void *key) {
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t cap_mask = OPENCSTL_NIDX(container, -7);
    uint16_t *meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    size_t type_size = key_size + value_size;
#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    float kf = 0.f;
    if (is_float_key) {
        kf = (float) *(double *) key;
        key = &kf;
    }
#endif
    size_t h = hash_mixer(key, key_size);
    uint16_t frag = __ht_frag(h);
    size_t home = h & cap_mask;
    if (!(meta[home] & HT_IN_HOME)) return;
    size_t prev = home;
    size_t bucket = home;
    bool at_home = true;
    while (true) {
        if ((meta[bucket] & HT_FRAG_MASK) == frag &&
            __ht_key_eq((char *) *container + bucket * type_size, key, key_size))
            goto found;
        uint16_t d = meta[bucket] & HT_DISP_MASK;
        if (d == HT_DISP_END) return;
        prev = bucket;
        bucket = (home + __ht_quad(d)) & cap_mask;
        at_home = false;
    }
found:;
    uint16_t d_next = meta[bucket] & HT_DISP_MASK;
    if (d_next == HT_DISP_END) {
        meta[bucket] = HT_EMPTY;
        if (!at_home)
            meta[prev] = (uint16_t) ((meta[prev] & ~HT_DISP_MASK) | HT_DISP_END);
    } else {
        size_t next = (home + __ht_quad(d_next)) & cap_mask;
        uint16_t d_nn = meta[next] & HT_DISP_MASK;
        uint16_t f_next = meta[next] & HT_FRAG_MASK;
        uint16_t home_flag = meta[bucket] & HT_IN_HOME;
        memcpy((char *) *container + bucket * type_size,
               (char *) *container + next * type_size, type_size);
        meta[bucket] = (uint16_t) (f_next | home_flag | d_nn);
        meta[next] = HT_EMPTY;
    }
    OPENCSTL_NIDX(container, -1)--;
}

OPENCSTL_FUNC

void *__cstl_hashtable_find(void **container, void *key) {
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t cap_mask = OPENCSTL_NIDX(container, -7);
    uint16_t *meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    size_t type_size = key_size + value_size;
#if !defined(__linux__) && !defined(__APPLE__)
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    float kf = 0.f;
    if (is_float_key) {
        kf = (float) *(double *) key;
        key = &kf;
    }
#endif
    size_t h = hash_mixer(key, key_size);
    uint16_t frag = __ht_frag(h);
    size_t home = h & cap_mask;
    if (!(meta[home] & HT_IN_HOME)) return NULL;
    size_t bucket = home;
    while (true) {
        if ((meta[bucket] & HT_FRAG_MASK) == frag &&
            __ht_key_eq((char *) *container + bucket * type_size, key, key_size))
            return (char *) *container + bucket * type_size;
        uint16_t d = meta[bucket] & HT_DISP_MASK;
        if (d == HT_DISP_END) return NULL;
        bucket = (home + __ht_quad(d)) & cap_mask;
    }
}

OPENCSTL_FUNC

void __cstl_hashtable_clear(void **container) {
    size_t cap_mask = OPENCSTL_NIDX(container, -7);
    uint16_t *meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    memset(meta, 0, (cap_mask + 1 + 4) * sizeof(uint16_t));
    meta[cap_mask + 1] = 0x0001;
    OPENCSTL_NIDX(container, -1) = 0;
}

OPENCSTL_FUNC

void *__cstl_hashtable_begin(void **container) {
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE) + OPENCSTL_NIDX(container, -4);
    size_t cap_mask = OPENCSTL_NIDX(container, -7);
    uint16_t *meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    for (size_t i = 0; i <= cap_mask; i++)
        if (meta[i] != HT_EMPTY)
            return (char *) *container + i * type_size;
    return NULL;
}

OPENCSTL_FUNC
void *__cstl_hashtable_end(void **container) { return NULL; }

OPENCSTL_FUNC

void *__cstl_hashtable_rbegin(void **container) {
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE) + OPENCSTL_NIDX(container, -4);
    size_t cap_mask = OPENCSTL_NIDX(container, -7);
    uint16_t *meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    for (int i = (int) cap_mask; i >= 0; i--)
        if (meta[i] != HT_EMPTY)
            return (char *) *container + i * type_size;
    return NULL;
}

OPENCSTL_FUNC

void *__cstl_hashtable_rend(void **container) {
    size_t type_size = OPENCSTL_NIDX(container, NIDX_TSIZE) + OPENCSTL_NIDX(container, -4);
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
    // int idx = -1;
    // for (int i = 0; i < (int) htm_length; i++)
    //     if (htm[i].p1 <= it && it < htm[i].p2) {
    //         idx = i;
    //         break;
    //     }
    //
    //
    // if (idx == -1)
    //     cstl_error("Unregistered hashtable");

    size_t ts = (size_t) chtm->type_size;
    size_t cap = ((char *) chtm->p2 - (char *) chtm->p1) / ts;
    uint16_t *meta = (uint16_t *) chtm->tombstone;

    // size_t ts = (size_t) htm[idx].type_size;
    // size_t cap = ((char *) htm[idx].p2 - (char *) htm[idx].p1) / ts;
    // uint16_t *meta = (uint16_t *) htm[idx].tombstone;
    if (n == -1) {
        //size_t pos = ((char *) it - (char *) htm[idx].p1) / ts + 1;
        size_t pos = ((char *) it - (char *) chtm->p1) / ts + 1;
        for (; pos < cap; pos++)
            if (meta[pos] != HT_EMPTY) {
                //return (char *) htm[idx].p1 + pos * ts;
                return (char *) chtm->p1 + pos * ts;
            }

        return NULL;
    }
    if (n == -2) {
        //size_t pos = ((char *) it - (char *) htm[idx].p1) / ts;
        size_t pos = ((char *) it - (char *) chtm->p1) / ts;
        //if (pos == 0) return (char *) htm[idx].p1 - ts;
        if (pos == 0) return (char *) chtm->p1 - ts;

        for (size_t i = pos - 1; ; i--) {
            if (meta[i] != HT_EMPTY) {
                //return (char *) htm[idx].p1 + i * ts;
                return (char *) chtm->p1 + i * ts;
            }

            if (i == 0) break;
        }
        //return (char *) htm[idx].p1 - ts;
        return (char *) chtm->p1 - ts;
    }
    return NULL;
}

OPENCSTL_FUNC

void __cstl_hashtable_free(void **container) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    uint16_t *meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);

    HashtableManager *chtm = htm_find(htm, *container);
    htm_erase(htm, chtm->p1);
    // int fi = -1;
    // for (int i = 0; i < (int) htm_length; i++)
    //     if (htm[i].p1 == *container) {
    //         fi = i;
    //         break;
    //     }
    // if (fi != -1) {
    //     memmove(&htm[fi], &htm[fi + 1], (htm_length - fi) * sizeof(HashtableManager));
    //     htm_length--;
    //}
    free(meta);
    free((char *) (*container) - header_sz);
    *container = NULL;
}

OPENCSTL_FUNC

void __cstl_hashtable_reserve(void **container, size_t n) {
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    size_t type_size = key_size + value_size;
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t cap_mask_old = OPENCSTL_NIDX(container, -7);
    uint16_t *old_meta = (uint16_t *) (uintptr_t) OPENCSTL_NIDX(container, -6);
    /* Required cap so that length+n fits comfortably under the load factor */
    size_t want = length + n;
    size_t min_cap = (want * 8 + 6) / 7; /* inverse of 7/8 load */
    if (min_cap < __HASHTABLE_DEFAULT_SIZE__) min_cap = __HASHTABLE_DEFAULT_SIZE__;
    size_t new_cap = __ht_next_pow2(min_cap);
    if (new_cap <= cap_mask_old + 1) return; /* already big enough */

    while (true) {
        void *new_raw = calloc(header_sz + new_cap * type_size, 1);
        if (!new_raw)
            cstl_error("Allocation failed (reserve)");
        memcpy(new_raw, (char *) *container - header_sz, header_sz);
        uint16_t *new_meta = __ht_alloc_meta(new_cap);
        void *nb = (char *) new_raw + header_sz;
        size_t new_mask = new_cap - 1;
        size_t done = 0;
        for (size_t i = 0; i <= cap_mask_old; i++) {
            if (old_meta[i] != HT_EMPTY) {
                void *ok = (char *) *container + i * type_size;
                void *ov = value_size > 0 ? (char *) ok + key_size : NULL;
                size_t h = hash_mixer(ok, key_size);
                if (__ht_reinsert(nb, new_meta, new_mask, ok, ov, key_size, value_size, h))
                    done++;
            }
        }
        if (done < length) {
            free(new_raw);
            free(new_meta);
            new_cap *= 2;
            continue;
        }
        void *old_ptr = *container;
        free((char *) *container - header_sz);
        free(old_meta);
        *container = nb;
        OPENCSTL_NIDX(container, -7) = new_mask;
        OPENCSTL_NIDX(container, -6) = (size_t) (uintptr_t) new_meta;
        htm_erase(htm, old_ptr);
        htm_insert(htm, *container,
                   (char *) *container + type_size * new_cap,
                   (char *) new_meta, (int) type_size);
        return;
    }
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

#define cstl_unordered_set _cstl_unordered_set
#define _cstl_unordered_set(KEY,...) _CSTL_USET_DISPATCH(KEY, ##__VA_ARGS__, NULL)
#define _CSTL_USET_DISPATCH(KEY, FUNC, ...) __cstl_unordered_set(sizeof(KEY),#KEY,(void*)(FUNC))
OPENCSTL_FUNC

void *__cstl_unordered_set(size_t key_size, const char *type_key, void *hash_func) {
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    size_t cap = __HASHTABLE_DEFAULT_SIZE__;
    void *ptr = (char *) calloc(header_sz + key_size * cap, 1) + header_sz;
    void **c = &ptr;
    uint16_t *meta = __ht_alloc_meta(cap);
    OPENCSTL_NIDX(c, NIDX_CTYPE) = OPENCSTL_UNORDERED_SET;
    OPENCSTL_NIDX(c, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(c, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(c, -9) = 0;
    OPENCSTL_NIDX(c, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(c, -7) = cap - 1;
    OPENCSTL_NIDX(c, -6) = (size_t) (uintptr_t) meta;
    OPENCSTL_NIDX(c, -4) = 0;
    OPENCSTL_NIDX(c, -3) = (size_t) type_key;
    OPENCSTL_NIDX(c, -2) = (size_t) hash_func;
    OPENCSTL_NIDX(c, -1) = 0;
    OPENCSTL_NIDX(c, 0) = 0;
    bool htm_init = false;
    if (htm == NULL) {
        htm = htm_new();
        htm_init = true;
    }
    htm_insert(htm, ptr, (char *) ptr + (key_size * cap), (char *) meta, (int) key_size);
    //__htm_append(ptr, key_size * cap, (char *) meta, (int) key_size);

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
#define cstl_unordered_map(KEY, VALUE) _cstl_unordered_map(KEY, VALUE)
#define _cstl_unordered_map(KEY,VALUE,...) _CSTL_UMAP_DISPATCH(KEY,VALUE,##__VA_ARGS__,NULL)
#define _CSTL_UMAP_DISPATCH(KEY,VALUE,FUNC,...) __cstl_unordered_map(sizeof(KEY),sizeof(VALUE),#KEY,#VALUE,(void*)(FUNC))
OPENCSTL_FUNC

void *__cstl_unordered_map(size_t key_size, size_t value_size,
                           const char *type_key, const char *type_value,
                           void *hash_func) {
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    size_t type_size = key_size + value_size;
    size_t cap = __HASHTABLE_DEFAULT_SIZE__;
    void *ptr = (char *) calloc(header_sz + type_size * cap, 1) + header_sz;
    void **c = &ptr;
    uint16_t *meta = __ht_alloc_meta(cap);
    OPENCSTL_NIDX(c, NIDX_CTYPE) = OPENCSTL_UNORDERED_MAP;
    OPENCSTL_NIDX(c, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(c, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(c, -9) = !strcmp(type_value, "float");
    OPENCSTL_NIDX(c, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(c, -7) = cap - 1;
    OPENCSTL_NIDX(c, -6) = (size_t) (uintptr_t) meta;
    OPENCSTL_NIDX(c, -4) = value_size;
    OPENCSTL_NIDX(c, -3) = (size_t) type_key;
    OPENCSTL_NIDX(c, -2) = (size_t) hash_func;
    OPENCSTL_NIDX(c, -1) = 0;
    OPENCSTL_NIDX(c, 0) = 0;
    bool htm_init = false;
    if (htm == NULL) {
        htm = htm_new();
        htm_init = true;
    }
    htm_insert(htm, ptr, (char *) ptr + (type_size * cap), (char *) meta, (int) type_size);
    //__htm_append(ptr, type_size * cap, (char *) meta, (int) type_size);
    if (htm_init) {
        atexit(__cstl_htm_destroy);
    }
    return ptr;
}
#endif
