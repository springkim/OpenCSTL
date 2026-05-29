#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

#include <stdbool.h>

void setUp(void) {}
void tearDown(void) {}

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *) a, y = *(const int *) b;
    return (x > y) - (x < y);
}

static bool pred_even(const void *it) {
    return (*(const int *) it) % 2 == 0;
}

static VECTOR(int) make_int_vector(const int *values, size_t n) {
    VECTOR(int) v = new_vector(int);
    for (size_t i = 0; i < n; i++) {
        push_back(v, values[i]);
    }
    return v;
}

static void test_vector_new_is_empty(void) {
    VECTOR(int) v = new_vector(int);
    TEST_ASSERT_EQUAL_INT(0, size(v));
    destroy(v);
}

static void test_vector_push_back_grows_size(void) {
    VECTOR(int) v = new_vector(int);
    for (int i = 0; i < 10; i++) push_back(v, i);
    TEST_ASSERT_EQUAL_INT(10, size(v));
    TEST_ASSERT_EQUAL_INT(0, v[0]);
    TEST_ASSERT_EQUAL_INT(9, v[9]);
    destroy(v);
}

static void test_vector_pop_back_shrinks(void) {
    int vals[] = {1, 2, 3};
    VECTOR(int) v = make_int_vector(vals, 3);
    pop_back(v);
    TEST_ASSERT_EQUAL_INT(2, size(v));
    TEST_ASSERT_EQUAL_INT(2, v[1]);
    destroy(v);
}

static void test_vector_capacity_grows_with_push(void) {
    VECTOR(int) v = new_vector(int);
    size_type cap0 = capacity(v);
    for (int i = 0; i < 100; i++) push_back(v, i);
    TEST_ASSERT_TRUE(capacity(v) >= 100);
    TEST_ASSERT_TRUE(capacity(v) >= cap0);
    destroy(v);
}

static void test_vector_reserve(void) {
    VECTOR(int) v = new_vector(int);
    reserve(v, 64);
    TEST_ASSERT_TRUE(capacity(v) >= 64);
    TEST_ASSERT_EQUAL_INT(0, size(v));
    destroy(v);
}

static void test_vector_indexing_after_grow(void) {
    VECTOR(int) v = new_vector(int);
    for (int i = 0; i < 200; i++) push_back(v, i * 2);
    TEST_ASSERT_EQUAL_INT(0,    v[0]);
    TEST_ASSERT_EQUAL_INT(100,  v[50]);
    TEST_ASSERT_EQUAL_INT(398,  v[199]);
    destroy(v);
}

static void test_vector_iteration_begin_end(void) {
    int vals[] = {10, 20, 30, 40};
    VECTOR(int) v = make_int_vector(vals, 4);
    int sum = 0;
    for (int *it = begin(v); it != end(v); it = next(it)) {
        sum += *it;
    }
    TEST_ASSERT_EQUAL_INT(100, sum);
    destroy(v);
}

static void test_vector_find_present(void) {
    int vals[] = {1, 3, 5, 7, 9};
    VECTOR(int) v = make_int_vector(vals, 5);

    int *it = (int *) find(v, 7);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(7, *it);
    destroy(v);
}

static void test_vector_find_absent(void) {
    int vals[] = {1, 3, 5};
    VECTOR(int) v = make_int_vector(vals, 3);

    int *it = (int *) find(v, 99);
    TEST_ASSERT_NULL(it);
    destroy(v);
}

static void test_vector_count_basic(void) {
    int vals[] = {1, 2, 2, 3, 2, 4};
    VECTOR(int) v = make_int_vector(vals, 6);

    int two = 2;
    TEST_ASSERT_EQUAL_UINT64(3, __cstl_vector_count((void **) &v, &two));
    destroy(v);
}

static void test_vector_count_if_basic(void) {
    int vals[] = {1, 2, 3, 4, 5, 6, 7, 8};
    VECTOR(int) v = make_int_vector(vals, 8);

    TEST_ASSERT_EQUAL_UINT64(4, __cstl_vector_count_if((void **) &v, pred_even));
    destroy(v);
}

static void test_vector_lower_bound(void) {
    int vals[] = {1, 3, 5, 7, 9};
    VECTOR(int) v = make_int_vector(vals, 5);

    int key = 4;
    int *it = (int *) __cstl_vector_lower_bound((void **) &v, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(5, *it);
    destroy(v);
}

static void test_vector_upper_bound(void) {
    int vals[] = {1, 3, 5, 7, 9};
    VECTOR(int) v = make_int_vector(vals, 5);

    int key = 5;
    int *it = (int *) __cstl_vector_upper_bound((void **) &v, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(7, *it);
    destroy(v);
}

static void test_vector_max_element(void) {
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    VECTOR(int) v = make_int_vector(vals, 8);

    int *it = (int *) max_element(v);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(9, *it);
    destroy(v);
}

static void test_vector_min_element(void) {
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    VECTOR(int) v = make_int_vector(vals, 8);

    int *it = (int *) min_element(v);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(1, *it);
    destroy(v);
}

static void test_vector_clear(void) {
    int vals[] = {1, 2, 3};
    VECTOR(int) v = make_int_vector(vals, 3);
    clear(v);
    TEST_ASSERT_EQUAL_INT(0, size(v));
    destroy(v);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_vector_new_is_empty);
    RUN_TEST(test_vector_push_back_grows_size);
    RUN_TEST(test_vector_pop_back_shrinks);
    RUN_TEST(test_vector_capacity_grows_with_push);
    RUN_TEST(test_vector_reserve);
    RUN_TEST(test_vector_indexing_after_grow);
    RUN_TEST(test_vector_iteration_begin_end);
    RUN_TEST(test_vector_find_present);
    RUN_TEST(test_vector_find_absent);
    RUN_TEST(test_vector_count_basic);
    RUN_TEST(test_vector_count_if_basic);
    RUN_TEST(test_vector_lower_bound);
    RUN_TEST(test_vector_upper_bound);
    RUN_TEST(test_vector_max_element);
    RUN_TEST(test_vector_min_element);
    RUN_TEST(test_vector_clear);
    return UNITY_END();
}
