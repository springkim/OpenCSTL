#include"opencstl/opencstl.h"
typedef unsigned long long uint64;
static uint64 _seed64 = 1;

static void srand64(uint64 seed) {
    _seed64 = seed;
}

static uint64 rand64(void) {
    _seed64 = (_seed64 * 6364136223846793005ULL) + 1442695040888963407ULL;
    return (uint64) _seed64;
}

#define INLINE_SORT(BASE,SIZE,TYPE,COMP) do{ TYPE* base=(TYPE*)BASE;\
for(uint64 i=0;i<(SIZE)-1;i++) {\
for(uint64 j=0;j<(SIZE)-1-i;j++) {\
if(base[j] COMP base[j+1]) {\
TYPE tmp=base[j];base[j]=base[j+1];base[j+1]=tmp;\
}\
}\
}\
}while(0)

int main() {
    int N = 100000000;
    N = 100;
    srand64(77);
    VECTOR(uint64) v = new_vector(uint64);

    for (uint64 i = 0; i < N; i++) {
        push_back(v, rand64());
    }

    watch t_beg = chrono.now();

    INLINE_SORT(v, N, UINT64, >);
    //sort(v, LESS(unsigned long long));

    //pmsort(v,size(v), sizeof(uint64),LESS(unsigned long long));

    watch t_end = chrono.now();
    double ms = chrono.duration(t_beg, t_end);
    bool is_sort = true;
    for (int i = 0; i < N - 1; i++) {
        if (*((uint64 *) v + i) > *((uint64 *) v + (i + 1))) {
            is_sort = false;
            break;
        }
    }
    if (is_sort) {
        puts("Sorted!!");
    }
    printf("OpenCSTL: %f\n", ms);

    for (uint64 *it = begin(v); it != end(v); it++) {
        printf("%llu\n", *it);
    }

    destroy(v);

    return 0;
}
