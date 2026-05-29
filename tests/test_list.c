#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static int first_value(LIST(int) l) {
    return *(int *) begin(l);
}

static int last_value(LIST(int) l) {
    return *(int *) rbegin(l);
}

static void test_list_new_is_empty(void) {
    LIST(int) l = new_list(int);
    TEST_ASSERT_EQUAL_INT(0, size(l));
    destroy(l);
}

static void test_list_push_back_increases_size(void) {
    LIST(int) l = new_list(int);
    push_back(l, 1);
    push_back(l, 2);
    push_back(l, 3);
    TEST_ASSERT_EQUAL_INT(3, size(l));
    destroy(l);
}

static void test_list_first_last_after_push_back(void) {
    LIST(int) l = new_list(int);
    push_back(l, 10);
    push_back(l, 20);
    push_back(l, 30);
    TEST_ASSERT_EQUAL_INT(10, first_value(l));
    TEST_ASSERT_EQUAL_INT(30, last_value(l));
    destroy(l);
}

static void test_list_push_front(void) {
    LIST(int) l = new_list(int);
    push_back(l, 2);
    push_front(l, 1);
    push_front(l, 0);
    TEST_ASSERT_EQUAL_INT(3, size(l));
    TEST_ASSERT_EQUAL_INT(0, first_value(l));
    TEST_ASSERT_EQUAL_INT(2, last_value(l));
    destroy(l);
}

static void test_list_pop_back(void) {
    LIST(int) l = new_list(int);
    push_back(l, 1);
    push_back(l, 2);
    push_back(l, 3);
    pop_back(l);
    TEST_ASSERT_EQUAL_INT(2, size(l));
    TEST_ASSERT_EQUAL_INT(2, last_value(l));
    destroy(l);
}

static void test_list_pop_front(void) {
    LIST(int) l = new_list(int);
    push_back(l, 1);
    push_back(l, 2);
    push_back(l, 3);
    pop_front(l);
    TEST_ASSERT_EQUAL_INT(2, size(l));
    TEST_ASSERT_EQUAL_INT(2, first_value(l));
    destroy(l);
}

static void test_list_iteration_forward(void) {
    LIST(int) l = new_list(int);
    for (int i = 1; i <= 5; i++) push_back(l, i);

    int sum = 0;
    for (int *it = begin(l); it != end(l); it = next(it)) {
        sum += *it;
    }
    TEST_ASSERT_EQUAL_INT(15, sum);
    destroy(l);
}

static void test_list_iteration_reverse(void) {
    LIST(int) l = new_list(int);
    push_back(l, 1);
    push_back(l, 2);
    push_back(l, 3);

    int seq[3], idx = 0;
    for (int *it = rbegin(l); it != rend(l); it = prev(it)) {
        seq[idx++] = *it;
    }
    TEST_ASSERT_EQUAL_INT(3, seq[0]);
    TEST_ASSERT_EQUAL_INT(2, seq[1]);
    TEST_ASSERT_EQUAL_INT(1, seq[2]);
    destroy(l);
}

static void test_list_alternating_push(void) {
    LIST(int) l = new_list(int);
    push_back(l, 10);
    push_front(l, 5);
    push_back(l, 20);
    push_front(l, 1);
    // expected order: 1 5 10 20
    TEST_ASSERT_EQUAL_INT(4, size(l));
    TEST_ASSERT_EQUAL_INT(1,  first_value(l));
    TEST_ASSERT_EQUAL_INT(20, last_value(l));

    int expected[] = {1, 5, 10, 20};
    int idx = 0;
    for (int *it = begin(l); it != end(l); it = next(it)) {
        TEST_ASSERT_EQUAL_INT(expected[idx++], *it);
    }
    destroy(l);
}

static void test_list_single_element_pop(void) {
    LIST(int) l = new_list(int);
    push_back(l, 42);
    TEST_ASSERT_EQUAL_INT(42, first_value(l));
    pop_back(l);
    TEST_ASSERT_EQUAL_INT(0, size(l));
    destroy(l);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_list_new_is_empty);
    RUN_TEST(test_list_push_back_increases_size);
    RUN_TEST(test_list_first_last_after_push_back);
    RUN_TEST(test_list_push_front);
    RUN_TEST(test_list_pop_back);
    RUN_TEST(test_list_pop_front);
    RUN_TEST(test_list_iteration_forward);
    RUN_TEST(test_list_iteration_reverse);
    RUN_TEST(test_list_alternating_push);
    RUN_TEST(test_list_single_element_pop);
    return UNITY_END();
}
