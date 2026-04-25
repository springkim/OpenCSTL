#include<stdio.h>
#include<math.h>
#include "gmp.h"

#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"


void test_uint64() {
    MsgBoxCLI("mpz_t");
    VECTOR(uint64_t) arr = new_vector(uint64_t);
    for (int64_t i = 0; i < 16; i++) {
        push_back(arr, i);
    }
    for (uint64_t *it = begin(arr); it != end(arr); it = next(it)) {
        uint64_t base = *it;
        uint64_t exp = *it;
        uint64_t val = 1;
        while (exp-- > 0) {
            val *= base;
        }
        *it = val;
    }
    for (uint64_t *it = begin(arr); it != end(arr); it = next(it)) {
        println("{}", *it);
    }
    destroy(arr);
}


static void mpz_print_ptr(const char *label, mpz_t *v) {
    char *s = mpz_get_str(NULL, 10, *v);
    printf("%s%s\n", label, s);
    free(s);
}

void test_mpz() {
    MsgBoxCLI("mpz_t");
    VECTOR(mpz_t *) arr = new_vector(mpz_t *);


    for (int i = 0; i < 66; i++) {
        mpz_t *p = malloc(sizeof(mpz_t));
        mpz_init_set_ui(*p, (unsigned long) i);
        push_back(arr, p);
    }


    for (mpz_t **it = begin(arr); it != end(arr); it = next(it)) {
        unsigned long x = mpz_get_ui(**it); // 지수로 쓸 값 먼저 추출
        mpz_pow_ui(**it, **it, x); // base = *it, exp = x
    }


    for (mpz_t **it = begin(arr); it != end(arr); it = next(it)) {
        mpz_print_ptr("", *it);
    }

    for (mpz_t **it = begin(arr); it != end(arr); it = next(it)) {
        mpz_clear(**it);
        free(*it);
    }
    destroy(arr);
}

int main() {
    test_uint64();
    test_mpz();
    return 0;
}
