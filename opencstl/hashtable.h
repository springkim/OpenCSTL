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
#if !defined(_OPENCSTL_HASHTABLE_H)
#define _OPENCSTL_HASHTABLE_H
#include"types.h"
#include"error.h"
#include"defines.h"

static bool is_prime(const unsigned int x) {
    if (x == 2 || x == 3 || x == 5 || x == 7 || x == 11) return true;
    if (x % 2 == 0 || x % 3 == 0 || x % 5 == 0 || x % 7 == 0 || x % 11 == 0) return false;
    if (x < 121) return (x > 1);

    static const unsigned int bases[] = {
        15591, 2018, 166, 7429, 8064, 16045, 10503, 4399, 1949, 1295, 2776, 3620, 560, 3128, 5212, 2657,
        2300, 2021, 4652, 1471, 9336, 4018, 2398, 20462, 10277, 8028, 2213, 6219, 620, 3763, 4852, 5012,
        3185, 1333, 6227, 5298, 1074, 2391, 5113, 7061, 803, 1269, 3875, 422, 751, 580, 4729, 10239,
        746, 2951, 556, 2206, 3778, 481, 1522, 3476, 481, 2487, 3266, 5633, 488, 3373, 6441, 3344,
        17, 15105, 1490, 4154, 2036, 1882, 1813, 467, 3307, 14042, 6371, 658, 1005, 903, 737, 1887,
        7447, 1888, 2848, 1784, 7559, 3400, 951, 13969, 4304, 177, 41, 19875, 3110, 13221, 8726, 571,
        7043, 6943, 1199, 352, 6435, 165, 1169, 3315, 978, 233, 3003, 2562, 2994, 10587, 10030, 2377,
        1902, 5354, 4447, 1555, 263, 27027, 2283, 305, 669, 1912, 601, 6186, 429, 1930, 14873, 1784,
        1661, 524, 3577, 236, 2360, 6146, 2850, 55637, 1753, 4178, 8466, 222, 2579, 2743, 2031, 2226,
        2276, 374, 2132, 813, 23788, 1610, 4422, 5159, 1725, 3597, 3366, 14336, 579, 165, 1375, 10018,
        12616, 9816, 1371, 536, 1867, 10864, 857, 2206, 5788, 434, 8085, 17618, 727, 3639, 1595, 4944,
        2129, 2029, 8195, 8344, 6232, 9183, 8126, 1870, 3296, 7455, 8947, 25017, 541, 19115, 368, 566,
        5674, 411, 522, 1027, 8215, 2050, 6544, 10049, 614, 774, 2333, 3007, 35201, 4706, 1152, 1785,
        1028, 1540, 3743, 493, 4474, 2521, 26845, 8354, 864, 18915, 5465, 2447, 42, 4511, 1660, 166,
        1249, 6259, 2553, 304, 272, 7286, 73, 6554, 899, 2816, 5197, 13330, 7054, 2818, 3199, 811,
        922, 350, 7514, 4452, 3449, 2663, 4708, 418, 1621, 1171, 3471, 88, 11345, 412, 1559, 194,
    };

    unsigned long long h = x;
    h = ((h >> 16) ^ h) * 73244475;
    h = ((h >> 16) ^ h) * 73244475;
    h = ((h >> 16) ^ h) & 255;

    unsigned int a = bases[h];

    unsigned int d = x - 1, s = 0;
    while ((d & 1) == 0) {
        ++s;
        d >>= 1;
    }

    unsigned long long cur = 1, pw = d;
    unsigned long long base = a % x;
    while (pw) {
        if (pw & 1) cur = (cur * base) % x;
        base = (base * base) % x;
        pw >>= 1;
    }

    if (cur == 1 || cur == (unsigned long long) x - 1) return true;

    for (unsigned int r = 1; r < s; r++) {
        cur = (cur * cur) % x;
        if (cur == (unsigned long long) x - 1) return true;
    }

    return false;
}

#define VACANT   0  // Empty
#define OCCUPIED 1  // Inserted
#define TOMBSTONE 2 // Deleted
#define LN2 0.69314718055994530941723212145817 //log(2)


// MurmurHash3 mixer
OPENCSTL_FUNC size_t hash32(void *_key) {
    unsigned int h = *(unsigned int *) _key;
    h ^= h >> 16;
    h *= 0x85ebca6b;
    h ^= h >> 13;
    h *= 0xc2b2ae35;
    h ^= h >> 16;
    return (size_t) h;
}

// SplitMix64 mixer
OPENCSTL_FUNC size_t hash64(void *_key) {
    unsigned long long x = *(unsigned long long *) _key;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9ULL;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebULL;
    x = x ^ (x >> 31);
    return (size_t) x;
}

// Hash for General-Purpose Data Blocks
OPENCSTL_FUNC size_t hash(void *_key, size_t n) {
    unsigned char *key = (unsigned char *) _key;
    size_t ret = 0xCBF29CE484222325ULL; // FNV offset basis

    // Process in 4-byte or 8-byte chunks
    while (n >= sizeof(size_t)) {
        ret ^= (sizeof(size_t) == 8) ? hash64(key) : hash32(key);
        ret *= 0x100000001b3ULL; // Mixing via prime factorization
        key += sizeof(size_t);
        n -= sizeof(size_t);
    }
    return ret;
}

OPENCSTL_FUNC size_t hash_mixer(void *key, size_t n) {
    unsigned char *data = (unsigned char *) key;

    size_t h = 0x9e3779b97f4a7c15ULL; // seed (golden ratio)

    // Processed in size_t units (8 or 4 bytes)
    while (n >= sizeof(size_t)) {
        size_t k;
        memcpy(&k, data, sizeof(size_t)); // unaligned-safe

        // mix (SplitMix64 스타일)
        k ^= k >> 30;
        k *= 0xbf58476d1ce4e5b9ULL;
        k ^= k >> 27;
        k *= 0x94d049bb133111ebULL;
        k ^= k >> 31;

        h ^= k;
        h *= 0x9e3779b97f4a7c15ULL;

        data += sizeof(size_t);
        n -= sizeof(size_t);
    }

    // tail
    size_t _tail = 0;
    for (size_t i = 0; i < n; i++) {
        _tail |= ((size_t) data[i]) << (i * 8);
    }

    if (n > 0) {
        _tail ^= _tail >> 30;
        _tail *= 0xbf58476d1ce4e5b9ULL;
        _tail ^= _tail >> 27;
        _tail *= 0x94d049bb133111ebULL;
        _tail ^= _tail >> 31;

        h ^= _tail;
        h *= 0x9e3779b97f4a7c15ULL;
    }

    // final avalanche
    h ^= h >> 30;
    h *= 0xbf58476d1ce4e5b9ULL;
    h ^= h >> 27;
    h *= 0x94d049bb133111ebULL;
    h ^= h >> 31;

    return h;
}


OPENCSTL_FUNC size_t H1(void *key, size_t _capacity, size_t key_size) {
    if (_capacity == 0) return 0;
    return hash_mixer(key, key_size) % _capacity;
}

// Determine the jump interval in case of a collision (for double hashing)
// This value must be independent of h1 and must be either an odd number or coprime with _capacity.
OPENCSTL_FUNC size_t H2(void *key, size_t _capacity, size_t key_size) {
    if (_capacity <= 1) return 1;
    size_t h = hash_mixer(key, key_size);
    size_t step = (h % (_capacity - 1)) + 1;
    return step;
}

// ██╗░░██╗░█████╗░░██████╗██╗░░██╗░░░░░░████████╗░█████╗░██████╗░██╗░░░░░███████╗░░░░░░███╗░░░███╗░█████╗░███╗░░██╗░█████╗░░██████╗░███████╗██████╗░
// ██║░░██║██╔══██╗██╔════╝██║░░██║░░░░░░╚══██╔══╝██╔══██╗██╔══██╗██║░░░░░██╔════╝░░░░░░████╗░████║██╔══██╗████╗░██║██╔══██╗██╔════╝░██╔════╝██╔══██╗
// ███████║███████║╚█████╗░███████║█████╗░░░██║░░░███████║██████╦╝██║░░░░░█████╗░░█████╗██╔████╔██║███████║██╔██╗██║███████║██║░░██╗░█████╗░░██████╔╝
// ██╔══██║██╔══██║░╚═══██╗██╔══██║╚════╝░░░██║░░░██╔══██║██╔══██╗██║░░░░░██╔══╝░░╚════╝██║╚██╔╝██║██╔══██║██║╚████║██╔══██║██║░░╚██╗██╔══╝░░██╔══██╗
// ██║░░██║██║░░██║██████╔╝██║░░██║░░░░░░░░░██║░░░██║░░██║██████╦╝███████╗███████╗░░░░░░██║░╚═╝░██║██║░░██║██║░╚███║██║░░██║╚██████╔╝███████╗██║░░██║
// ╚═╝░░╚═╝╚═╝░░╚═╝╚═════╝░╚═╝░░╚═╝░░░░░░░░░╚═╝░░░╚═╝░░╚═╝╚═════╝░╚══════╝╚══════╝░░░░░░╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝╚═╝░░╚═╝░╚═════╝░╚══════╝╚═╝░░╚═╝

#define HTM_SIZE 1024
typedef struct HashtableManager HashtableManager;

struct HashtableManager {
    void *p1;
    void *p2;
    char *tombstone;
    int type_size;
    bool used;
};

static HashtableManager htm[1024] = {0};
static size_t htm_length = 0;

void __htm_append(void *ptr, size_t sz, char *tombstone, int type_size) {
    htm[htm_length].p1 = ptr;
    htm[htm_length].p2 = (char *) ptr + sz;
    htm[htm_length].tombstone = tombstone;
    htm[htm_length].used = true;
    htm[htm_length].type_size = type_size;
    htm_length++;
}

#define __HASHTABLE_DEFAULT_SIZE__ 7

// ██╗░░░██╗███╗░░██╗░█████╗░██████╗░██████╗░███████╗██████╗░███████╗██████╗░░░░░░░░██████╗███████╗████████╗
// ██║░░░██║████╗░██║██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔════╝██╔══██╗░░░░░░██╔════╝██╔════╝╚══██╔══╝
// ██║░░░██║██╔██╗██║██║░░██║██████╔╝██║░░██║█████╗░░██████╔╝█████╗░░██║░░██║░░░░░░╚█████╗░█████╗░░░░░██║░░░
// ██║░░░██║██║╚████║██║░░██║██╔══██╗██║░░██║██╔══╝░░██╔══██╗██╔══╝░░██║░░██║░░░░░░░╚═══██╗██╔══╝░░░░░██║░░░
// ╚██████╔╝██║░╚███║╚█████╔╝██║░░██║██████╔╝███████╗██║░░██║███████╗██████╔╝█████╗██████╔╝███████╗░░░██║░░░
// ░╚═════╝░╚═╝░░╚══╝░╚════╝░╚═╝░░╚═╝╚═════╝░╚══════╝╚═╝░░╚═╝╚══════╝╚═════╝░╚════╝╚═════╝░╚══════╝░░░╚═╝░░░

#define cstl_unordered_set(KEY) _cstl_unordered_set(KEY,NULL)
#define _cstl_unordered_set(KEY,...) __cstl_unordered_set(sizeof(KEY),#KEY,ARGN(__VA_ARGS__),__VA_ARGS__)
OPENCSTL_FUNC void *__cstl_unordered_set(size_t key_size, char *type_key, int argc, ...) {
    va_list vl;
    va_start(vl, argc);
    void *hash_func = va_arg(vl, void*);
    if (argc == 0) {
        hash_func = NULL; //use default hash function
    }
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    void *ptr = (char *) calloc(header_sz + (key_size * __HASHTABLE_DEFAULT_SIZE__),
                                sizeof(char)) + header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_UNORDERED_SET;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(container, -9) = 0;
    OPENCSTL_NIDX(container, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(container, -7) = __HASHTABLE_DEFAULT_SIZE__; // _capacity
    OPENCSTL_NIDX(container, -6) = (size_t) calloc(__HASHTABLE_DEFAULT_SIZE__, sizeof(char)); // tombstone
    OPENCSTL_NIDX(container, -4) = 0; //value_size, but set does not have value_size.
    OPENCSTL_NIDX(container, -3) = (size_t) type_key; //type
    OPENCSTL_NIDX(container, -2) = (size_t) hash_func; //hash function
    OPENCSTL_NIDX(container, -1) = 0; // length
    OPENCSTL_NIDX(container, 0) = 0; // base
    va_end(vl);

    __htm_append(ptr, key_size * __HASHTABLE_DEFAULT_SIZE__, (char *) OPENCSTL_NIDX(container, -6), key_size);
    return ptr;
}

#define cstl_unordered_map(KEY, VALUE) _cstl_unordered_map(KEY,VALUE)

// ██╗░░░██╗███╗░░██╗░█████╗░██████╗░██████╗░███████╗██████╗░███████╗██████╗░░░░░░░███╗░░░███╗░█████╗░██████╗░
// ██║░░░██║████╗░██║██╔══██╗██╔══██╗██╔══██╗██╔════╝██╔══██╗██╔════╝██╔══██╗░░░░░░████╗░████║██╔══██╗██╔══██╗
// ██║░░░██║██╔██╗██║██║░░██║██████╔╝██║░░██║█████╗░░██████╔╝█████╗░░██║░░██║░░░░░░██╔████╔██║███████║██████╔╝
// ██║░░░██║██║╚████║██║░░██║██╔══██╗██║░░██║██╔══╝░░██╔══██╗██╔══╝░░██║░░██║░░░░░░██║╚██╔╝██║██╔══██║██╔═══╝░
// ╚██████╔╝██║░╚███║╚█████╔╝██║░░██║██████╔╝███████╗██║░░██║███████╗██████╔╝█████╗██║░╚═╝░██║██║░░██║██║░░░░░
// ░╚═════╝░╚═╝░░╚══╝░╚════╝░╚═╝░░╚═╝╚═════╝░╚══════╝╚═╝░░╚═╝╚══════╝╚═════╝░╚════╝╚═╝░░░░░╚═╝╚═╝░░╚═╝╚═╝░░░░░


#define _cstl_unordered_map(KEY,VALUE,...) __cstl_unordered_map(sizeof(KEY),sizeof(VALUE),#KEY,#VALUE,ARGN(__VA_ARGS__),__VA_ARGS__)
OPENCSTL_FUNC void *__cstl_unordered_map(size_t key_size, size_t value_size, char *type_key, char *type_value, int argc,
                                         ...) {
    va_list vl;
    va_start(vl, argc);
    void *hash_func = va_arg(vl, void*);
    if (argc == 0) {
        hash_func = NULL; //use default hash function
    }
    size_t header_sz = sizeof(size_t) * OPENCSTL_HEADER;
    void *ptr = (char *) calloc(header_sz + ((key_size + value_size) * __HASHTABLE_DEFAULT_SIZE__), sizeof(char)) +
                header_sz;
    void **container = &ptr;
    OPENCSTL_NIDX(container, NIDX_CTYPE) = OPENCSTL_UNORDERED_MAP;
    OPENCSTL_NIDX(container, NIDX_HSIZE) = header_sz;
    OPENCSTL_NIDX(container, NIDX_TSIZE) = key_size;
    OPENCSTL_NIDX(container, -9) = !strcmp(type_value, "float");
    OPENCSTL_NIDX(container, -8) = !strcmp(type_key, "float");
    OPENCSTL_NIDX(container, -7) = __HASHTABLE_DEFAULT_SIZE__; // _capacity
    OPENCSTL_NIDX(container, -6) = (size_t) calloc(__HASHTABLE_DEFAULT_SIZE__, sizeof(char)); // tombstone
    OPENCSTL_NIDX(container, -4) = value_size; //value_size, but set does not have value_size.
    OPENCSTL_NIDX(container, -3) = (size_t) type_key; //type
    OPENCSTL_NIDX(container, -2) = (size_t) hash_func; //hash function
    OPENCSTL_NIDX(container, -1) = 0; // length
    OPENCSTL_NIDX(container, 0) = 0; // base
    va_end(vl);
    __htm_append(ptr, ((key_size + value_size) * __HASHTABLE_DEFAULT_SIZE__), (char *) OPENCSTL_NIDX(container, -6),
                 (key_size + value_size));
    return ptr;
}

OPENCSTL_FUNC bool ___cstl_hashtable_insert(void *base, char *c, void *key, void *value, size_t key_size,
                                            size_t value_size, size_t new__capacity) {
    size_t h1 = H1(key, new__capacity, key_size);
    size_t h2 = H2(key, new__capacity, key_size);
    size_t pos = h1;
    size_t type_size = key_size + value_size;
    while (c[pos] == OCCUPIED) {
        if (memcmp(((char *) base) + (pos * type_size), key, key_size) == 0) {
            return false;
        }
        pos = (pos + h2) % new__capacity;
    }
    c[pos] = OCCUPIED;
    memcpy(((char *) base) + (pos * type_size), key, key_size);
    if (value) {
        memcpy(((char *) base) + (pos * type_size) + key_size, value, value_size);
    }
    return true;
}


OPENCSTL_FUNC void __cstl_hashtable_insert(void **container, void *key, void *value) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    size_t type_size = key_size + value_size;
    size_t length = OPENCSTL_NIDX(container, -1);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);

    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t is_float_value = OPENCSTL_NIDX(container, -9);

#if !defined(__linux__) && !defined(__APPLE__)
    float keyf = 0.0F;
    if (is_float_key) {
        keyf = (float) *(double *) key;
        key = &keyf;
    }

    float valuef = 0.0F;
    if (is_float_value) {
        valuef = (float) *(double *) value;
        value = &valuef;
    }
#endif

    if (length > _capacity * LN2) {
        void *ptr = *container;
        int htm_index = -1;
        for (int i = 0; i < (int) htm_length; i++) {
            if (htm[i].p1 == ptr) {
                htm_index = i;
                break;
            }
        }
        if (htm_index == -1) {
            cstl_error("Unregistered hashtable");
        }
        size_t new__capacity = _capacity * 2;
        while (!is_prime(new__capacity))new__capacity++;
        void *new_base_raw = calloc(header_sz + new__capacity * type_size, sizeof(char));
        if (new_base_raw == NULL) {
            cstl_error("Allocation failed at hashtable insert");
        }
        // Copy header
        memcpy(new_base_raw, (char *) *container - header_sz, header_sz);

        char *new_tombstone = calloc(new__capacity, sizeof(char));
        if (new_tombstone == NULL) {
            cstl_error("Allocation failed at hashtable insert");
        }

        void *old_base = *container;
        char *old_tombstone = tombstone;

        void *nb = (char *) new_base_raw + header_sz;

        for (int i = 0; i < (int) _capacity; i++) {
            if (old_tombstone[i] == OCCUPIED) {
                void *old_key = (char *) old_base + (i * type_size);
                void *old_val = value_size > 0 ? (char *) old_key + key_size : NULL;
                ___cstl_hashtable_insert(nb, new_tombstone, old_key, old_val, key_size, value_size,
                                         new__capacity);
            }
        }

        free((char *) old_base - header_sz);
        free(old_tombstone);

        *container = nb;
        OPENCSTL_NIDX(container, -6) = (size_t) new_tombstone; // tombstone
        OPENCSTL_NIDX(container, -7) = new__capacity;
        tombstone = new_tombstone;
        _capacity = new__capacity;


        htm[htm_index].p1 = *container;
        htm[htm_index].p2 = ((char *) *container) + (type_size * new__capacity);
        htm[htm_index].tombstone = tombstone;
        htm[htm_index].type_size = type_size;
    }
    if (___cstl_hashtable_insert(*container, tombstone, key, value, key_size, value_size, _capacity)) {
        OPENCSTL_NIDX(container, -1)++;
    }
}

OPENCSTL_FUNC void __cstl_hashtable_erase(void **container, void *key) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;


#if !defined(__linux__) && !defined(__APPLE__)
    float keyf = 0.0F;
    if (is_float_key) {
        keyf = (float) *(double *) key;
        key = &keyf;
    }
#endif
    size_t h1 = H1(key, _capacity, key_size);
    size_t h2 = H2(key, _capacity, key_size);
    size_t pos = h1;
    while (tombstone[pos] != VACANT && memcmp(((char *) *container) + (pos * type_size), key, key_size) != 0) {
        pos = (pos + h2) % _capacity;
    }
    if (memcmp(((char *) *container) + (pos * type_size), key, key_size) == 0) {
        return;
    }
    size_t sw = pos;
    size_t mv = pos;
    while (tombstone[mv] != VACANT) {
        if (tombstone[mv] != TOMBSTONE && H1(((char *) *container) + (pos * type_size), _capacity, key_size) == h1) {
            sw = mv;
        }
        mv = (mv + h2) % _capacity;
    }
    memcpy(((char *) *container) + (pos * type_size), ((char *) *container) + (sw * type_size), type_size);
    tombstone[pos] = tombstone[sw];
    tombstone[sw] = TOMBSTONE;
    OPENCSTL_NIDX(container, -1)--;
}

OPENCSTL_FUNC void *__cstl_hashtable_find(void **container, void *key) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;

#if !defined(__linux__) && !defined(__APPLE__)
    float keyf = 0.0F;
    if (is_float_key) {
        keyf = (float) *(double *) key;
        key = &keyf;
    }
#endif

    size_t h1 = H1(key, _capacity, key_size);
    size_t h2 = H2(key, _capacity, key_size);
    size_t pos = h1;
    while (tombstone[pos] != VACANT) {
        if (tombstone[pos] != OCCUPIED && memcmp(((char *) *container) + (pos * type_size), key, key_size) == 0) {
            return ((char *) *container) + (pos * type_size);
        }
        pos = (pos + h2) % _capacity;
    }
    return NULL;
}

OPENCSTL_FUNC void __cstl_hashtable_clear(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;

    memset(tombstone, 0, _capacity * sizeof(char));
    OPENCSTL_NIDX(container, -1) = 0;
}

OPENCSTL_FUNC void *__cstl_hashtable_begin(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;

    for (int i = 0; i < _capacity; i++) {
        if (tombstone[i] == OCCUPIED) {
            return ((char *) *container) + (i * type_size);
        }
    }
    return NULL;
}

OPENCSTL_FUNC void *__cstl_hashtable_end(void **container) {
    return NULL;
}

OPENCSTL_FUNC void *__cstl_hashtable_rbegin(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;

    for (int i = _capacity - 1; i >= 0; i--) {
        if (tombstone[i] == OCCUPIED) {
            return ((char *) *container) + (i * type_size);
        }
    }
    return (void *) ((char *) *container + (type_size * (length - 1)));
}

OPENCSTL_FUNC void *__cstl_hashtable_rend(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;
    return (void *) ((char *) *container - (type_size));
}

OPENCSTL_FUNC void *__cstl_hashtable_empty(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;
    return (void *) ((char *) *container - (type_size));
}

OPENCSTL_FUNC size_t __cstl_hashtable_size(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;
    return length;
}

OPENCSTL_FUNC size_t __cstl_hashtable_capacity(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;
    return _capacity;
}


OPENCSTL_FUNC void *__cstl_hashtable_next_prev(void *it, int n) {
    // next(-1), prev(-2)
    int htm_index = -1;
    for (int i = 0; i < htm_length; i++) {
        if (htm[i].p1 <= it && it < htm[i].p2) {
            htm_index = i;
            break;
        }
    }
    if (htm_index == -1) {
        cstl_error("Unregistered hashtable");
    }
    if (n == -1) {
        size_t pos = ((char *) it - (char *) htm[htm_index].p1) / htm[htm_index].type_size;
        void *iter = (char *) it + htm[htm_index].type_size;
        pos++;
        size_t _capacity = ((char *) htm[htm_index].p2 - (char *) htm[htm_index].p1) / htm[htm_index].type_size;
        while (pos < _capacity) {
            if (htm[htm_index].tombstone[pos] == OCCUPIED) {
                return iter;
            }
            pos++;
            iter = (char *) iter + htm[htm_index].type_size;
        }
        return NULL;
    }
    if (n == -2) {
        size_t pos = ((char *) it - (char *) htm[htm_index].p1) / htm[htm_index].type_size;
        void *iter = (char *) it - htm[htm_index].type_size;
        if (pos == 0) return (char *) htm[htm_index].p1 - htm[htm_index].type_size;
        pos--;
        while (iter != (char *) htm[htm_index].p1 - htm[htm_index].type_size) {
            if (htm[htm_index].tombstone[pos] == OCCUPIED) {
                return iter;
            }
            if (pos == 0) break;
            pos--;
            iter = (char *) iter - htm[htm_index].type_size;
        }
        return (char *) htm[htm_index].p1 - htm[htm_index].type_size;
    }
    return NULL;
}

OPENCSTL_FUNC void __cstl_hashtable_free(void **container) {
    size_t container_type = OPENCSTL_NIDX(container, NIDX_CTYPE);
    size_t header_sz = OPENCSTL_NIDX(container, NIDX_HSIZE);
    size_t key_size = OPENCSTL_NIDX(container, NIDX_TSIZE);

    size_t is_float_value = OPENCSTL_NIDX(container, -9);
    size_t is_float_key = OPENCSTL_NIDX(container, -8);
    size_t _capacity = OPENCSTL_NIDX(container, -7);
    char *tombstone = (char *) OPENCSTL_NIDX(container, -6);
    char *type_value = (char *) OPENCSTL_NIDX(container, -5);
    size_t value_size = OPENCSTL_NIDX(container, -4);
    char *type_key = (char *) OPENCSTL_NIDX(container, -3);
    cstl_hash hash_func = (cstl_hash) OPENCSTL_NIDX(container, -2);
    size_t length = OPENCSTL_NIDX(container, -1);

    size_t type_size = key_size + value_size;

    int free_idx = -1;
    for (int i = 0; i < htm_length; i++) {
        if (htm[i].p1 == *container) {
            free_idx = i;
            break;
        }
    }
    if (free_idx != -1) {
        memmove(&htm[free_idx], &htm[free_idx + 1], (htm_length - free_idx) * sizeof(HashtableManager));
        htm_length--;
        free(tombstone);
        free((char *) (*container) - header_sz);
        *container = NULL;
    }
}
#undef VACANT
#undef OCCUPIED
#undef TOMBSTONE
#endif //_OPENCSTL_HASHTABLE_H
