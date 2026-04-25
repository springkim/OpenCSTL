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
#if !defined(OPENCSTL_BITSET_H)
#define OPENCSTL_BITSET_H
#include "types.h"
#include "zalloc.h"
#include "crossplatform.h"

static inline int __popcnt_sw(int v) {
    v = (v & 0x55555555) + ((v >> 1) & 0x55555555);
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);
    v = (v & 0x0f0f0f0f) + ((v >> 4) & 0x0f0f0f0f);
    v = (v & 0x00ff00ff) + ((v >> 8) & 0x00ff00ff);
    v = (v & 0x0000ffff) + ((v >> 16) & 0x0000ffff);
    return v;
}

static inline int __popcnt64_sw(long long v) {
    v = (v & 0x5555555555555555LL) + ((v >> 1) & 0x5555555555555555LL);
    v = (v & 0x3333333333333333LL) + ((v >> 2) & 0x3333333333333333LL);
    v = (v & 0x0f0f0f0f0f0f0f0fLL) + ((v >> 4) & 0x0f0f0f0f0f0f0f0fLL);
    v = (v & 0x00ff00ff00ff00ffLL) + ((v >> 8) & 0x00ff00ff00ff00ffLL);
    v = (v & 0x0000ffff0000ffffLL) + ((v >> 16) & 0x0000ffff0000ffffLL);
    v = (v & 0x00000000ffffffffLL) + ((v >> 32) & 0x00000000ffffffffLL);
    return (int) v;
}
#if defined(OCSTL_OS_LINUX)
#if defined (OCSTL_CC_TCC)
#define __popcnt __popcnt_sw
#define __popcnt64 __popcnt64_sw
#else
#define __popcnt(v)   __builtin_popcount((unsigned int)(v))
#define __popcnt64(v) __builtin_popcountll((unsigned long long)(v))
#endif
#elif defined(OCSTL_OS_WINDOWS)
#if !defined(OCSTL_CC_TCC)

#include<intrin.h>
#else
#define __popcnt __popcnt_sw
#define __popcnt64 __popcnt64_sw
#endif

#elif defined(OCSTL_OS_MACOS)
#if !defined(OCSTL_CC_TCC)
#define __popcnt(v)   __builtin_popcount((unsigned int)(v))
#define __popcnt64(v) __builtin_popcountll((unsigned long long)(v))
#else
#define __popcnt __popcnt_sw
#define __popcnt64 __popcnt64_sw
#endif
#endif
#define __BASE_SIZE 8

static inline size_type __cstl_bitset_capacity(size_type size) {
    size_type cap = (size + 7) / 8;
    return cap < __BASE_SIZE ? __BASE_SIZE : cap;
}

typedef struct {
    size_type nbits;
    ubyte_x *bits;
    char *str;
} BITSET;

#define new_bitset(N) __cstl_bitset(N)

static BITSET __cstl_bitset(size_type n) {
    size_type cap = __cstl_bitset_capacity(n);
    BITSET b;
    b.nbits = n;
    b.bits = (ubyte_x *) calloc(cap, 1);

    b.str = (char *) calloc(b.nbits + 1, 1);

    bool iveb_init = false;
    if (iveb == NULL) {
        iveb = iveb_new();
        iveb_init = true;
    }
    iveb_insert(iveb, b.bits, b.bits + cap, CT_BITSET, 0, "bitset");
    if (iveb_init) {
        atexit(__opencstl_iveb_destroy);
    }
    return b;
}

// ██████╗░██╗████████╗░██████╗███████╗████████╗░░░░░░███████╗██╗░░░██╗███╗░░██╗░█████╗░░░░░░░██████╗░████████╗██████╗░
// ██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝░░░░░░██╔════╝██║░░░██║████╗░██║██╔══██╗░░░░░░██╔══██╗╚══██╔══╝██╔══██╗
// ██████╦╝██║░░░██║░░░╚█████╗░█████╗░░░░░██║░░░░░░░░░█████╗░░██║░░░██║██╔██╗██║██║░░╚═╝░░░░░░██████╔╝░░░██║░░░██████╔╝
// ██╔══██╗██║░░░██║░░░░╚═══██╗██╔══╝░░░░░██║░░░░░░░░░██╔══╝░░██║░░░██║██║╚████║██║░░██╗░░░░░░██╔═══╝░░░░██║░░░██╔══██╗
// ██████╦╝██║░░░██║░░░██████╔╝███████╗░░░██║░░░█████╗██║░░░░░╚██████╔╝██║░╚███║╚█████╔╝█████╗██║░░░░░░░░██║░░░██║░░██║
// ╚═════╝░╚═╝░░░╚═╝░░░╚═════╝░╚══════╝░░░╚═╝░░░╚════╝╚═╝░░░░░░╚═════╝░╚═╝░░╚══╝░╚════╝░╚════╝╚═╝░░░░░░░░╚═╝░░░╚═╝░░╚═╝
typedef void (*bitset_fn1)(BITSET b);

typedef void (*bitset_fn2)(BITSET b, size_type idx, bool val);

typedef size_type (*bitset_fn3)(BITSET b);

typedef bool (*bitset_fn4)(BITSET b);

typedef void (*bitset_fn5)(BITSET b, size_type idx);

typedef bool (*bitset_fn6)(BITSET b, size_type idx);

typedef char * (*bitset_fn7)(BITSET b);

// ██████╗░██╗████████╗░██████╗███████╗████████╗░░░░░░██╗███╗░░░███╗██████╗░██╗░░░░░
// ██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝╚══██╔══╝░░░░░░██║████╗░████║██╔══██╗██║░░░░░
// ██████╦╝██║░░░██║░░░╚█████╗░█████╗░░░░░██║░░░░░░░░░██║██╔████╔██║██████╔╝██║░░░░░
// ██╔══██╗██║░░░██║░░░░╚═══██╗██╔══╝░░░░░██║░░░░░░░░░██║██║╚██╔╝██║██╔═══╝░██║░░░░░
// ██████╦╝██║░░░██║░░░██████╔╝███████╗░░░██║░░░█████╗██║██║░╚═╝░██║██║░░░░░███████╗
// ╚═════╝░╚═╝░░░╚═╝░░░╚═════╝░╚══════╝░░░╚═╝░░░╚════╝╚═╝╚═╝░░░░░╚═╝╚═╝░░░░░╚══════╝
static void __cstl_bitset_set(BITSET b) {
    // 모든 비트를 1로 설정
    size_type cap = __cstl_bitset_capacity(b.nbits);
    memset(b.bits, 0xFF, cap);
    // _size 이후의 여분 비트를 0으로 클리어
    size_type rem = b.nbits % 8;
    if (rem != 0) {
        b.bits[b.nbits / 8] &= (ubyte_x) ((1 << rem) - 1);
    }
}

static void __cstl_bitset_reset(BITSET b) {
    // 모든 비트를 0으로 설정
    size_type cap = __cstl_bitset_capacity(b.nbits);
    memset(b.bits, 0, cap);
}

static void __cstl_bitset_set_at(BITSET b, size_type idx, bool val) {
    // idx번째 비트를 val로 설정
    size_type byte_idx = idx / 8;
    size_type bit_idx = idx % 8;
    if (val) {
        b.bits[byte_idx] |= (ubyte_x) (1 << bit_idx);
    } else {
        b.bits[byte_idx] &= (ubyte_x) ~(1 << bit_idx);
    }
}

static size_type __cstl_bitset_count(BITSET b) {
    // 1로 설정된 비트의 개수를 반환
    size_type cap = __cstl_bitset_capacity(b.nbits);
    size_type64 count = 0;
    size_type i = 0;
    // 8바이트(64비트) 단위로 처리
    for (; i + 8 <= cap; i += 8) {
        long long chunk;
        memcpy(&chunk, b.bits + i, 8);
        count += __popcnt64(chunk);
    }
    // 남은 바이트를 4바이트 단위로 처리
    for (; i + 4 <= cap; i += 4) {
        int chunk;
        memcpy(&chunk, b.bits + i, 4);
        count += __popcnt(chunk);
    }
    // 나머지 바이트 처리
    for (; i < cap; i++) {
        count += __popcnt((int) b.bits[i]);
    }
    return (size_type) count;
}

static bool __cstl_bitset_all(BITSET b) {
    // 모든 비트가 1인지 검사
    if (b.nbits == 0) return true;
    size_type full_bytes = b.nbits / 8;
    size_type rem = b.nbits % 8;
    for (size_type i = 0; i < full_bytes; i++) {
        if (b.bits[i] != 0xFF) return false;
    }
    if (rem != 0) {
        ubyte_x mask = (ubyte_x) ((1 << rem) - 1);
        if ((b.bits[full_bytes] & mask) != mask) return false;
    }
    return true;
}

static bool __cstl_bitset_any(BITSET b) {
    // 하나라도 1인 비트가 있는지 검사
    if (b.nbits == 0) return false;
    size_type full_bytes = b.nbits / 8;
    size_type rem = b.nbits % 8;
    for (size_type i = 0; i < full_bytes; i++) {
        if (b.bits[i] != 0) return true;
    }
    if (rem != 0) {
        ubyte_x mask = (ubyte_x) ((1 << rem) - 1);
        if ((b.bits[full_bytes] & mask) != 0) return true;
    }
    return false;
}

static bool __cstl_bitset_none(BITSET b) {
    // 모든 비트가 0인지 검사
    return !__cstl_bitset_any(b);
}

static void __cstl_bitset_flip(BITSET b) {
    // 모든 비트를 반전
    size_type full_bytes = b.nbits / 8;
    size_type rem = b.nbits % 8;
    for (size_type i = 0; i < full_bytes; i++) {
        b.bits[i] = ~b.bits[i];
    }
    if (rem != 0) {
        ubyte_x mask = (ubyte_x) ((1 << rem) - 1);
        b.bits[full_bytes] ^= mask;
    }
}

static void __cstl_bitset_flip_at(BITSET b, size_type idx) {
    // 특정 위치의 비트를 반전
    size_type byte_idx = idx / 8;
    size_type bit_idx = idx % 8;
    b.bits[byte_idx] ^= (ubyte_x) (1 << bit_idx);
}


static size_type __cstl_bitset_nbits(BITSET b) {
    // 비트셋의 크기(비트 수)를 반환
    return b.nbits;
}

static bool __cstl_bitset_test(BITSET b, size_type idx) {
    // 특정 위치의 비트가 1인지 검사
    size_type byte_idx = idx / 8;
    size_type bit_idx = idx % 8;
    return (b.bits[byte_idx] >> bit_idx) & 1;
}


static char *__cstl_bitset_to_string(BITSET b) {
    // 비트셋을 문자열로 변환 (MSB가 앞, C++ bitset::to_string과 동일)
    for (size_type i = 0; i < b.nbits; i++) {
        size_type bit_pos = b.nbits - 1 - i;
        b.str[i] = __cstl_bitset_test(b, bit_pos) ? '1' : '0';
    }
    b.str[b.nbits] = '\0';
    return b.str;
}

static void __cstl_bitset_free(BITSET b) {
    iveb_erase(iveb, b.bits);
    free(b.bits);
    free(b.str);
}

typedef struct {
    bitset_fn1 set;
    bitset_fn1 reset;
    bitset_fn2 set_at;
    bitset_fn3 count;
    bitset_fn4 all;
    bitset_fn4 any;
    bitset_fn4 none;
    bitset_fn1 flip;
    bitset_fn5 flip_at;
    bitset_fn3 nbits;
    bitset_fn6 test;
    bitset_fn7 to_string;
} __BITSET;

static __BITSET bitset = {
    __cstl_bitset_set,
    __cstl_bitset_reset,
    __cstl_bitset_set_at,
    __cstl_bitset_count,
    __cstl_bitset_all,
    __cstl_bitset_any,
    __cstl_bitset_none,
    __cstl_bitset_flip,
    __cstl_bitset_flip_at,
    __cstl_bitset_nbits,
    __cstl_bitset_test,
    __cstl_bitset_to_string,
};

#endif //OPENCSTL_BITSET_H
