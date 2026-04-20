//
// Created by spring on 2026. 4. 12..
//

#ifndef BENCH_SORT_RSORT_H
#define BENCH_SORT_RSORT_H

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>

/*
 * C99-compatible LSD radix sort
 * - signed int32_t / int64_t 전용
 * - rsort(base, n) 매크로는 원소 크기로 32/64 자동 선택
 * - 지원하지 않는 타입 크기면 컴파일 에러
 *
 * 주의:
 *   C99에는 _Generic이 없어서 "정확한 타입" 구분은 불가능합니다.
 *   따라서 rsort(base, n)은 sizeof(*base)==4 또는 8 기준으로 선택합니다.
 *   의도한 사용은 int32_t* / int64_t* 입니다.
 */

static void rsort32(int32_t *__base, size_t n) {
    if (__base == NULL || n <= 1) {
        return;
    }

    int32_t *tmp = (int32_t *) malloc(n * sizeof(int32_t));
    if (tmp == NULL) {
        return;
    }

    {
        size_t count[256];
        size_t pass, i;
        uint32_t sign_mask = (uint32_t) 1u << (sizeof(int32_t) * CHAR_BIT - 1);

        for (pass = 0; pass < sizeof(int32_t); ++pass) {
            size_t shift = pass * 8;

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

static void rsort64(int64_t *__base, size_t n) {
    if (__base == NULL || n <= 1) {
        return;
    }

    int64_t *tmp = (int64_t *) malloc(n * sizeof(int64_t));
    if (tmp == NULL) {
        return;
    }

    {
        size_t count[256];
        size_t pass, i;
        uint64_t sign_mask = (uint64_t) 1ULL << (sizeof(int64_t) * CHAR_BIT - 1);

        for (pass = 0; pass < sizeof(int64_t); ++pass) {
            size_t shift = pass * 8;

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

/*
 * 지원하지 않는 타입이면 컴파일 에러 유도용
 * sizeof(char[-1])가 되어 컴파일 실패합니다.
 */
#define RSORT_STATIC_TYPECHECK(base) \
    ((void)sizeof(char[(sizeof(*(base)) == 4 || sizeof(*(base)) == 8) ? 1 : -1]))

/*
 * C99에서는 _Generic이 없으므로 원소 크기로 분기
 * 의도한 사용:
 *   int32_t * -> rsort32
 *   int64_t * -> rsort64
 */
#define rsort(base, n)                                                     \
    do {                                                                   \
        RSORT_STATIC_TYPECHECK(base);                                      \
        if (sizeof(*(base)) == sizeof(int32_t)) {                          \
            rsort32((int32_t *)(base), (n));                               \
        } else {                                                           \
            rsort64((int64_t *)(base), (n));                               \
        }                                                                  \
    } while (0)

#endif /* BENCH_SORT_RSORT_H */
