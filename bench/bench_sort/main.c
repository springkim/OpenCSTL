#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64) || defined(_MSC_VER)

#include <windows.h>

typedef LARGE_INTEGER watch;

static watch now() {
    watch t;
    QueryPerformanceCounter(&t);
    return t;
}

static double duration(const watch t_beg, const watch t_end) {
    LARGE_INTEGER freq;
    QueryPerformanceFrequency(&freq);
    double ms = (double) (t_end.QuadPart - t_beg.QuadPart) * 1000.0 / (double) freq.QuadPart;
    return ms > 0 ? ms : -ms;
}


#elif defined(__MINGW32__) || defined(__MINGW64__) || defined(__GNUC__) || defined(__TINYC__)


#include <sys/time.h>
#include <time.h>

typedef struct timeval watch;

static watch now() {
    watch tv;
    gettimeofday(&tv, NULL);
    return tv;
}

static double duration(const watch t_beg, const watch t_end) {
    double ms = (t_end.tv_sec - t_beg.tv_sec) * 1000.0 +
                (t_end.tv_usec - t_beg.tv_usec) / 1000.0;
    return ms > 0 ? ms : -ms;
}


#else
#error Unsupported compiler/platform
#endif

#include "tsort.h"
#include "msort.h"
#include "pdqsort.h"
#include "rsort.h"
/*
 *
 *
 
void qsort(
    void *__base, 
    size_t __nel, 
    size_t __width, 
    int(*__compar)(const void *, const void *)) 
 */
int is_sorted(void *base, size_t nmemb, size_t size, int (*compar)(const void *, const void *)) {
    for (size_t i = 1; i < nmemb; i += size) {
        const int r = compar(((char *) base) + ((i - 1) * size), (char *) base + (i * size));
        if (r == -1) return 0;
    }
    return 1;
}

typedef long long DTYPE;

//typedef double DTYPE;

#define COMPARE(TYPE) int cmp(const void* a, const void* b) {return *(TYPE *) a < *(TYPE *) b ? -1 : *(TYPE *) a > *(TYPE *) b;}

COMPARE(DTYPE);


typedef int (*CMPFUNC)(const void *, const void *);

int sort_test() {
    CMPFUNC compare = cmp;

    DTYPE N = 5000000;

    size_t ALGORITHMS = 5;
#ifdef DEBUG
    size_t REPEAT = 3;
    printf("RAND_MAX: %d\n", RAND_MAX);
#else
    size_t REPEAT = 5;
#endif
    DTYPE *arr = (DTYPE *) calloc(N, sizeof(DTYPE));
    for (int i = 0; i < N; i++) {
        //arr[i] = rand() * 1.0 * rand();
        arr[i] = rand() * rand();
        //printf("arr[%d] = %d\n", i, arr[i]);
    }


    DTYPE *target = (DTYPE *) calloc(N, sizeof(DTYPE));


    watch bgn;
    watch end;
    double m_diff = 0;
    double q_diff = 0;
    double t_diff = 0;
    double p_diff = 0;
    double r_diff = 0;
    for (int i = 0; i < ALGORITHMS * REPEAT; i++) {
        memcpy(target, arr, N * sizeof(int));
        bgn = now();
        switch (i % ALGORITHMS) {
            case 0: {
                qsort(target, N, sizeof(DTYPE), compare);
                end = now();
                q_diff += duration(bgn, end);
            };
                break;
            case 1: {
                msort(target, N, sizeof(DTYPE), compare);
                end = now();
                m_diff += duration(bgn, end);
            };
                break;
            case 2: {
                tsort(target, N, sizeof(DTYPE), compare);
                end = now();
                t_diff += duration(bgn, end);
            };
                break;
            case 3: {
                pdqsort(target, N, sizeof(DTYPE), compare);
                end = now();
                p_diff += duration(bgn, end);
            }
            break;
            case 4: {
                rsort64(target, N);
                end = now();
                r_diff += duration(bgn, end);
            }
            break;
            default: {
                exit(-1);
            };
        }

        if (is_sorted(target, N, sizeof(DTYPE), compare) == -1) {
            puts("Not sorted");
        }
    }
    printf("qsort: %.2fms\n", q_diff);
    printf("msort: %.2fms\n", m_diff);

    printf("tsort: %.2fms\n", t_diff);
    printf("pdqsort: %.2fms\n", p_diff);
    printf("rsort: %.2fms\n", r_diff);

    printf("|%.2f|%.2f|%.2f|%.2f|\n",q_diff,m_diff,t_diff,p_diff);
    return 0;
}

int main(void) {
    sort_test();
    return 0;
}
