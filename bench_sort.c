#include "opencstl/opencstl.h"
#include  "opencstl/ipnsort.h"
typedef long long DTYPE;

//typedef double DTYPE;

#define COMPARE(TYPE) int cmp(const void* a, const void* b) {return *(TYPE *) a < *(TYPE *) b ? -1 : *(TYPE *) a > *(TYPE *) b;}

COMPARE(DTYPE);

typedef int (*CMPFUNC)(const void *, const void *);

int sort_test() {
    CMPFUNC compare = cmp;

    DTYPE N = 5000000;

    size_t ALGORITHMS = 7;
    size_t REPEAT = 10;
    // DTYPE *arr = (DTYPE *) calloc(N, sizeof(DTYPE));
    VECTOR(DTYPE) arr = new_vector(DTYPE);
    for (int i = 0; i < N; i++) {
        //arr[i] = rand() * 1.0 * rand();
        DTYPE val = rand() * rand();
        push_back(arr, val);
        //printf("arr[%d] = %d\n", i, arr[i]);
    }


    DTYPE *target = (DTYPE *) calloc(N, sizeof(DTYPE));


    double t_beg;
    double t_end;
    double m_diff = 0;
    double q_diff = 0;
    double t_diff = 0;
    double p_diff = 0;
    double r_diff = 0;
    double pm_diff = 0;
    double ipn_diff = 0;
    for (int i = 0; i < ALGORITHMS * REPEAT; i++) {
        mt19937.shuffle(arr);
        memcpy(target, arr, N * sizeof(DTYPE));;
        switch (i % ALGORITHMS) {
            case 0: {
                t_beg = ttime();
                qsort(target, N, sizeof(DTYPE), compare);
                t_end = ttime();
                q_diff += t_end - t_beg;
            };
                break;
            case 1: {
                t_beg = ttime();
                msort(target, N, sizeof(DTYPE), compare);
                t_end = ttime();
                m_diff += t_end - t_beg;
            };
                break;
            case 2: {
                t_beg = ttime();
                tsort(target, N, sizeof(DTYPE), compare);
                t_end = ttime();
                t_diff += t_end - t_beg;
            };
                break;
            case 3: {
                t_beg = ttime();
                pdqsort(target, N, sizeof(DTYPE), compare);
                t_end = ttime();
                p_diff += t_end - t_beg;
            }
            break;
            case 4: {
                t_beg = ttime();
                rsort(target, N);
                t_end = ttime();
                r_diff += t_end - t_beg;
            }
            break;
            case 5: {
                t_beg = ttime();
                pmsort(target, N, sizeof(DTYPE), compare);
                t_end = ttime();
                pm_diff += t_end - t_beg;
            }
            break;
            case 6: {
                t_beg = ttime();
                ipnsort(target, N, sizeof(DTYPE), compare);
                t_end = ttime();
                ipn_diff += t_end - t_beg;
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
    printf("|ENV/SORTING|%s|%s|%s|%s|%s|%s|%s|\n", "qsort", "msort", "tsort", "pdqsort", "rsort", "pmsort", "ipnsort");
    printf("|-|-|-|-|-|-|-|-|\n");
    printf("|%s/%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|\n",OCSTL_OS_STR,OCSTL_CC_STR,
           q_diff, m_diff, t_diff, p_diff, r_diff, pm_diff, ipn_diff);

    free(target);
    destroy(arr);
    return 0;
}

int main() {
    sort_test();
}
