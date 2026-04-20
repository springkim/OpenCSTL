#include "opencstl/opencstl.h"

typedef long long DTYPE;

//typedef double DTYPE;

#define COMPARE(TYPE) int cmp(const void* a, const void* b) {return *(TYPE *) a < *(TYPE *) b ? -1 : *(TYPE *) a > *(TYPE *) b;}

COMPARE(DTYPE);

typedef int (*CMPFUNC)(const void *, const void *);

int sort_test() {
    CMPFUNC compare = cmp;

    DTYPE N = 5000000;

    size_t ALGORITHMS = 6;
    size_t REPEAT = 10;
    DTYPE *arr = (DTYPE *) calloc(N, sizeof(DTYPE));
    for (int i = 0; i < N; i++) {
        //arr[i] = rand() * 1.0 * rand();
        arr[i] = rand() * rand();
        //printf("arr[%d] = %d\n", i, arr[i]);
    }


    DTYPE *target = (DTYPE *) calloc(N, sizeof(DTYPE));


    watch t_beg;
    watch t_end;
    double m_diff = 0;
    double q_diff = 0;
    double t_diff = 0;
    double p_diff = 0;
    double r_diff = 0;
    double pm_diff = 0;
    for (int i = 0; i < ALGORITHMS * REPEAT; i++) {
        memcpy(target, arr, N * sizeof(int));;
        switch (i % ALGORITHMS) {
            case 0: {
                t_beg = chrono.now();
                qsort(target, N, sizeof(DTYPE), compare);
                t_end = chrono.now();
                q_diff += chrono.duration(t_beg, t_end);
            };
                break;
            case 1: {
                t_beg = chrono.now();
                msort(target, N, sizeof(DTYPE), compare);
                t_end = chrono.now();
                m_diff += chrono.duration(t_beg, t_end);
            };
                break;
            case 2: {
                t_beg = chrono.now();
                tsort(target, N, sizeof(DTYPE), compare);
                t_end = chrono.now();
                t_diff += chrono.duration(t_beg, t_end);
            };
                break;
            case 3: {
                t_beg = chrono.now();
                pdqsort(target, N, sizeof(DTYPE), compare);
                t_end = chrono.now();
                p_diff += chrono.duration(t_beg, t_end);
            }
            break;
            case 4: {
                t_beg = chrono.now();
                rsort(target, N);
                t_end = chrono.now();
                r_diff += chrono.duration(t_beg, t_end);
            }
            break;
            case 5: {
                t_beg = chrono.now();
                pmsort(target, N, sizeof(DTYPE), compare);
                t_end = chrono.now();
                pm_diff += chrono.duration(t_beg, t_end);
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
    printf("|ENV/SORTING|%s|%s|%s|%s|%s|%s|\n", "qsort", "msort", "tsort", "pdqsort", "rsort","pmsort");
    printf("|-|-|-|-|-|-|-|\n");
    printf("|%s/%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|\n",OCSTL_OS_STR,OCSTL_CC_STR,
        q_diff,m_diff,t_diff,p_diff,r_diff,pm_diff);

    // printf("qsort: %.2fms\n", q_diff);
    // printf("msort: %.2fms\n", m_diff);
    //
    // printf("tsort: %.2fms\n", t_diff);
    // printf("pdqsort: %.2fms\n", p_diff);
    // printf("rsort: %.2fms\n", r_diff);
    //
    // printf("pmsort: %.2fms\n", pm_diff);
    //
    // printf("|%.2f|%.2f|%.2f|%.2f|\n", q_diff, m_diff, t_diff, p_diff);
    return 0;
}

int main() {
    sort_test();
}