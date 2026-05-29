#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_uset_new_is_empty(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    TEST_ASSERT_EQUAL_INT(0, size(h));
    destroy(h);
}

static void test_uset_insert_increases_size(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    insert(h, 1);
    insert(h, 2);
    insert(h, 3);
    TEST_ASSERT_EQUAL_INT(3, size(h));
    destroy(h);
}

static void test_uset_duplicate_insert_is_noop(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    insert(h, 42);
    insert(h, 42);
    insert(h, 42);
    TEST_ASSERT_EQUAL_INT(1, size(h));
    destroy(h);
}

static void test_uset_find_present(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    insert(h, 10);
    insert(h, 20);
    insert(h, 30);

    int *it = find(h, 20);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(20, *it);
    destroy(h);
}

static void test_uset_find_absent_returns_end(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    insert(h, 1);
    int *it = find(h, 999);
    TEST_ASSERT_EQUAL_PTR(end(h), it);
    destroy(h);
}

static void test_uset_erase(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    insert(h, 1);
    insert(h, 2);
    insert(h, 3);

    int *it = find(h, 2);
    erase(h, it);
    TEST_ASSERT_EQUAL_INT(2, size(h));
    TEST_ASSERT_EQUAL_PTR(end(h), find(h, 2));
    destroy(h);
}

static void test_uset_many_insertions_triggers_rehash(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    for (int i = 0; i < 1000; i++) insert(h, i);
    TEST_ASSERT_EQUAL_INT(1000, size(h));

    for (int i = 0; i < 1000; i += 100) {
        int *it = find(h, i);
        TEST_ASSERT_NOT_NULL(it);
        TEST_ASSERT_EQUAL_INT(i, *it);
    }
    destroy(h);
}

static void test_uset_clear(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    for (int i = 0; i < 50; i++) insert(h, i);
    TEST_ASSERT_EQUAL_INT(50, size(h));
    clear(h);
    TEST_ASSERT_EQUAL_INT(0, size(h));
    destroy(h);
}

static void test_uset_iteration_visits_every_element(void) {
    UNORDERED_SET(int) h = new_unordered_set(int);
    for (int i = 1; i <= 20; i++) insert(h, i);

    int sum = 0, count = 0;
    for (int *it = begin(h); it != end(h); it = next(it)) {
        sum += *it;
        count++;
    }
    TEST_ASSERT_EQUAL_INT(20, count);
    TEST_ASSERT_EQUAL_INT(210, sum); // 1+2+...+20
    destroy(h);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_uset_new_is_empty);
    RUN_TEST(test_uset_insert_increases_size);
    RUN_TEST(test_uset_duplicate_insert_is_noop);
    RUN_TEST(test_uset_find_present);
    RUN_TEST(test_uset_find_absent_returns_end);
    RUN_TEST(test_uset_erase);
    RUN_TEST(test_uset_many_insertions_triggers_rehash);
    RUN_TEST(test_uset_clear);
    RUN_TEST(test_uset_iteration_visits_every_element);
    return UNITY_END();
}
