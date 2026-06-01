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
#if !defined(OPENCSTL_MT19937_H)
#define OPENCSTL_MT19937_H
#include <stdint.h>
#include <time.h>
#include "defines.h"
#include "deque.h"
#include "list.h"
#include "swap.h"
#define MT64_N          312
#define MT64_M          156
#define MT64_MATRIX_A   0xB5026F5AA96619E9ULL
#define MT64_UPPER_MASK 0xFFFFFFFF80000000ULL
#define MT64_LOWER_MASK 0x7FFFFFFFULL

// Per-thread storage qualifier. Each thread gets its own RNG state, so
// mt19937.{seed,random,randint,uuid,shuffle} are safe to call concurrently
// from multiple threads without locking. Each thread must seed itself
// (e.g. mt19937.seed(random_device())) if it wants a non-default sequence.
#if defined(__TINYC__)
// TCC accepts the keyword but does not emit working TLS sections on most
// targets (notably Mach-O / macOS), which would crash at runtime on every
// access. Fall back to a plain global — mt19937 is NOT thread-safe under TCC.
#  define OCSTL_THREAD_LOCAL
#  warning "mt19937: TCC lacks working TLS support; mt19937 will not be thread-safe"
#elif defined(_MSC_VER)
#  define OCSTL_THREAD_LOCAL __declspec(thread)
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L && !defined(__STDC_NO_THREADS__)
#  define OCSTL_THREAD_LOCAL _Thread_local
#elif defined(__GNUC__) || defined(__clang__)
#  define OCSTL_THREAD_LOCAL __thread
#else
#  define OCSTL_THREAD_LOCAL
#  warning "mt19937: no thread-local storage support detected; not thread-safe"
#endif

uint64_t random_device(void);

typedef struct {
    uint64_t mt[MT64_N];
    int index;
} __mt19937_64_t;

OCSTL_THREAD_LOCAL __mt19937_64_t __rng64 = {
    {
        0x69DD1B46, 0xF423854F, 0x945B9FE5, 0x94B6B644, 0x5AB34525, 0xCEC40359, 0x41803CD8, 0xAB71D9FF,
        0xE6FE53AE, 0x89530945, 0xCF85DA85, 0xD9EDE399, 0xCFF1E74B, 0x71EADD3C, 0xB720A5F4, 0x0340364D,
        0x7D35BCF3, 0xC89DC3C8, 0xDB43A23A, 0xBC204A45, 0x1943C862, 0x16F2567C, 0x00D7B63C, 0x56D44BD0,
        0x0E288202, 0xD8C7D873, 0x78B09BCA, 0x78E6989D, 0x623535E2, 0xD10A17AA, 0xB1DFFFD3, 0xA1832609,
        0xF693A788, 0x7A38EB09, 0x30D7C7B7, 0xE6EF6521, 0x500AA51E, 0xA8D46798, 0x78329DDE, 0x076CE22D,
        0x52891511, 0xD2712326, 0x28068705, 0xA49FB639, 0x26A64F31, 0x3C9FB9F7, 0x76183999, 0xB8E20714,
        0xAFE22AB4, 0x68B4CDD5, 0x22DDD9A3, 0xF3409FAD, 0x69D963CA, 0xCCDF3FE4, 0x3CCF55A4, 0x14626A0B,
        0xCB241ACD, 0xAD20679C, 0x05A5182D, 0x53349324, 0xA25BB7EA, 0xF4636C05, 0xEF8EF74C, 0x6DCCAA22,
        0xF32AC63A, 0xDB331DA0, 0xEE64148F, 0xAC700F39, 0xFC027376, 0xBF63D603, 0x52E19CA0, 0x277BE867,
        0x4105F363, 0x6E68E5B0, 0x47932DC1, 0x1F11C892, 0xAA302F23, 0x0C0EA81A, 0x3FAFF20D, 0x85287EC5,
        0xC3AB03F1, 0x134940AE, 0xBBA731BB, 0x17510405, 0xB6502E8F, 0xC741221E, 0xCD40626F, 0xAA966080,
        0xA68EF45F, 0x087C17B2, 0xB02B78A4, 0xBC87105C, 0xE028020F, 0xE6C8CE00, 0x6213B58B, 0x5942E074,
        0xD75F7E4B, 0x1B56EB09, 0xB1B34724, 0x2A82DB3E, 0xCDAF4DF0, 0x5555C1EF, 0x5F63A869, 0x662DB1DC,
        0x5C10E59B, 0x381741A8, 0x7C70E2F2, 0x361CF49B, 0xF611657E, 0xA953D8C0, 0x69C4D7B5, 0x86F5B640,
        0x4B05C80A, 0x74132033, 0x796F773C, 0xC06B3786, 0x68F7BC2F, 0x88ACE68B, 0xF314FCB8, 0xC0E83256,
        0xA1420469, 0x89525EC1, 0x72D16767, 0xEE4DC90F, 0xA79049F2, 0x377B8D34, 0x44339CFC, 0xC69F9CCB,
        0x05D346D5, 0xE56B1D4C, 0xADC3D938, 0xA78874E2, 0x4482AA3E, 0x5B82AF6B, 0xC845EB55, 0xCBED0A4B,
        0xCCAC8730, 0x66151180, 0x94E2940A, 0x01AC7B7A, 0x596A3CA4, 0x4CF682E8, 0x7C7919B0, 0x2877D47F,
        0xF1665BE3, 0x2FF246F1, 0xC1A2871C, 0xEA7A23AC, 0xEEDC18FD, 0x87C168E1, 0x40270D7D, 0x9C4F6290,
        0xDF6A416F, 0x6C9393C2, 0x30BDB6E1, 0x8532C382, 0x48295BA3, 0xA1047A17, 0x83FD6222, 0x1D031E99,
        0x2D5CC7B2, 0x79CF6A91, 0x61B8AB1F, 0x97657616, 0x3538AC82, 0xC4CA50AC, 0x92DC813C, 0x8960FB60,
        0xAACBCEE2, 0xEF94FB90, 0xC9F7A754, 0x962F93F6, 0x8DC70E90, 0xE606FFFD, 0x027A8327, 0xD3B6E85D,
        0x1B237B36, 0x6A5FF25C, 0x9165BC65, 0x434C37CE, 0x46FB01EA, 0x48AEEDAA, 0x5A991D98, 0x88A61B9C,
        0x1B46BE51, 0x01942323, 0x02AC8C87, 0xF09EAD76, 0x68750A20, 0x1DCE278A, 0xE5016BA6, 0xE54546ED,
        0x4F4E0B69, 0x11A91790, 0x59FA1299, 0xD2A6AD7B, 0x33F38463, 0xC8EF64D2, 0xCEBD6883, 0xB9CDDDA1,
        0x2E02D36F, 0x3468BD1F, 0x7346A03D, 0x45946CDE, 0xD5B9C6A5, 0xA1D245FB, 0x6899D493, 0x2B4DCE1F,
        0x61F99D43, 0xE0B8E43E, 0xC66660E5, 0xA484A26E, 0x9D55A0FD, 0xA5965121, 0x654B2376, 0x274FC83B,
        0x8BB27837, 0x7E44EDFD, 0x046F5953, 0x0D06E318, 0x01D6D414, 0x459D3461, 0xCEA8D418, 0xF372B044,
        0x782EB8D4, 0x6A76A87F, 0x5A5DA035, 0x4FC87534, 0x1FA56708, 0xDB9892D4, 0x95CD79B1, 0x57ECB4D7,
        0xF77573B3, 0x5979A6D9, 0x4FDF7CE2, 0xB0C712A5, 0x44722147, 0x18E51368, 0x7D028163, 0x9DAA5D29,
        0x537138F8, 0x9E820C89, 0x58D12C07, 0x15AF372E, 0xCE880537, 0xD8ECB573, 0x4F8EF32D, 0x7F5F93B3,
        0x038146E8, 0x7E198E1B, 0x451EE18C, 0xE7F918F1, 0xB7037186, 0x087FEEB8, 0xD397BE83, 0x3F6E7B06,
        0x57F19B0E, 0x6BF4B54A, 0xDC059504, 0xA9D1ADE4, 0xC4D3AE18, 0xF64C00C4, 0x3AFC5DD4, 0x030D28D2,
        0x2ACA5BF2, 0xF873B966, 0x9244B125, 0x79A87DE6, 0xEEC63E20, 0xE00B4A34, 0xE8A5575F, 0xF9B37DC2,
        0x20AF5BD0, 0x3A04D3CE, 0x6DA76FEE, 0x71113F16, 0x618C82C5, 0x733BB8B6, 0xB8A54241, 0xE5EAE3DE,
        0x798B311E, 0x9CB60632, 0xFF9666B7, 0xC1275ABF, 0x65EFB755, 0x1424E53B, 0xE1CB13F6, 0x7C9C8F03,
        0x66A1227A, 0x900515C0, 0xCC04930F, 0x94524D6C, 0x5351B04D, 0x580DB281, 0x770B60D3, 0x90D7B33E,
        0x99C146B4, 0x65913854, 0x841D1175, 0x89DE1DBC, 0x24627D92, 0xFC2B0104, 0x70642AE2, 0xCD012816,
        0x78E4F70E, 0xC9F560A7, 0xC3B25860, 0xB5D1A79A, 0x89375C73, 0xA2C6D0E5, 0x690852D1, 0xD7313EF4
    },
    MT64_N
};

OCSTL_THREAD_LOCAL __mt19937_64_t __uuid64 = {0};
OCSTL_THREAD_LOCAL int __uuid64_seeded = 0;

static void __mt19937_64_uuid_seed(uint64_t seed) {
    __uuid64.mt[0] = seed;
    {
        int i;
        for (i = 1; i < MT64_N; i++) {
            __uuid64.mt[i] = 6364136223846793005ULL * (__uuid64.mt[i - 1] ^ (__uuid64.mt[i - 1] >> 62)) + (uint64_t) i;
        }
    }
    __uuid64.index = MT64_N;
}

static void __mt19937_64_seed(uint64_t seed) {
    __rng64.mt[0] = seed;
    int i;
    for (i = 1; i < MT64_N; i++) {
        __rng64.mt[i] = 6364136223846793005ULL * (__rng64.mt[i - 1] ^ (__rng64.mt[i - 1] >> 62)) + (uint64_t) i;
    }
    __rng64.index = MT64_N;
}


// [0, 2^64 - 1]
static inline uint64_t __mt19937_64_next() {
    uint64_t y;

    if (__rng64.index >= MT64_N) {
        static const uint64_t mag01[2] = {0ULL, MT64_MATRIX_A};
        uint64_t y;
        int i;

        for (i = 0; i < MT64_N - MT64_M; i++) {
            y = (__rng64.mt[i] & MT64_UPPER_MASK) | (__rng64.mt[i + 1] & MT64_LOWER_MASK);
            __rng64.mt[i] = __rng64.mt[i + MT64_M] ^ (y >> 1) ^ mag01[y & 1ULL];
        }
        for (; i < MT64_N - 1; i++) {
            y = (__rng64.mt[i] & MT64_UPPER_MASK) | (__rng64.mt[i + 1] & MT64_LOWER_MASK);
            __rng64.mt[i] = __rng64.mt[i + (MT64_M - MT64_N)] ^ (y >> 1) ^ mag01[y & 1ULL];
        }
        y = (__rng64.mt[MT64_N - 1] & MT64_UPPER_MASK) | (__rng64.mt[0] & MT64_LOWER_MASK);
        __rng64.mt[MT64_N - 1] = __rng64.mt[MT64_M - 1] ^ (y >> 1) ^ mag01[y & 1ULL];

        __rng64.index = 0;
    }

    y = __rng64.mt[__rng64.index++];

    // tempering
    y ^= (y >> 29) & 0x5555555555555555ULL;
    y ^= (y << 17) & 0x71D67FFFEDA60000ULL;
    y ^= (y << 37) & 0xFFF7EEE000000000ULL;
    y ^= (y >> 43);

    return y;
}

// Unbiased uniform integer in [0, range-1] via rejection sampling.
// Avoids the modulo bias of (next() % range) when range is not a power of two.
static inline uint64_t __mt19937_64_bounded(uint64_t range) {
    uint64_t limit = UINT64_MAX - (UINT64_MAX % range);
    uint64_t r;
    do { r = __mt19937_64_next(); } while (r >= limit);
    return r % range;
}

// [0, 1) uniform real
static double __mt19937_random(void) {
    return (double) (__mt19937_64_next() >> 11) * (1.0 / 9007199254740992.0);
}


// [lo, hi] inclusive integer range (supports negative)
static int64_t __mt19937_randint(int64_t lo, int64_t hi) {
    uint64_t range = (uint64_t) (hi - lo) + 1ULL;
    return lo + (int64_t) __mt19937_64_bounded(range);
}

char *__mt19937_uuid(void) {
    if (!__uuid64_seeded) {
        __mt19937_64_uuid_seed(random_device());
        __uuid64_seeded = 1;
    }
    static OCSTL_THREAD_LOCAL char buf[37];
    static const char hex[] = "0123456789abcdef";

    uint64_t hi = __mt19937_64_next();
    uint64_t lo = __mt19937_64_next();

    hi = (hi & ~((uint64_t) 0xF << 12)) | ((uint64_t) 0x4 << 12);
    lo = (lo & ~((uint64_t) 0x3 << 62)) | ((uint64_t) 0x2 << 62);

    int p = 0;
    // time_low: 8자 (hi >> 32)
    int i = 0;
    for (i = 60; i >= 32; i -= 4)
        buf[p++] = hex[(hi >> i) & 0xF];
    buf[p++] = '-';
    // time_mid: 4자 (hi >> 16)
    for (i = 28; i >= 16; i -= 4)
        buf[p++] = hex[(hi >> i) & 0xF];
    buf[p++] = '-';
    // time_hi_and_version: 4자 (hi >> 0)
    for (i = 12; i >= 0; i -= 4)
        buf[p++] = hex[(hi >> i) & 0xF];
    buf[p++] = '-';
    // clock_seq: 4자 (lo >> 48)
    for (i = 60; i >= 48; i -= 4)
        buf[p++] = hex[(lo >> i) & 0xF];
    buf[p++] = '-';
    // node: 12자 (lo >> 0)
    for (i = 44; i >= 0; i -= 4)
        buf[p++] = hex[(lo >> i) & 0xF];

    buf[36] = '\0';
    return buf;
}

OPENCSTL_FUNC void __cstl_vector_shuffle(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    size_type64 length = OPENCSTL_NIDX(container, -1);

    {
        size_type64 i;
        for (i = length - 1; i > 0; i--) {
            size_type64 rng_idx = (size_type64) __mt19937_64_bounded((uint64_t) (i + 1));
            swap((char *) (*container) + i * type_size, (char *) (*container) + rng_idx * type_size, type_size);
        }
    }
}

OPENCSTL_FUNC void __cstl_deque_shuffle(void **container) {
    ptrdiff_t distance = OPENCSTL_NIDX(container, -1) + 1;
    size_type64 type_size = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (NIDX_TSIZE) * (ptrdiff_t) sizeof(size_type64) + distance);
    size_type64 length = *(_opencstl_ll_ua *) ((char *) *(void **) container + (ptrdiff_t) (-2) * (ptrdiff_t) sizeof(size_type64) + distance);

    {
        size_type64 i;
        for (i = length - 1; i > 0; i--) {
            size_type64 rng_idx = (size_type64) __mt19937_64_bounded((uint64_t) (i + 1));
            swap((char *) (*container) + i * type_size, (char *) (*container) + rng_idx * type_size, type_size);
        }
    }
}

OPENCSTL_FUNC void __cstl_list_shuffle(void **container) {
    size_type64 type_size = OPENCSTL_NIDX(container, NIDX_TSIZE);
    void **head = (void **) &OPENCSTL_NIDX(container, 0);
    size_type length = (size_type) OPENCSTL_NIDX(container, -1);
    if (length <= 1) { return; }

    // Copy list data to flat array
    void *ptr = malloc(type_size * length);
    void *it = *head;
    {
        size_type i;
        for (i = 0; i < length; i++) {
            memcpy((char *) ptr + (i * type_size), it, type_size);
            it = __cstl_list_next_prev(it, -1);
        }
    }
    // Fisher-Yates shuffle on flat array
    {
        size_type i;
        for (i = length - 1; i > 0; i--) {
            size_type rng_idx = (size_type) __mt19937_64_bounded((uint64_t) (i + 1));
            swap((char *) ptr + i * type_size, (char *) ptr + rng_idx * type_size, type_size);
        }
    }
    // Copy shuffled data back to list nodes
    it = *head;
    {
        size_type i;
        for (i = 0; i < length; i++) {
            memcpy(it, (char *) ptr + (i * type_size), type_size);
            it = __cstl_list_next_prev(it, -1);
        }
    }
    free(ptr);
}


void __mt19937_shuffle(void *container) {
    size_type64 container_type;
    if (__is_deque((void **) &container)) {
        ptrdiff_t distance = OPENCSTL_NIDX(((void**)&container), -1) + 1;
        container_type = *(_opencstl_ll_ua *) ((char *) *(void **) &container + (ptrdiff_t) (NIDX_CTYPE) * (ptrdiff_t) sizeof(size_type64) + distance);
    } else {
        container_type = OPENCSTL_NIDX(((void**)&container), NIDX_CTYPE);
    }
    switch (container_type) {
        case OPENCSTL_VECTOR: {
            __cstl_vector_shuffle((void **) &container);
        }
        break;
        case OPENCSTL_DEQUE: {
            __cstl_deque_shuffle((void **) &container);
        }
        break;
        case OPENCSTL_LIST: {
            __cstl_list_shuffle((void **) &container);
        }
        break;
        default: fault("Invalid operator");
            break;
    }
}

typedef void (*seed_fn)(uint64_t);

typedef double (*random_fn)(void);

typedef int64_t (*randint_fn)(int64_t, int64_t);

typedef char *(*uuid_fn)(void);

typedef void (*shuffle_fn)(void *);

typedef struct {
    random_fn random;
    randint_fn randint;
    seed_fn seed;
    uuid_fn uuid;
    shuffle_fn shuffle;
} RANDOM;

RANDOM mt19937 = {
    __mt19937_random,
    __mt19937_randint,
    __mt19937_64_seed,
    __mt19937_uuid,
    __mt19937_shuffle
};

// ██████╗░░█████╗░███╗░░██╗██████╗░░█████╗░███╗░░░███╗░░░░░░██████╗░███████╗██╗░░░██╗██╗░█████╗░███████╗
// ██╔══██╗██╔══██╗████╗░██║██╔══██╗██╔══██╗████╗░████║░░░░░░██╔══██╗██╔════╝██║░░░██║██║██╔══██╗██╔════╝
// ██████╔╝███████║██╔██╗██║██║░░██║██║░░██║██╔████╔██║░░░░░░██║░░██║█████╗░░╚██╗░██╔╝██║██║░░╚═╝█████╗░░
// ██╔══██╗██╔══██║██║╚████║██║░░██║██║░░██║██║╚██╔╝██║░░░░░░██║░░██║██╔══╝░░░╚████╔╝░██║██║░░██╗██╔══╝░░
// ██║░░██║██║░░██║██║░╚███║██████╔╝╚█████╔╝██║░╚═╝░██║█████╗██████╔╝███████╗░░╚██╔╝░░██║╚█████╔╝███████╗
// ╚═╝░░╚═╝╚═╝░░╚═╝╚═╝░░╚══╝╚═════╝░░╚════╝░╚═╝░░░░░╚═╝╚════╝╚═════╝░╚══════╝░░░╚═╝░░░╚═╝░╚════╝░╚══════╝
#include <time.h>
#if defined(OCSTL_OS_WINDOWS)
#include <windows.h>
// RtlGenRandom = SystemFunction036, advapi32에 포함 (MinGW 기본 링크)
#define RtlGenRandom SystemFunction036

BOOLEAN NTAPI RtlGenRandom(PVOID RandomBuffer, ULONG RandomBufferLength);
#elif defined(__APPLE__)
#include <sys/types.h>
#elif defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#include <errno.h>
#endif
inline uint64_t random_device(void) {
    uint64_t val = 0;
#if defined(OCSTL_CC_TCC)
    val = (uint64_t) time(NULL);
    val ^= (uint64_t) (uintptr_t) &val; // 스택 주소 XOR
    val ^= val << 21;
    val ^= val >> 35;
    val ^= val << 4;
#elif defined(OCSTL_OS_WINDOWS)
    RtlGenRandom(&val, sizeof(val));

#elif defined(__APPLE__)
    arc4random_buf(&val, sizeof(val));

#elif defined(__linux__)
    size_t done = 0;
    while (done < sizeof(val)) {
        long ret = syscall(SYS_getrandom,
                           (char *) &val + done, sizeof(val) - done, 0);
        if (ret < 0) {
            if (errno == EINTR) { continue; }
            FILE *f = fopen("/dev/urandom", "rb");
            if (f) {
                fread((char *) &val + done, 1, sizeof(val) - done, f);
                fclose(f);
            }
            break;
        }
        done += (size_t) ret;
    }

#else
    FILE *f = fopen("/dev/urandom", "rb");
    if (f) {
        fread(&val, sizeof(val), 1, f);
        fclose(f);
    }
#endif

    return val;
}
#endif //OPENCSTL_MT19937_H
