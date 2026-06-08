#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static void test_stack_new_is_empty(void) {
    STACK(int) s = new_stack(int);
    TEST_ASSERT_EQUAL_INT(0, size(s));
    TEST_ASSERT_TRUE(empty(s));
    destroy(s);
}

static void test_stack_push_then_top(void) {
    STACK(int) s = new_stack(int);
    push(s, 10);
    TEST_ASSERT_EQUAL_INT(10, top(s));
    TEST_ASSERT_EQUAL_INT(1, size(s));
    destroy(s);
}

static void test_stack_lifo_order(void) {
    STACK(int) s = new_stack(int);
    for (int i = 1; i <= 5; i++) push(s, i);
    TEST_ASSERT_EQUAL_INT(5, size(s));

    int expected[] = {5, 4, 3, 2, 1};
    for (int i = 0; i < 5; i++) {
        TEST_ASSERT_EQUAL_INT(expected[i], top(s));
        pop(s);
    }
    TEST_ASSERT_EQUAL_INT(0, size(s));
    destroy(s);
}

static void test_stack_pop_until_empty(void) {
    STACK(int) s = new_stack(int);
    for (int i = 0; i < 100; i++) push(s, i * 2);
    TEST_ASSERT_EQUAL_INT(100, size(s));

    for (int i = 0; i < 100; i++) {
        TEST_ASSERT_EQUAL_INT((99 - i) * 2, top(s));
        pop(s);
    }
    TEST_ASSERT_TRUE(empty(s));
    destroy(s);
}

static void test_stack_push_after_pop(void) {
    STACK(int) s = new_stack(int);
    push(s, 1);
    push(s, 2);
    push(s, 3);
    pop(s); // remove 3
    TEST_ASSERT_EQUAL_INT(2, top(s));
    push(s, 99);
    TEST_ASSERT_EQUAL_INT(99, top(s));
    pop(s);
    TEST_ASSERT_EQUAL_INT(2, top(s));
    destroy(s);
}

static void test_stack_with_double_type(void) {
    STACK(double) s = new_stack(double);
    push(s, 1.5);
    push(s, 2.5);
    push(s, 3.5);
    TEST_ASSERT_EQUAL_DOUBLE(3.5, top(s));
    pop(s);
    TEST_ASSERT_EQUAL_DOUBLE(2.5, top(s));
    destroy(s);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_stack_new_is_empty);
    RUN_TEST(test_stack_push_then_top);
    RUN_TEST(test_stack_lifo_order);
    RUN_TEST(test_stack_pop_until_empty);
    RUN_TEST(test_stack_push_after_pop);
    RUN_TEST(test_stack_with_double_type);
    return UNITY_END();
}
