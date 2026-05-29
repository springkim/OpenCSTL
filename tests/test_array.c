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

static void test_array_construct_and_size(void) {
    ARRAY(int) a = cstl_array(int, 5);
    TEST_ASSERT_EQUAL_INT(5, size(a));
    destroy(a);
}

static void test_array_indexing(void) {
    ARRAY(int) a = cstl_array(int, 4);
    a[0] = 10; a[1] = 20; a[2] = 30; a[3] = 40;
    TEST_ASSERT_EQUAL_INT(10, a[0]);
    TEST_ASSERT_EQUAL_INT(40, a[3]);
    destroy(a);
}

static void test_array_begin_end_iteration(void) {
    ARRAY(int) a = cstl_array(int, 3);
    a[0] = 7; a[1] = 8; a[2] = 9;

    int sum = 0;
    for (int *it = begin(a); it != end(a); it = next(it)) {
        sum += *it;
    }
    TEST_ASSERT_EQUAL_INT(24, sum);
    destroy(a);
}

static void test_array_rbegin_rend_iteration(void) {
    ARRAY(int) a = cstl_array(int, 3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    int seq[3], idx = 0;
    for (int *it = rbegin(a); it != rend(a); it = prev(it)) {
        seq[idx++] = *it;
    }
    TEST_ASSERT_EQUAL_INT(3, seq[0]);
    TEST_ASSERT_EQUAL_INT(2, seq[1]);
    TEST_ASSERT_EQUAL_INT(1, seq[2]);
    destroy(a);
}

static void test_array_count_basic(void) {
    ARRAY(int) a = cstl_array(int, 6);
    a[0] = 2; a[1] = 1; a[2] = 2; a[3] = 3; a[4] = 2; a[5] = 4;

    int two = 2, nine = 9;
    TEST_ASSERT_EQUAL_UINT64(3, __cstl_array_count((void **) &a, &two));
    TEST_ASSERT_EQUAL_UINT64(0, __cstl_array_count((void **) &a, &nine));
    destroy(a);
}

static void test_array_count_if_basic(void) {
    ARRAY(int) a = cstl_array(int, 6);
    a[0] = 1; a[1] = 2; a[2] = 3; a[3] = 4; a[4] = 5; a[5] = 6;

    TEST_ASSERT_EQUAL_UINT64(3, __cstl_array_count_if((void **) &a, pred_even));
    destroy(a);
}

static void test_array_lower_bound_exact(void) {
    ARRAY(int) a = cstl_array(int, 5);
    a[0] = 1; a[1] = 3; a[2] = 5; a[3] = 7; a[4] = 9;

    int key = 5;
    int *it = (int *) __cstl_array_lower_bound((void **) &a, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(5, *it);
    TEST_ASSERT_EQUAL_PTR(&a[2], it);
    destroy(a);
}

static void test_array_lower_bound_rounds_up(void) {
    ARRAY(int) a = cstl_array(int, 5);
    a[0] = 1; a[1] = 3; a[2] = 5; a[3] = 7; a[4] = 9;

    int key = 4;
    int *it = (int *) __cstl_array_lower_bound((void **) &a, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(5, *it);
    destroy(a);
}

static void test_array_upper_bound_basic(void) {
    ARRAY(int) a = cstl_array(int, 5);
    a[0] = 1; a[1] = 3; a[2] = 5; a[3] = 7; a[4] = 9;

    int key = 5;
    int *it = (int *) __cstl_array_upper_bound((void **) &a, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(7, *it);
    destroy(a);
}

static void test_array_upper_bound_beyond_end(void) {
    ARRAY(int) a = cstl_array(int, 3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    int key = 100;
    int *it = (int *) __cstl_array_upper_bound((void **) &a, &key, cmp_int);
    TEST_ASSERT_NULL(it);
    destroy(a);
}

static void test_array_find_present(void) {
    ARRAY(int) a = cstl_array(int, 4);
    a[0] = 1; a[1] = 4; a[2] = 9; a[3] = 16;

    int *it = (int *) find(a, 9);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(9, *it);
    destroy(a);
}

static void test_array_find_absent(void) {
    ARRAY(int) a = cstl_array(int, 3);
    a[0] = 1; a[1] = 2; a[2] = 3;

    int *it = (int *) find(a, 99);
    TEST_ASSERT_NULL(it);
    destroy(a);
}

static void test_array_max_element(void) {
    ARRAY(int) a = cstl_array(int, 5);
    a[0] = 3; a[1] = 1; a[2] = 4; a[3] = 1; a[4] = 5;

    int *it = (int *) max_element(a);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(5, *it);
    destroy(a);
}

static void test_array_min_element(void) {
    ARRAY(int) a = cstl_array(int, 5);
    a[0] = 3; a[1] = 1; a[2] = 4; a[3] = 1; a[4] = 5;

    int *it = (int *) min_element(a);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(1, *it);
    destroy(a);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_array_construct_and_size);
    RUN_TEST(test_array_indexing);
    RUN_TEST(test_array_begin_end_iteration);
    RUN_TEST(test_array_rbegin_rend_iteration);
    RUN_TEST(test_array_find_present);
    RUN_TEST(test_array_find_absent);
    RUN_TEST(test_array_count_basic);
    RUN_TEST(test_array_count_if_basic);
    RUN_TEST(test_array_lower_bound_exact);
    RUN_TEST(test_array_lower_bound_rounds_up);
    RUN_TEST(test_array_upper_bound_basic);
    RUN_TEST(test_array_upper_bound_beyond_end);
    RUN_TEST(test_array_max_element);
    RUN_TEST(test_array_min_element);
    return UNITY_END();
}
