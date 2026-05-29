#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_set_new_is_empty(void) {
    SET(int) s = new_set(int);
    TEST_ASSERT_EQUAL_INT(0, size(s));
    destroy(s);
}

static void test_set_insert_increases_size(void) {
    SET(int) s = new_set(int);
    insert(s, 5);
    insert(s, 1);
    insert(s, 3);
    TEST_ASSERT_EQUAL_INT(3, size(s));
    destroy(s);
}

static void test_set_duplicate_insert_is_noop(void) {
    SET(int) s = new_set(int);
    insert(s, 7);
    insert(s, 7);
    insert(s, 7);
    TEST_ASSERT_EQUAL_INT(1, size(s));
    destroy(s);
}

static void test_set_find_present(void) {
    SET(int) s = new_set(int);
    insert(s, 10);
    insert(s, 20);
    insert(s, 30);
    int *it = find(s, 20);
    TEST_ASSERT_NOT_NULL(it);
    TEST_ASSERT_EQUAL_INT(20, *it);
    destroy(s);
}

static void test_set_find_absent_returns_null(void) {
    SET(int) s = new_set(int);
    insert(s, 1);
    insert(s, 2);
    int *it = find(s, 99);
    TEST_ASSERT_NULL(it);
    destroy(s);
}

static void test_set_erase_removes_element(void) {
    SET(int) s = new_set(int);
    insert(s, 1);
    insert(s, 2);
    insert(s, 3);

    int *it = find(s, 2);
    erase(s, it);
    TEST_ASSERT_EQUAL_INT(2, size(s));
    TEST_ASSERT_NULL(find(s, 2));
    destroy(s);
}

static void test_set_iteration_is_sorted(void) {
    SET(int) s = new_set(int);
    int input[] = {5, 1, 9, 3, 7, 2, 8, 4, 6};
    for (size_t i = 0; i < sizeof(input)/sizeof(input[0]); i++) {
        insert(s, input[i]);
    }

    int prev_val = -1;
    int count = 0;
    for (int *it = begin(s); it != end(s); it = next(it)) {
        TEST_ASSERT_TRUE(*it > prev_val);
        prev_val = *it;
        count++;
    }
    TEST_ASSERT_EQUAL_INT(9, count);
    destroy(s);
}

static void test_set_iteration_reverse_descending(void) {
    SET(int) s = new_set(int);
    for (int i = 1; i <= 5; i++) insert(s, i);

    int seq[5], idx = 0;
    for (int *it = rbegin(s); it != rend(s); it = prev(it)) {
        seq[idx++] = *it;
    }
    TEST_ASSERT_EQUAL_INT(5, seq[0]);
    TEST_ASSERT_EQUAL_INT(1, seq[4]);
    destroy(s);
}

static void test_set_many_insertions_and_erasures(void) {
    SET(int) s = new_set(int);
    for (int i = 0; i < 100; i++) insert(s, i);
    TEST_ASSERT_EQUAL_INT(100, size(s));

    for (int i = 50; i < 70; i++) {
        int *it = find(s, i);
        erase(s, it);
    }
    TEST_ASSERT_EQUAL_INT(80, size(s));
    TEST_ASSERT_NULL(find(s, 60));
    TEST_ASSERT_NOT_NULL(find(s, 49));
    TEST_ASSERT_NOT_NULL(find(s, 70));
    destroy(s);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_set_new_is_empty);
    RUN_TEST(test_set_insert_increases_size);
    RUN_TEST(test_set_duplicate_insert_is_noop);
    RUN_TEST(test_set_find_present);
    RUN_TEST(test_set_find_absent_returns_null);
    RUN_TEST(test_set_erase_removes_element);
    RUN_TEST(test_set_iteration_is_sorted);
    RUN_TEST(test_set_iteration_reverse_descending);
    RUN_TEST(test_set_many_insertions_and_erasures);
    return UNITY_END();
}
