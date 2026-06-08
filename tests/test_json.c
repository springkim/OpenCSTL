#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static char *dup_str(const char *s) {
    size_t n = strlen(s) + 1;
    char *p = (char *) malloc(n);
    memcpy(p, s, n);
    return p;
}

static void test_json_parse_int(void) {
    char *src = dup_str("{\"age\": 42}");
    JSON *root = json.parse(src);
    TEST_ASSERT_EQUAL_INT(42, json.get(root, "age")->as_int());
    destroy(root);
    free(src);
}

static void test_json_parse_string(void) {
    char *src = dup_str("{\"name\": \"alice\"}");
    JSON *root = json.parse(src);
    TEST_ASSERT_EQUAL_STRING("alice", json.get(root, "name")->as_string());
    destroy(root);
    free(src);
}

static void test_json_parse_bool(void) {
    char *src = dup_str("{\"a\": true, \"b\": false}");
    JSON *root = json.parse(src);
    TEST_ASSERT_TRUE(json.get(root, "a")->as_bool());
    TEST_ASSERT_FALSE(json.get(root, "b")->as_bool());
    destroy(root);
    free(src);
}

static void test_json_parse_null(void) {
    char *src = dup_str("{\"x\": null, \"y\": 1}");
    JSON *root = json.parse(src);
    TEST_ASSERT_TRUE(json.get(root, "x")->is_null());
    TEST_ASSERT_FALSE(json.get(root, "y")->is_null());
    destroy(root);
    free(src);
}

static void test_json_parse_double(void) {
    char *src = dup_str("{\"pi\": 3.14159}");
    JSON *root = json.parse(src);
    TEST_ASSERT_DOUBLE_WITHIN(1e-6, 3.14159, json.get(root, "pi")->as_double());
    destroy(root);
    free(src);
}

static void test_json_parse_negative_number(void) {
    char *src = dup_str("{\"n\": -123}");
    JSON *root = json.parse(src);
    TEST_ASSERT_EQUAL_INT(-123, json.get(root, "n")->as_int());
    destroy(root);
    free(src);
}

static void test_json_array_index(void) {
    char *src = dup_str("{\"arr\": [10, 20, 30, 40]}");
    JSON *root = json.parse(src);
    TEST_ASSERT_EQUAL_INT(10, json.get(root, "arr.0")->as_int());
    TEST_ASSERT_EQUAL_INT(20, json.get(root, "arr.1")->as_int());
    TEST_ASSERT_EQUAL_INT(40, json.get(root, "arr.3")->as_int());
    destroy(root);
    free(src);
}

static void test_json_nested_object(void) {
    char *src = dup_str("{\"user\": {\"name\": \"bob\", \"id\": 7}}");
    JSON *root = json.parse(src);
    TEST_ASSERT_EQUAL_STRING("bob", json.get(root, "user.name")->as_string());
    TEST_ASSERT_EQUAL_INT(7,        json.get(root, "user.id")->as_int());
    destroy(root);
    free(src);
}

static void test_json_array_of_objects(void) {
    char *src = dup_str("{\"items\":[{\"v\":1},{\"v\":2},{\"v\":3}]}");
    JSON *root = json.parse(src);
    TEST_ASSERT_EQUAL_INT(1, json.get(root, "items.0.v")->as_int());
    TEST_ASSERT_EQUAL_INT(2, json.get(root, "items.1.v")->as_int());
    TEST_ASSERT_EQUAL_INT(3, json.get(root, "items.2.v")->as_int());
    destroy(root);
    free(src);
}

static void test_json_string_with_escapes(void) {
    char *src = dup_str("{\"s\": \"a\\nb\\tc\"}");
    JSON *root = json.parse(src);
    TEST_ASSERT_EQUAL_STRING("a\nb\tc", json.get(root, "s")->as_string());
    destroy(root);
    free(src);
}

static void test_json_missing_key_returns_null(void) {
    char *src = dup_str("{\"a\": 1}");
    JSON *root = json.parse(src);
    TEST_ASSERT_NULL(json.get(root, "nonexistent"));
    destroy(root);
    free(src);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_json_parse_int);
    RUN_TEST(test_json_parse_string);
    RUN_TEST(test_json_parse_bool);
    RUN_TEST(test_json_parse_null);
    RUN_TEST(test_json_parse_double);
    RUN_TEST(test_json_parse_negative_number);
    RUN_TEST(test_json_array_index);
    RUN_TEST(test_json_nested_object);
    RUN_TEST(test_json_array_of_objects);
    RUN_TEST(test_json_string_with_escapes);
    RUN_TEST(test_json_missing_key_returns_null);
    return UNITY_END();
}
