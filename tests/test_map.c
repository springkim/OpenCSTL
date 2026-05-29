#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_map_new_is_empty(void) {
    MAP(int) m = new_map(int, double);
    TEST_ASSERT_EQUAL_INT(0, size(m));
    destroy(m);
}

static void test_map_insert_increases_size(void) {
    MAP(int) m = new_map(int, double);
    insert(m, 1, 1.5);
    insert(m, 2, 2.5);
    insert(m, 3, 3.5);
    TEST_ASSERT_EQUAL_INT(3, size(m));
    destroy(m);
}

static void test_map_find_returns_key_and_value(void) {
    MAP(int) m = new_map(int, double);
    insert(m, 10, 100.0);
    insert(m, 20, 200.0);

    int *it = find(m, 20);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(20, first(it));
    TEST_ASSERT_EQUAL_DOUBLE(200.0, second(it, double));
    destroy(m);
}

static void test_map_find_absent_returns_null(void) {
    MAP(int) m = new_map(int, double);
    insert(m, 1, 1.0);
    int *it = find(m, 999);
    TEST_ASSERT_NULL(it);
    destroy(m);
}

static void test_map_iteration_keys_sorted(void) {
    MAP(int) m = new_map(int, double);
    int keys[] = {5, 1, 4, 2, 3};
    for (size_t i = 0; i < sizeof(keys)/sizeof(keys[0]); i++) {
        insert(m, keys[i], (double) keys[i] * 10.0);
    }

    int prev_k = 0;
    double seen_sum = 0.0;
    int count = 0;
    for (int *it = begin(m); it != end(m); it = next(it)) {
        TEST_ASSERT_TRUE(first(it) > prev_k);
        prev_k = first(it);
        seen_sum += second(it, double);
        count++;
    }
    TEST_ASSERT_EQUAL_INT(5, count);
    TEST_ASSERT_EQUAL_DOUBLE(150.0, seen_sum);
    destroy(m);
}

static void test_map_value_associativity(void) {
    MAP(int) m = new_map(int, double);
    for (int i = 0; i < 20; i++) {
        insert(m, i, (double) i * i);
    }

    for (int k = 0; k < 20; k++) {
        int *it = find(m, k);
        TEST_ASSERT_NOT_NULL(it);
        TEST_ASSERT_EQUAL_INT(k, first(it));
        TEST_ASSERT_EQUAL_DOUBLE((double) k * k, second(it, double));
    }
    destroy(m);
}

static void test_map_many_insertions(void) {
    MAP(int) m = new_map(int, double);
    for (int i = 0; i < 100; i++) {
        insert(m, i, (double) i * 0.5);
    }
    TEST_ASSERT_EQUAL_INT(100, size(m));

    int *it = find(m, 50);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_DOUBLE(25.0, second(it, double));
    destroy(m);
}

static void test_map_duplicate_key_size_stays_one(void) {
    // After the rbtree memcmp(key_size) fix, duplicate-key insert is a no-op.
    MAP(int) m = new_map(int, double);
    insert(m, 42, 1.0);
    insert(m, 42, 2.0);
    TEST_ASSERT_EQUAL_INT(1, size(m));
    destroy(m);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_map_new_is_empty);
    RUN_TEST(test_map_insert_increases_size);
    RUN_TEST(test_map_find_returns_key_and_value);
    RUN_TEST(test_map_find_absent_returns_null);
    RUN_TEST(test_map_iteration_keys_sorted);
    RUN_TEST(test_map_value_associativity);
    RUN_TEST(test_map_many_insertions);
    RUN_TEST(test_map_duplicate_key_size_stays_one);
    return UNITY_END();
}
