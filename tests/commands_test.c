//
// Created by carson on 7/6/22.
//

#include <check.h>

#include "../lib/commands.h"

START_TEST(test_parse_command) {
    printf("Hello\n");
    ck_assert_msg(1 == 1, "invalid failed tests");
} END_TEST

Suite * make_test_suite() {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Basic tests");

    tc_core = tcase_create("Sample test cases");

    tcase_add_test(tc_core, test_parse_command);

    suite_add_tcase(s, tc_core);

    return s;
}

// Test main
int main() {
    int number_failed;
    SRunner *sr;

    sr = srunner_create(make_test_suite());
    srunner_set_fork_status(sr, CK_NOFORK);
    srunner_run_all(sr, CK_VERBOSE);

    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}