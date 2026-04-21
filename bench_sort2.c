#include "opencstl/opencstl.h"

typedef char *DTYPE;

//typedef double DTYPE;

int cmp(const void *a, const void *b) {
    return strcmp(a, b);
}


typedef int (*CMPFUNC)(const void *, const void *);

int sort_test() {
    CMPFUNC compare = cmp;


    size_t ALGORITHMS = 5;
    size_t REPEAT = 10;

    FILE *fp = fstream.open("../names.txt", "rt");

    char *buf = fstream.read(fp);
    int n;
    char **arr = string.split(buf, "\n", &n);


    DTYPE *arr2 = (DTYPE *) calloc(n, sizeof(DTYPE));
    for (int i = 0; i < n; i++) {
        arr2[i] = arr[i];
    }


    DTYPE *target = (DTYPE *) calloc(n, sizeof(DTYPE));


    watch t_beg;
    watch t_end;
    double m_diff = 0;
    double q_diff = 0;
    double t_diff = 0;
    double p_diff = 0;
    double r_diff = -1;
    double pm_diff = 0;
    for (int i = 0; i < ALGORITHMS * REPEAT; i++) {
        memcpy(target, arr, n * sizeof(char *));;
        switch (i % ALGORITHMS) {
            case 0: {
                t_beg = chrono.now();
                qsort(target, n, sizeof(DTYPE), compare);
                t_end = chrono.now();
                q_diff += chrono.duration(t_beg, t_end);
            };
                break;
            case 1: {
                t_beg = chrono.now();
                msort(target, n, sizeof(DTYPE), compare);
                t_end = chrono.now();
                m_diff += chrono.duration(t_beg, t_end);
            };
                break;
            case 2: {
                t_beg = chrono.now();
                tsort(target, n, sizeof(DTYPE), compare);
                t_end = chrono.now();
                t_diff += chrono.duration(t_beg, t_end);
            };
                break;
            case 3: {
                t_beg = chrono.now();
                pdqsort(target, n, sizeof(DTYPE), compare);
                t_end = chrono.now();
                p_diff += chrono.duration(t_beg, t_end);
            }
            break;
            case 4: {
                t_beg = chrono.now();
                pmsort(target, n, sizeof(DTYPE), compare);
                t_end = chrono.now();
                pm_diff += chrono.duration(t_beg, t_end);
            }
            break;
            default: {
                exit(-1);
            };
        }

        if (is_sorted(target, n, sizeof(DTYPE), compare) == -1) {
            puts("Not sorted");
        }
    }
    printf("|ENV/SORTING|%s|%s|%s|%s|%s|%s|\n", "qsort", "msort", "tsort", "pdqsort", "rsort", "pmsort");
    printf("|-|-|-|-|-|-|-|\n");
    printf("|%s/%s|%.2f|%.2f|%.2f|%.2f|%.2f|%.2f|\n",OCSTL_OS_STR,OCSTL_CC_STR,
           q_diff, m_diff, t_diff, p_diff, r_diff, pm_diff);


    return 0;
}

int main() {
    sort_test();
}
