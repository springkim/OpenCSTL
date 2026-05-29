#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

#include <stdbool.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static void test_bitset_new_is_all_zero(void) {
    BITSET b = new_bitset(20);
    TEST_ASSERT_EQUAL_UINT64(0, bitset.count(b));
    TEST_ASSERT_TRUE(bitset.none(b));
    TEST_ASSERT_FALSE(bitset.any(b));
    TEST_ASSERT_FALSE(bitset.all(b));
    destroy(b);
}

static void test_bitset_set_at_increases_count(void) {
    BITSET b = new_bitset(16);
    bitset.set_at(b, 0, true);
    bitset.set_at(b, 5, true);
    bitset.set_at(b, 15, true);
    TEST_ASSERT_EQUAL_UINT64(3, bitset.count(b));
    TEST_ASSERT_TRUE(bitset.test(b, 0));
    TEST_ASSERT_TRUE(bitset.test(b, 5));
    TEST_ASSERT_TRUE(bitset.test(b, 15));
    TEST_ASSERT_FALSE(bitset.test(b, 1));
    destroy(b);
}

static void test_bitset_set_at_false_clears(void) {
    BITSET b = new_bitset(8);
    bitset.set_at(b, 3, true);
    TEST_ASSERT_EQUAL_UINT64(1, bitset.count(b));
    bitset.set_at(b, 3, false);
    TEST_ASSERT_EQUAL_UINT64(0, bitset.count(b));
    TEST_ASSERT_FALSE(bitset.test(b, 3));
    destroy(b);
}

static void test_bitset_set_all(void) {
    BITSET b = new_bitset(13);
    bitset.set(b);
    TEST_ASSERT_TRUE(bitset.all(b));
    TEST_ASSERT_EQUAL_UINT64(13, bitset.count(b));
    destroy(b);
}

static void test_bitset_reset_all(void) {
    BITSET b = new_bitset(16);
    bitset.set(b);
    bitset.reset(b);
    TEST_ASSERT_TRUE(bitset.none(b));
    TEST_ASSERT_EQUAL_UINT64(0, bitset.count(b));
    destroy(b);
}

static void test_bitset_flip_all_toggles(void) {
    BITSET b = new_bitset(10);
    bitset.set_at(b, 2, true);
    bitset.set_at(b, 7, true);
    // bits set: 2 → after flip, 8 should be set
    bitset.flip(b);
    TEST_ASSERT_EQUAL_UINT64(8, bitset.count(b));
    TEST_ASSERT_FALSE(bitset.test(b, 2));
    TEST_ASSERT_FALSE(bitset.test(b, 7));
    TEST_ASSERT_TRUE(bitset.test(b, 0));
    destroy(b);
}

static void test_bitset_flip_at_toggles_single_bit(void) {
    BITSET b = new_bitset(8);
    bitset.flip_at(b, 4);
    TEST_ASSERT_TRUE(bitset.test(b, 4));
    bitset.flip_at(b, 4);
    TEST_ASSERT_FALSE(bitset.test(b, 4));
    destroy(b);
}

static void test_bitset_to_string_format(void) {
    BITSET b = new_bitset(8);
    bitset.set_at(b, 0, true);
    bitset.set_at(b, 3, true);
    bitset.set_at(b, 7, true);
    // MSB-first: bit7 bit6 ... bit0  →  "1 0 0 0 1 0 0 1"
    char *s = bitset.to_string(b);
    TEST_ASSERT_EQUAL_STRING("10001001", s);
    destroy(b);
}

static void test_bitset_nbits(void) {
    BITSET b = new_bitset(100);
    TEST_ASSERT_EQUAL_UINT64(100, bitset.nbits(b));
    destroy(b);
}

static void test_bitset_pattern_every_third(void) {
    BITSET b = new_bitset(30);
    for (size_t i = 0; i < 30; i += 3) {
        bitset.set_at(b, i, true);
    }
    TEST_ASSERT_EQUAL_UINT64(10, bitset.count(b));
    destroy(b);
}

static void test_bitset_destroy_via_free_regression(void) {
    // Regression: destroy(b) must route to bitset free, not container free.
    BITSET b = new_bitset(50);
    bitset.set_at(b, 0, true);
    bitset.set_at(b, 49, true);
    TEST_ASSERT_EQUAL_UINT64(2, bitset.count(b));
    destroy(b); // must not crash
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_bitset_new_is_all_zero);
    RUN_TEST(test_bitset_set_at_increases_count);
    RUN_TEST(test_bitset_set_at_false_clears);
    RUN_TEST(test_bitset_set_all);
    RUN_TEST(test_bitset_reset_all);
    RUN_TEST(test_bitset_flip_all_toggles);
    RUN_TEST(test_bitset_flip_at_toggles_single_bit);
    RUN_TEST(test_bitset_to_string_format);
    RUN_TEST(test_bitset_nbits);
    RUN_TEST(test_bitset_pattern_every_third);
    RUN_TEST(test_bitset_destroy_via_free_regression);
    return UNITY_END();
}
