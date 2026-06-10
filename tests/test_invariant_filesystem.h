#include <check.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "opencstl/filesystem.h"

START_TEST(test_path_join_buffer_safety)
{
    // Invariant: path_join must not write beyond allocated buffer bounds
    // Test with adversarial lengths that could trigger overflow or off-by-one errors
    
    struct test_case {
        const char *path1;
        const char *path2;
    } cases[] = {
        // Maximum length paths that could overflow size_t calculation
        {"/aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
         "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
         "/bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
         "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
         "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"
         "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb"},
        // Boundary: empty and non-empty paths
        {"", "/test"},
        {"/test", ""},
        // Valid normal case
        {"/usr/local", "bin"}
    };
    
    for (size_t i = 0; i < sizeof(cases) / sizeof(cases[0]); i++) {
        char *result = path_join(cases[i].path1, cases[i].path2);
        
        if (result != NULL) {
            size_t l1 = strlen(cases[i].path1);
            size_t l2 = strlen(cases[i].path2);
            size_t expected_max = l1 + l2 + 2;
            size_t actual_len = strlen(result);
            
            // Invariant: result length must not exceed allocated size
            ck_assert_uint_le(actual_len, expected_max);
            
            // Invariant: result must be null-terminated
            ck_assert_int_eq(result[actual_len], '\0');
            
            free(result);
        }
    }
}
END_TEST

Suite *security_suite(void)
{
    Suite *s;
    TCase *tc_core;

    s = suite_create("Security");
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_path_join_buffer_safety);
    suite_add_tcase(s, tc_core);

    return s;
}

int main(void)
{
    int number_failed;
    Suite *s;
    SRunner *sr;

    s = security_suite();
    sr = srunner_create(s);

    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}