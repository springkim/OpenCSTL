#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

#include <stdbool.h>

void setUp(void) {}
void tearDown(void) {}

static int cmp_int(const void *a, const void *b) {
    int x = *(const int *) a;
    int y = *(const int *) b;
    return (x > y) - (x < y);
}

static bool pred_even(const void *it) {
    return (*(const int *) it) % 2 == 0;
}

static bool pred_negative(const void *it) {
    return (*(const int *) it) < 0;
}

static DEQUE(int) make_int_deque(const int *values, size_t n) {
    DEQUE(int) q = new_deque(int);
    for (size_t i = 0; i < n; i++) {
        push_back(q, values[i]);
    }
    return q;
}

static void test_deque_count_basic(void) {
    int vals[] = {1, 2, 3, 2, 4, 2, 5};
    DEQUE(int) q = make_int_deque(vals, sizeof(vals) / sizeof(vals[0]));

    int two = 2;
    int nine = 9;
    TEST_ASSERT_EQUAL_UINT64(3, __cstl_deque_count((void **) &q, &two));
    TEST_ASSERT_EQUAL_UINT64(0, __cstl_deque_count((void **) &q, &nine));

    destroy(q);
}

static void test_deque_count_after_push_front(void) {
    // Exercises distance != 0 header access.
    DEQUE(int) q = new_deque(int);
    for (int i = 0; i < 5; i++) {
        push_back(q, 7);
    }
    for (int i = 0; i < 3; i++) {
        push_front(q, 7);
    }
    push_front(q, 99);

    int seven = 7;
    int ninetynine = 99;
    TEST_ASSERT_EQUAL_UINT64(8, __cstl_deque_count((void **) &q, &seven));
    TEST_ASSERT_EQUAL_UINT64(1, __cstl_deque_count((void **) &q, &ninetynine));

    destroy(q);
}

static void test_deque_count_empty(void) {
    DEQUE(int) q = new_deque(int);
    int zero = 0;
    TEST_ASSERT_EQUAL_UINT64(0, __cstl_deque_count((void **) &q, &zero));
    destroy(q);
}

static void test_deque_count_if_basic(void) {
    int vals[] = {1, 2, 3, 4, 5, 6};
    DEQUE(int) q = make_int_deque(vals, sizeof(vals) / sizeof(vals[0]));

    TEST_ASSERT_EQUAL_UINT64(3, __cstl_deque_count_if((void **) &q, pred_even));
    TEST_ASSERT_EQUAL_UINT64(0, __cstl_deque_count_if((void **) &q, pred_negative));

    destroy(q);
}

static void test_deque_count_if_after_push_front(void) {
    DEQUE(int) q = new_deque(int);
    for (int i = 1; i <= 4; i++) push_back(q, i);    // 1 2 3 4
    for (int i = 5; i <= 8; i++) push_front(q, -i);  // -8 -7 -6 -5 1 2 3 4

    TEST_ASSERT_EQUAL_UINT64(4, __cstl_deque_count_if((void **) &q, pred_negative));
    TEST_ASSERT_EQUAL_UINT64(4, __cstl_deque_count_if((void **) &q, pred_even));

    destroy(q);
}

static void test_deque_lower_bound_found(void) {
    int vals[] = {1, 3, 5, 7, 9};
    DEQUE(int) q = make_int_deque(vals, 5);

    int key = 5;
    int *it = (int *) __cstl_deque_lower_bound((void **) &q, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(5, *it);
    TEST_ASSERT_EQUAL_PTR(&q[2], it);

    destroy(q);
}

static void test_deque_lower_bound_missing_rounds_up(void) {
    int vals[] = {1, 3, 5, 7, 9};
    DEQUE(int) q = make_int_deque(vals, 5);

    int key = 4;
    int *it = (int *) __cstl_deque_lower_bound((void **) &q, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(5, *it);

    destroy(q);
}

static void test_deque_lower_bound_beyond_end_returns_null(void) {
    int vals[] = {1, 3, 5};
    DEQUE(int) q = make_int_deque(vals, 3);

    int key = 100;
    int *it = (int *) __cstl_deque_lower_bound((void **) &q, &key, cmp_int);
    TEST_ASSERT_NULL(it);

    destroy(q);
}

static void test_deque_lower_bound_empty(void) {
    DEQUE(int) q = new_deque(int);
    int key = 1;
    TEST_ASSERT_NULL(__cstl_deque_lower_bound((void **) &q, &key, cmp_int));
    destroy(q);
}

static void test_deque_upper_bound_found(void) {
    int vals[] = {1, 3, 5, 7, 9};
    DEQUE(int) q = make_int_deque(vals, 5);

    int key = 5;
    int *it = (int *) __cstl_deque_upper_bound((void **) &q, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(7, *it);

    destroy(q);
}

static void test_deque_upper_bound_beyond_end_returns_null(void) {
    int vals[] = {1, 3, 5};
    DEQUE(int) q = make_int_deque(vals, 3);

    int key = 5;
    int *it = (int *) __cstl_deque_upper_bound((void **) &q, &key, cmp_int);
    TEST_ASSERT_NULL(it);

    destroy(q);
}

static int cmp_int_desc(const void *a, const void *b) {
    int x = *(const int *) a;
    int y = *(const int *) b;
    return (y > x) - (y < x);
}

static void test_deque_max_element_default_cmp(void) {
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    DEQUE(int) q = make_int_deque(vals, sizeof(vals) / sizeof(vals[0]));

    int *it = (int *) max_element(q);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(9, *it);

    destroy(q);
}

static void test_deque_max_element_custom_cmp(void) {
    // With descending compare, "max" under that order is the smallest value.
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    DEQUE(int) q = make_int_deque(vals, sizeof(vals) / sizeof(vals[0]));

    int *it = (int *) max_element(q, cmp_int_desc);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(1, *it);

    destroy(q);
}

static void test_deque_min_element_default_cmp(void) {
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    DEQUE(int) q = make_int_deque(vals, sizeof(vals) / sizeof(vals[0]));

    int *it = (int *) min_element(q);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(1, *it);

    destroy(q);
}

static void test_deque_min_element_custom_cmp(void) {
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    DEQUE(int) q = make_int_deque(vals, sizeof(vals) / sizeof(vals[0]));

    int *it = (int *) min_element(q, cmp_int_desc);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(9, *it);

    destroy(q);
}

static void test_bitset_destroy_via_free(void) {
    // Regression: _cstl_free must detect BITSET by its `.bits` field
    // (not `*tmp` which reads `nbits`), otherwise destroy(b) crashes.
    BITSET b = new_bitset(50);
    bitset.set_at(b, 10, true);
    bitset.set_at(b, 0, true);
    TEST_ASSERT_EQUAL_UINT64(2, __cstl_bitset_count(b));
    destroy(b);
}

static void test_deque_min_max_after_push_front(void) {
    DEQUE(int) q = new_deque(int);
    for (int i = 5; i <= 9; i++) push_back(q, i);
    for (int i = 4; i >= 1; i--) push_front(q, i);   // 1..9

    int *hi = (int *) max_element(q);
    int *lo = (int *) min_element(q);
    TEST_ASSERT_EQUAL_INT(9, *hi);
    TEST_ASSERT_EQUAL_INT(1, *lo);

    destroy(q);
}

static void test_deque_bounds_after_push_front(void) {
    // Build a sorted deque with non-zero distance.
    DEQUE(int) q = new_deque(int);
    for (int i = 5; i <= 9; i++) push_back(q, i);    // 5 6 7 8 9
    for (int i = 4; i >= 1; i--) push_front(q, i);   // 1 2 3 4 5 6 7 8 9

    int key = 5;
    int *lo = (int *) __cstl_deque_lower_bound((void **) &q, &key, cmp_int);
    int *hi = (int *) __cstl_deque_upper_bound((void **) &q, &key, cmp_int);
    TEST_ASSERT_NOT_NULL(lo);
    TEST_ASSERT_NOT_NULL(hi);
    TEST_ASSERT_EQUAL_INT(5, *lo);
    TEST_ASSERT_EQUAL_INT(6, *hi);
    TEST_ASSERT_EQUAL_PTR(&q[4], lo);
    TEST_ASSERT_EQUAL_PTR(&q[5], hi);

    destroy(q);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_deque_count_basic);
    RUN_TEST(test_deque_count_after_push_front);
    RUN_TEST(test_deque_count_empty);
    RUN_TEST(test_deque_count_if_basic);
    RUN_TEST(test_deque_count_if_after_push_front);
    RUN_TEST(test_deque_lower_bound_found);
    RUN_TEST(test_deque_lower_bound_missing_rounds_up);
    RUN_TEST(test_deque_lower_bound_beyond_end_returns_null);
    RUN_TEST(test_deque_lower_bound_empty);
    RUN_TEST(test_deque_upper_bound_found);
    RUN_TEST(test_deque_upper_bound_beyond_end_returns_null);
    RUN_TEST(test_deque_bounds_after_push_front);
    RUN_TEST(test_deque_max_element_default_cmp);
    RUN_TEST(test_deque_max_element_custom_cmp);
    RUN_TEST(test_deque_min_element_default_cmp);
    RUN_TEST(test_deque_min_element_custom_cmp);
    RUN_TEST(test_deque_min_max_after_push_front);
    RUN_TEST(test_bitset_destroy_via_free);
    return UNITY_END();
}
