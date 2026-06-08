#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

void setUp(void) {}
void tearDown(void) {}

static int cmp_int_desc(const void *a, const void *b) {
    int x = *(const int *) a, y = *(const int *) b;
    return (x < y) - (x > y);
}

static void test_queue_new_is_empty(void) {
    QUEUE(int) q = new_queue(int);
    TEST_ASSERT_EQUAL_INT(0, size(q));
    TEST_ASSERT_TRUE(empty(q));
    destroy(q);
}

static void test_queue_push_then_front(void) {
    QUEUE(int) q = new_queue(int);
    push(q, 7);
    TEST_ASSERT_EQUAL_INT(7, front(q));
    TEST_ASSERT_EQUAL_INT(1, size(q));
    destroy(q);
}

static void test_queue_fifo_order(void) {
    QUEUE(int) q = new_queue(int);
    for (int i = 1; i <= 5; i++) push(q, i * 10);

    TEST_ASSERT_EQUAL_INT(5, size(q));
    for (int i = 1; i <= 5; i++) {
        TEST_ASSERT_EQUAL_INT(i * 10, front(q));
        pop(q);
    }
    TEST_ASSERT_EQUAL_INT(0, size(q));
    destroy(q);
}

static void test_queue_drain_and_refill(void) {
    QUEUE(int) q = new_queue(int);
    for (int i = 0; i < 50; i++) push(q, i);
    for (int i = 0; i < 30; i++) pop(q);
    TEST_ASSERT_EQUAL_INT(20, size(q));
    TEST_ASSERT_EQUAL_INT(30, front(q));

    for (int i = 100; i < 110; i++) push(q, i);
    TEST_ASSERT_EQUAL_INT(30, size(q));
    TEST_ASSERT_EQUAL_INT(30, front(q));
    destroy(q);
}

static void test_priority_queue_max_heap_default(void) {
    QUEUE(int) pq = new_priority_queue(int);
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) push(pq, vals[i]);

    int sorted[] = {9, 6, 5, 4, 3, 2, 1, 1};
    for (size_t i = 0; i < sizeof(sorted) / sizeof(sorted[0]); i++) {
        TEST_ASSERT_EQUAL_INT(sorted[i], top(pq));
        pop(pq);
    }
    TEST_ASSERT_TRUE(empty(pq));
    destroy(pq);
}

static void test_priority_queue_min_heap_with_cmp(void) {
    QUEUE(int) pq = new_priority_queue(int, cmp_int_desc);
    int vals[] = {3, 1, 4, 1, 5, 9, 2, 6};
    for (size_t i = 0; i < sizeof(vals) / sizeof(vals[0]); i++) push(pq, vals[i]);

    int sorted[] = {1, 1, 2, 3, 4, 5, 6, 9};
    for (size_t i = 0; i < sizeof(sorted) / sizeof(sorted[0]); i++) {
        TEST_ASSERT_EQUAL_INT(sorted[i], top(pq));
        pop(pq);
    }
    destroy(pq);
}

static void test_priority_queue_single_element(void) {
    QUEUE(int) pq = new_priority_queue(int);
    push(pq, 42);
    TEST_ASSERT_EQUAL_INT(42, top(pq));
    pop(pq);
    TEST_ASSERT_TRUE(empty(pq));
    destroy(pq);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_queue_new_is_empty);
    RUN_TEST(test_queue_push_then_front);
    RUN_TEST(test_queue_fifo_order);
    RUN_TEST(test_queue_drain_and_refill);
    RUN_TEST(test_priority_queue_max_heap_default);
    RUN_TEST(test_priority_queue_min_heap_with_cmp);
    RUN_TEST(test_priority_queue_single_element);
    return UNITY_END();
}
