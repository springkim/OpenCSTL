#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
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


    struct timeval bgn;
    struct timeval end;
    double m_diff = 0;
    double q_diff = 0;
    double t_diff = 0;
    double p_diff = 0;
    double r_diff = 0;
    for (int i = 0; i < ALGORITHMS * REPEAT; i++) {
        memcpy(target, arr, N * sizeof(int));
        gettimeofday(&bgn, NULL);
        switch (i % ALGORITHMS) {
            case 0: {
                qsort(target, N, sizeof(DTYPE), compare);
                gettimeofday(&end, NULL);
                q_diff += end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;
            };
                break;
            case 1: {
                msort(target, N, sizeof(DTYPE), compare);
                gettimeofday(&end, NULL);
                m_diff += end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;
            };
                break;
            case 2: {
                tsort(target, N, sizeof(DTYPE), compare);
                gettimeofday(&end, NULL);
                t_diff += end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;
            };
                break;
            case 3: {
                pdqsort(target, N, sizeof(DTYPE), compare);
                gettimeofday(&end, NULL);
                p_diff += end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;
            }
            break;
            case 4: {
                rsort64(target, N);
                gettimeofday(&end, NULL);
                r_diff += end.tv_sec + end.tv_usec / 1000000.0 - bgn.tv_sec - bgn.tv_usec / 1000000.0;
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
    printf("qsort: %f\n", q_diff);
    printf("msort: %f\n", m_diff);
    
    printf("tsort: %f\n", t_diff);
    printf("pdqsort: %f\n", p_diff);
    printf("rsort: %f\n", r_diff);
    return 0;
}

int main(void) {
    sort_test();
    return 0;
}
