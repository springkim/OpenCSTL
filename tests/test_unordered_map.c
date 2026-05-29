#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_umap_new_is_empty(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    TEST_ASSERT_EQUAL_INT(0, size(m));
    destroy(m);
}

static void test_umap_insert_increases_size(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    insert(m, 1, 1.5);
    insert(m, 2, 2.5);
    insert(m, 3, 3.5);
    TEST_ASSERT_EQUAL_INT(3, size(m));
    destroy(m);
}

static void test_umap_find_returns_key_value(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    insert(m, 10, 100.0);
    insert(m, 20, 200.0);

    int *it = find(m, 20);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(20, first(it));
    TEST_ASSERT_EQUAL_DOUBLE(200.0, second(it, double));
    destroy(m);
}

static void test_umap_find_absent_returns_null(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    insert(m, 1, 1.0);
    int *it = find(m, 999);
    TEST_ASSERT_NULL(it);
    destroy(m);
}

static void test_umap_erase(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    insert(m, 1, 1.0);
    insert(m, 2, 2.0);
    insert(m, 3, 3.0);

    int *it = find(m, 2);
    erase(m, it);
    TEST_ASSERT_EQUAL_INT(2, size(m));
    TEST_ASSERT_EQUAL_PTR(end(m), find(m, 2));
    destroy(m);
}

static void test_umap_duplicate_key_size_stays_one(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    insert(m, 42, 1.0);
    insert(m, 42, 2.0);
    TEST_ASSERT_EQUAL_INT(1, size(m));
    destroy(m);
}

static void test_umap_many_insertions(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    for (int i = 0; i < 500; i++) {
        insert(m, i, (double) i + 0.5);
    }
    TEST_ASSERT_EQUAL_INT(500, size(m));

    int *it = find(m, 250);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_DOUBLE(250.5, second(it, double));
    destroy(m);
}

static void test_umap_clear(void) {
    UNORDERED_MAP(int) m = new_unordered_map(int, double);
    for (int i = 0; i < 30; i++) insert(m, i, (double) i);
    clear(m);
    TEST_ASSERT_EQUAL_INT(0, size(m));
    destroy(m);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_umap_new_is_empty);
    RUN_TEST(test_umap_insert_increases_size);
    RUN_TEST(test_umap_find_returns_key_value);
    RUN_TEST(test_umap_find_absent_returns_null);
    RUN_TEST(test_umap_erase);
    RUN_TEST(test_umap_duplicate_key_size_stays_one);
    RUN_TEST(test_umap_many_insertions);
    RUN_TEST(test_umap_clear);
    return UNITY_END();
}
