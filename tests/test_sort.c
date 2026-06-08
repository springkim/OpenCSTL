#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

#include <stdbool.h>

void setUp(void) {}
void tearDown(void) {}

static int cmp_int_asc(const void *a, const void *b) {
    int x = *(const int *) a, y = *(const int *) b;
    return (x > y) - (x < y);
}

static int cmp_int_desc(const void *a, const void *b) {
    int x = *(const int *) a, y = *(const int *) b;
    return (x < y) - (x > y);
}

static void test_sort_vector_default_ascending(void) {
    VECTOR(int) v = new_vector(int);
    int vals[] = {5, 2, 8, 1, 9, 3};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) {
        push_back(v, vals[i]);
    }
    sort(v);
    TEST_ASSERT_EQUAL_INT(1, v[0]);
    TEST_ASSERT_EQUAL_INT(2, v[1]);
    TEST_ASSERT_EQUAL_INT(3, v[2]);
    TEST_ASSERT_EQUAL_INT(5, v[3]);
    TEST_ASSERT_EQUAL_INT(8, v[4]);
    TEST_ASSERT_EQUAL_INT(9, v[5]);
    destroy(v);
}

static void test_sort_vector_custom_descending(void) {
    VECTOR(int) v = new_vector(int);
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) {
        push_back(v, vals[i]);
    }
    sort(v, cmp_int_desc);
    TEST_ASSERT_EQUAL_INT(9, v[0]);
    TEST_ASSERT_EQUAL_INT(6, v[1]);
    TEST_ASSERT_EQUAL_INT(5, v[2]);
    TEST_ASSERT_EQUAL_INT(1, v[size(v) - 1]);
    destroy(v);
}

static void test_sort_vector_already_sorted(void) {
    VECTOR(int) v = new_vector(int);
    for (int i = 0; i < 20; i++) push_back(v, i);
    sort(v);
    for (int i = 0; i < 20; i++) {
        TEST_ASSERT_EQUAL_INT(i, v[i]);
    }
    destroy(v);
}

static void test_sort_vector_reverse_input(void) {
    VECTOR(int) v = new_vector(int);
    for (int i = 100; i > 0; i--) push_back(v, i);
    sort(v);
    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_INT(i + 1, v[i]);
    }
    destroy(v);
}

static void test_sort_vector_with_duplicates(void) {
    VECTOR(int) v = new_vector(int);
    int vals[] = {3, 1, 3, 2, 1, 2, 3, 1};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) {
        push_back(v, vals[i]);
    }
    sort(v);
    TEST_ASSERT_EQUAL_INT(1, v[0]);
    TEST_ASSERT_EQUAL_INT(1, v[1]);
    TEST_ASSERT_EQUAL_INT(1, v[2]);
    TEST_ASSERT_EQUAL_INT(2, v[3]);
    TEST_ASSERT_EQUAL_INT(3, v[size(v) - 1]);
    destroy(v);
}

static void test_sort_vector_single_element(void) {
    VECTOR(int) v = new_vector(int);
    push_back(v, 42);
    sort(v);
    TEST_ASSERT_EQUAL_INT(1, size(v));
    TEST_ASSERT_EQUAL_INT(42, v[0]);
    destroy(v);
}

static void test_sort_vector_empty(void) {
    VECTOR(int) v = new_vector(int);
    sort(v);
    TEST_ASSERT_EQUAL_INT(0, size(v));
    destroy(v);
}

static void test_is_sorted_vector_true(void) {
    VECTOR(int) v = new_vector(int);
    int vals[] = {1, 2, 3, 4, 5};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) push_back(v, vals[i]);
    TEST_ASSERT_EQUAL_INT(1, is_sorted(v, cmp_int_asc));
    destroy(v);
}

static void test_is_sorted_vector_false(void) {
    VECTOR(int) v = new_vector(int);
    int vals[] = {1, 2, 5, 3, 4};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) push_back(v, vals[i]);
    TEST_ASSERT_EQUAL_INT(0, is_sorted(v, cmp_int_asc));
    destroy(v);
}

static void test_stable_sort_vector(void) {
    VECTOR(int) v = new_vector(int);
    int vals[] = {4, 2, 4, 1, 3, 2};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) push_back(v, vals[i]);
    stable_sort(v, cmp_int_asc);
    TEST_ASSERT_EQUAL_INT(1, v[0]);
    TEST_ASSERT_EQUAL_INT(2, v[1]);
    TEST_ASSERT_EQUAL_INT(2, v[2]);
    TEST_ASSERT_EQUAL_INT(3, v[3]);
    TEST_ASSERT_EQUAL_INT(4, v[4]);
    TEST_ASSERT_EQUAL_INT(4, v[5]);
    destroy(v);
}

static void test_sort_list(void) {
    LIST(int) l = new_list(int);
    int vals[] = {7, 3, 1, 9, 5};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) push_back(l, vals[i]);
    sort(l, cmp_int_asc);
    TEST_ASSERT_EQUAL_INT(1, is_sorted(l, cmp_int_asc));
    destroy(l);
}

static void test_sort_deque(void) {
    DEQUE(int) d = new_deque(int);
    int vals[] = {5, 1, 4, 2, 3};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) push_back(d, vals[i]);
    sort(d, cmp_int_asc);
    TEST_ASSERT_EQUAL_INT(1, is_sorted(d, cmp_int_asc));
    destroy(d);
}

static void test_sort_large_random_ish(void) {
    VECTOR(int) v = new_vector(int);
    const int N = 1000;
    int seed = 12345;
    for (int i = 0; i < N; i++) {
        seed = seed * 1103515245 + 12345;
        push_back(v, (seed >> 16) & 0x7fff);
    }
    sort(v, cmp_int_asc);
    TEST_ASSERT_EQUAL_INT(1, is_sorted(v, cmp_int_asc));
    destroy(v);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_sort_vector_default_ascending);
    RUN_TEST(test_sort_vector_custom_descending);
    RUN_TEST(test_sort_vector_already_sorted);
    RUN_TEST(test_sort_vector_reverse_input);
    RUN_TEST(test_sort_vector_with_duplicates);
    RUN_TEST(test_sort_vector_single_element);
    RUN_TEST(test_sort_vector_empty);
    RUN_TEST(test_is_sorted_vector_true);
    RUN_TEST(test_is_sorted_vector_false);
    RUN_TEST(test_stable_sort_vector);
    RUN_TEST(test_sort_list);
    RUN_TEST(test_sort_deque);
    RUN_TEST(test_sort_large_random_ish);
    return UNITY_END();
}
