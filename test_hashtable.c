//
// Created by spring on 4/22/2026.
//
#include "opencstl/opencstl.h"

// const int SET_SIZE = 50000000;

const int SET_SIZE = 50000000;
void test_opencstl_unordered_set() {
    srand(21);
    UNORDERED_SET(int) v = new_unordered_set(int);
    //reserve(v, SET_SIZE);
    for (int i = 0; i < SET_SIZE; ++i) {
        insert(v, rand()*rand());
    }
    printf("size: %d\n", size(v));
    destroy(v);
}
int main() {
    watch t_beg = chrono.now();
    test_opencstl_unordered_set();
    watch t_end = chrono.now();
    double ms = chrono.duration(t_beg, t_end);
    printf("%f ms\n", ms);
}