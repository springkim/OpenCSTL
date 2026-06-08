#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static void test_string_substr(void) {
    char *r = string.substr("hello world", 6, 5);
    TEST_ASSERT_EQUAL_STRING("world", r);
    free(r);
}

static void test_string_split_basic(void) {
    int n = 0;
    char **parts = string.split("a,b,c,d", ",", &n);
    TEST_ASSERT_EQUAL_INT(4, n);
    TEST_ASSERT_EQUAL_STRING("a", parts[0]);
    TEST_ASSERT_EQUAL_STRING("b", parts[1]);
    TEST_ASSERT_EQUAL_STRING("c", parts[2]);
    TEST_ASSERT_EQUAL_STRING("d", parts[3]);
    free(parts);
}

static void test_string_split_no_separator(void) {
    int n = 0;
    char **parts = string.split("hello", ",", &n);
    TEST_ASSERT_EQUAL_INT(1, n);
    TEST_ASSERT_EQUAL_STRING("hello", parts[0]);
    free(parts);
}

static void test_string_replace(void) {
    char *r = string.replace("foo bar foo", "foo", "baz");
    TEST_ASSERT_EQUAL_STRING("baz bar baz", r);
    free(r);
}

static void test_string_replace_with_shorter(void) {
    char *r = string.replace("aaaa", "aa", "x");
    TEST_ASSERT_EQUAL_STRING("xx", r);
    free(r);
}

static void test_string_ltrim(void) {
    char *r = string.ltrim("   hello");
    TEST_ASSERT_EQUAL_STRING("hello", r);
    free(r);
}

static void test_string_rtrim(void) {
    char *r = string.rtrim("hello   ");
    TEST_ASSERT_EQUAL_STRING("hello", r);
    free(r);
}

static void test_string_trim_both_sides(void) {
    char *r = string.trim("   hello world   ");
    TEST_ASSERT_EQUAL_STRING("hello world", r);
    free(r);
}

static void test_string_to_upper(void) {
    char *r = string.to_upper("Hello World 123");
    TEST_ASSERT_EQUAL_STRING("HELLO WORLD 123", r);
    free(r);
}

static void test_string_to_lower(void) {
    char *r = string.to_lower("Hello World 123");
    TEST_ASSERT_EQUAL_STRING("hello world 123", r);
    free(r);
}

static void test_string_starts_with(void) {
    TEST_ASSERT_TRUE(string.starts_with("hello world", "hello"));
    TEST_ASSERT_FALSE(string.starts_with("hello world", "world"));
}

static void test_string_ends_with(void) {
    TEST_ASSERT_TRUE(string.ends_with("hello world", "world"));
    TEST_ASSERT_FALSE(string.ends_with("hello world", "hello"));
}

static void test_string_count_substr(void) {
    TEST_ASSERT_EQUAL_INT(3, string.count_str("ababab", "ab"));
    TEST_ASSERT_EQUAL_INT(0, string.count_str("hello", "xyz"));
}

static void test_string_concat(void) {
    char *r = string.concat("foo", "bar");
    TEST_ASSERT_EQUAL_STRING("foobar", r);
    free(r);
}

static void test_string_join(void) {
    char *parts[] = {"a", "b", "c"};
    char *r = string.join(parts, 3, "-");
    TEST_ASSERT_EQUAL_STRING("a-b-c", r);
    free(r);
}

static void test_string_reverse(void) {
    char *r = string.reverse("abcdef");
    TEST_ASSERT_EQUAL_STRING("fedcba", r);
    free(r);
}

static void test_string_predicates(void) {
    TEST_ASSERT_TRUE(string.is_digit("12345"));
    TEST_ASSERT_FALSE(string.is_digit("12a45"));

    TEST_ASSERT_TRUE(string.is_alpha("hello"));
    TEST_ASSERT_FALSE(string.is_alpha("hel1o"));

    TEST_ASSERT_TRUE(string.is_alnum("abc123"));
    TEST_ASSERT_FALSE(string.is_alnum("abc 123"));

    TEST_ASSERT_TRUE(string.is_space("   \t\n"));
    TEST_ASSERT_FALSE(string.is_space("  a "));
}

static void test_string_kmp_finds_all_matches(void) {
    int count = 0;
    int *positions = string.kmp("ababcababab", "ab", &count);
    TEST_ASSERT_EQUAL_INT(5, count);
    TEST_ASSERT_NOT_NULL(positions);
    TEST_ASSERT_EQUAL_INT(0, positions[0]);
    TEST_ASSERT_EQUAL_INT(2, positions[1]);
    TEST_ASSERT_EQUAL_INT(5, positions[2]);
    TEST_ASSERT_EQUAL_INT(7, positions[3]);
    TEST_ASSERT_EQUAL_INT(9, positions[4]);
    free(positions);
}

static void test_string_kmp_no_match(void) {
    int count = -1;
    int *positions = string.kmp("hello world", "xyz", &count);
    TEST_ASSERT_EQUAL_INT(0, count);
    TEST_ASSERT_NULL(positions);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_string_substr);
    RUN_TEST(test_string_split_basic);
    RUN_TEST(test_string_split_no_separator);
    RUN_TEST(test_string_replace);
    RUN_TEST(test_string_replace_with_shorter);
    RUN_TEST(test_string_ltrim);
    RUN_TEST(test_string_rtrim);
    RUN_TEST(test_string_trim_both_sides);
    RUN_TEST(test_string_to_upper);
    RUN_TEST(test_string_to_lower);
    RUN_TEST(test_string_starts_with);
    RUN_TEST(test_string_ends_with);
    RUN_TEST(test_string_count_substr);
    RUN_TEST(test_string_concat);
    RUN_TEST(test_string_join);
    RUN_TEST(test_string_reverse);
    RUN_TEST(test_string_predicates);
    RUN_TEST(test_string_kmp_finds_all_matches);
    RUN_TEST(test_string_kmp_no_match);
    return UNITY_END();
}
