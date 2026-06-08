#define OPENCSTL_TRACER
#include "opencstl/opencstl.h"
#include "unity.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

static const char *TMP_CSV = "test_csv_tmp.csv";

static void write_tmp(const char *path, const char *content) {
    FILE *f = fopen(path, "wb");
    TEST_ASSERT_NOT_NULL(f);
    fwrite(content, 1, strlen(content), f);
    fclose(f);
}

static void test_csv_parse_with_header(void) {
    write_tmp(TMP_CSV,
              "name,age,city\n"
              "Alice,30,NYC\n"
              "Bob,25,LA\n");
    CSV c = csv.parse((char *) TMP_CSV, true);

    TEST_ASSERT_EQUAL_INT(2, c.rows);
    TEST_ASSERT_EQUAL_INT(3, c.cols);
    TEST_ASSERT_NOT_NULL(c.header);
    TEST_ASSERT_EQUAL_STRING("name", c.header[0]);
    TEST_ASSERT_EQUAL_STRING("age",  c.header[1]);
    TEST_ASSERT_EQUAL_STRING("city", c.header[2]);

    TEST_ASSERT_EQUAL_STRING("Alice", c.table[0][0]);
    TEST_ASSERT_EQUAL_STRING("30",    c.table[0][1]);
    TEST_ASSERT_EQUAL_STRING("NYC",   c.table[0][2]);
    TEST_ASSERT_EQUAL_STRING("Bob",   c.table[1][0]);
    TEST_ASSERT_EQUAL_STRING("25",    c.table[1][1]);
    TEST_ASSERT_EQUAL_STRING("LA",    c.table[1][2]);

    destroy(c);
    remove(TMP_CSV);
}

static void test_csv_parse_without_header(void) {
    write_tmp(TMP_CSV,
              "1,2,3\n"
              "4,5,6\n"
              "7,8,9\n");
    CSV c = csv.parse((char *) TMP_CSV, false);

    TEST_ASSERT_EQUAL_INT(3, c.rows);
    TEST_ASSERT_EQUAL_INT(3, c.cols);
    TEST_ASSERT_NULL(c.header);
    TEST_ASSERT_EQUAL_STRING("1", c.table[0][0]);
    TEST_ASSERT_EQUAL_STRING("5", c.table[1][1]);
    TEST_ASSERT_EQUAL_STRING("9", c.table[2][2]);

    destroy(c);
    remove(TMP_CSV);
}

static void test_csv_quoted_field_with_comma(void) {
    write_tmp(TMP_CSV,
              "id,address\n"
              "1,\"123 Main St, Apt 4\"\n");
    CSV c = csv.parse((char *) TMP_CSV, true);

    TEST_ASSERT_EQUAL_INT(1, c.rows);
    TEST_ASSERT_EQUAL_INT(2, c.cols);
    TEST_ASSERT_EQUAL_STRING("1", c.table[0][0]);
    TEST_ASSERT_EQUAL_STRING("123 Main St, Apt 4", c.table[0][1]);

    destroy(c);
    remove(TMP_CSV);
}

static void test_csv_escaped_quotes(void) {
    write_tmp(TMP_CSV,
              "id,quote\n"
              "1,\"He said \"\"hi\"\"\"\n");
    CSV c = csv.parse((char *) TMP_CSV, true);

    TEST_ASSERT_EQUAL_INT(1, c.rows);
    TEST_ASSERT_EQUAL_INT(2, c.cols);
    TEST_ASSERT_EQUAL_STRING("He said \"hi\"", c.table[0][1]);

    destroy(c);
    remove(TMP_CSV);
}

static void test_csv_quoted_field_with_newline(void) {
    write_tmp(TMP_CSV,
              "id,note\n"
              "1,\"line1\nline2\"\n");
    CSV c = csv.parse((char *) TMP_CSV, true);

    TEST_ASSERT_EQUAL_INT(1, c.rows);
    TEST_ASSERT_EQUAL_INT(2, c.cols);
    TEST_ASSERT_EQUAL_STRING("line1\nline2", c.table[0][1]);

    destroy(c);
    remove(TMP_CSV);
}

static void test_csv_uneven_rows_pad_with_empty(void) {
    write_tmp(TMP_CSV,
              "a,b,c,d\n"
              "1,2\n"
              "3,4,5,6\n");
    CSV c = csv.parse((char *) TMP_CSV, true);

    TEST_ASSERT_EQUAL_INT(2, c.rows);
    TEST_ASSERT_EQUAL_INT(4, c.cols);
    TEST_ASSERT_EQUAL_STRING("1", c.table[0][0]);
    TEST_ASSERT_EQUAL_STRING("2", c.table[0][1]);
    TEST_ASSERT_EQUAL_STRING("", c.table[0][2]);
    TEST_ASSERT_EQUAL_STRING("", c.table[0][3]);
    TEST_ASSERT_EQUAL_STRING("3", c.table[1][0]);
    TEST_ASSERT_EQUAL_STRING("6", c.table[1][3]);

    destroy(c);
    remove(TMP_CSV);
}

static void test_csv_crlf_line_endings(void) {
    write_tmp(TMP_CSV,
              "x,y\r\n"
              "10,20\r\n"
              "30,40\r\n");
    CSV c = csv.parse((char *) TMP_CSV, true);

    TEST_ASSERT_EQUAL_INT(2, c.rows);
    TEST_ASSERT_EQUAL_INT(2, c.cols);
    TEST_ASSERT_EQUAL_STRING("10", c.table[0][0]);
    TEST_ASSERT_EQUAL_STRING("40", c.table[1][1]);

    destroy(c);
    remove(TMP_CSV);
}

static void test_csv_single_row_no_trailing_newline(void) {
    write_tmp(TMP_CSV, "k1,k2\nv1,v2");
    CSV c = csv.parse((char *) TMP_CSV, true);

    TEST_ASSERT_EQUAL_INT(1, c.rows);
    TEST_ASSERT_EQUAL_INT(2, c.cols);
    TEST_ASSERT_EQUAL_STRING("v1", c.table[0][0]);
    TEST_ASSERT_EQUAL_STRING("v2", c.table[0][1]);

    destroy(c);
    remove(TMP_CSV);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_csv_parse_with_header);
    RUN_TEST(test_csv_parse_without_header);
    RUN_TEST(test_csv_quoted_field_with_comma);
    RUN_TEST(test_csv_escaped_quotes);
    RUN_TEST(test_csv_quoted_field_with_newline);
    RUN_TEST(test_csv_uneven_rows_pad_with_empty);
    RUN_TEST(test_csv_crlf_line_endings);
    RUN_TEST(test_csv_single_row_no_trailing_newline);
    return UNITY_END();
}
