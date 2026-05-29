#include <stdlib.h>
#include <string.h>

static void isort(void *base, size_t number, size_t width, int (*compare)(const void *, const void *)) {
    char *arr = (char *) base;
    char sbuf[1024];
    char *tmp = (width <= sizeof(sbuf)) ? sbuf : (char *) malloc(width);
    {
        size_t i;
        for (i = 1; i < number; i++) {
            memcpy(tmp, arr + i * width, width);
            size_t lo = 0, hi = i;
            while (lo < hi) {
                size_t mid = lo + ((hi - lo) >> 1);
                if (compare(tmp, arr + mid * width) < 0) { hi = mid; } else { lo = mid + 1; }
            }
            if (lo < i) {
                memmove(arr + (lo + 1) * width, arr + lo * width, (i - lo) * width);
                memcpy(arr + lo * width, tmp, width);
            }
        }
    }
    if (tmp != sbuf) {
        free(tmp);
    }
}

#define MSORT_ISORT_THRESH 32


static void msort_merge(char *arr, size_t len1, size_t len2, size_t sz,
                        int (*cmp)(const void *, const void *), char *buf) {
    if (cmp(arr + (len1 - 1) * sz, arr + len1 * sz) <= 0) { return; }
    if (len1 <= len2) {
        memcpy(buf, arr, len1 * sz);
        char *c1 = buf, *e1 = buf + len1 * sz;
        char *c2 = arr + len1 * sz, *e2 = c2 + len2 * sz;
        char *d = arr;
        while (c1 < e1 && c2 < e2) {
            if (cmp(c2, c1) < 0) {
                memcpy(d, c2, sz);
                c2 += sz;
            } else {
                memcpy(d, c1, sz);
                c1 += sz;
            }
            d += sz;
        }
        if (c1 < e1) {
            memcpy(d, c1, (size_t)(e1 - c1));
        }
    } else {
        memcpy(buf, arr + len1 * sz, len2 * sz);
        size_t i = len1, j = len2, k = len1 + len2;
        while (i > 0 && j > 0) {
            k--;
            if (cmp(buf + (j - 1) * sz, arr + (i - 1) * sz) < 0) {
                memcpy(arr + k * sz, arr + (i - 1) * sz, sz);
                i--;
            } else {
                memcpy(arr + k * sz, buf + (j - 1) * sz, sz);
                j--;
            }
        }
        if (j > 0) {
            memcpy(arr, buf, j * sz);
        }
    }
}

void msort(void *base, size_t number, size_t width, int (*compare)(const void *, const void *)) {
    if (number < 2) { return; }
    char *arr = (char *) base;
    size_t sz = width;
    {
        size_t i;
        for (i = 0; i < number; i += MSORT_ISORT_THRESH) {
            size_t blk = number - i;
            if (blk > MSORT_ISORT_THRESH) { blk = MSORT_ISORT_THRESH; }
            isort(arr + i * sz, blk, sz, compare);
        }
    }
    char *buf = (char *) calloc(((number + 1) / 2), sz);
    if (!buf) { return; }
    {
        size_t mb;
        for (mb = MSORT_ISORT_THRESH; mb < number; mb *= 2) {
            {
                size_t i;
                for (i = 0; i + mb < number; i += 2 * mb) {
                    size_t len1 = mb;
                    size_t len2 = number - i - mb;
                    if (len2 > mb) { len2 = mb; }
                    msort_merge(arr + i * sz, len1, len2, sz, compare, buf);
                }
            }
        }
    }
    free(buf);
}

// clang -O3 -DNDEBUG -mcpu=native -arch arm64 -fvisibility=hidden -dynamiclib -o msort.dylib msort.c
// clang -O3 -dynamiclib -o msort.dylib msort.c
// gcc-15 -O3 -DNDEBUG -mcpu=apple-m1 -mtune=native -flto -dynamiclib -o msort.dylib msort.c
// cp msort.dylib ../cmake-build-release-tcc/
// nm -gU msort.dylib

/*
*clang -O3 -DNDEBUG -flto=thin -mcpu=native -arch arm64 -fvisibility=hidden -dynamiclib -o msort.dylib msort.c
strip -x msort.dylib
 *
 */
