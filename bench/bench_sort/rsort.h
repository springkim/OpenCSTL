//
// Created by spring on 2026. 4. 12..
//

#ifndef BENCH_SORT_RSORT_H
#define BENCH_SORT_RSORT_H
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>

void rsort32(int *__base, size_t n) {
    if (__base == NULL || n <= 1) {
        return;
    }

    int *tmp = (int *) malloc(n * sizeof(int));
    if (tmp == NULL) {
        return;
    }

    {
        size_t count[256];
        size_t pass, i;
        unsigned int sign_mask = 1u << (sizeof(int) * CHAR_BIT - 1);

        for (pass = 0; pass < sizeof(int); ++pass) {
            size_t shift = pass * 8;

            for (i = 0; i < 256; ++i) {
                count[i] = 0;
            }

            for (i = 0; i < n; ++i) {
                unsigned int key = ((unsigned int) __base[i]) ^ sign_mask;
                unsigned int digit = (key >> shift) & 0xFFu;
                count[digit]++;
            }

            for (i = 1; i < 256; ++i) {
                count[i] += count[i - 1];
            }

            for (i = n; i > 0; --i) {
                unsigned int key = ((unsigned int) __base[i - 1]) ^ sign_mask;
                unsigned int digit = (key >> shift) & 0xFFu;
                tmp[--count[digit]] = __base[i - 1];
            }

            for (i = 0; i < n; ++i) {
                __base[i] = tmp[i];
            }
        }
    }

    free(tmp);
}

void rsort64(long long *__base, size_t n) {
    if (__base == NULL || n <= 1) {
        return;
    }

    long long *tmp = (long long *) malloc(n * sizeof(long long));
    if (tmp == NULL) {
        return;
    }

    {
        size_t count[256];
        size_t pass, i;
        unsigned long long sign_mask =
                1ULL << (sizeof(long long) * CHAR_BIT - 1);

        for (pass = 0; pass < sizeof(long long); ++pass) {
            size_t shift = pass * 8;

            for (i = 0; i < 256; ++i) {
                count[i] = 0;
            }

            for (i = 0; i < n; ++i) {
                unsigned long long key =
                        ((unsigned long long) __base[i]) ^ sign_mask;
                unsigned int digit = (unsigned int) ((key >> shift) & 0xFFULL);
                count[digit]++;
            }

            for (i = 1; i < 256; ++i) {
                count[i] += count[i - 1];
            }

            for (i = n; i > 0; --i) {
                unsigned long long key =
                        ((unsigned long long) __base[i - 1]) ^ sign_mask;
                unsigned int digit = (unsigned int) ((key >> shift) & 0xFFULL);
                tmp[--count[digit]] = __base[i - 1];
            }

            for (i = 0; i < n; ++i) {
                __base[i] = tmp[i];
            }
        }
    }

    free(tmp);
}

#endif //BENCH_SORT_RSORT_H
