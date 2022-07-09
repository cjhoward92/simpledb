//
// Created by carson on 7/6/22.
//

#include <check.h>

#include "../lib/commands.h"

START_TEST(test_parse_command) {
    size_t len = 10;
    const char* cmd_str = "get my_key";
    command *cmd = NULL;

    int res = parse_command(len, cmd_str, &cmd);

    char *p = cmd->payload;
    ck_assert_msg(res == 0, "Parsing failed");
    ck_assert_msg(cmd != NULL, "Command was not parsed properly");
    ck_assert_msg(strcmp(cmd->cmd_str, GET_COMMAND) == 0, "Invalid command");
    ck_assert_msg(strcmp(p, "my_key") == 0, "Invalid payload");

    free_command(cmd);
} END_TEST

START_TEST(test_parse_command_no_str) {
    size_t len = 0;
    const char* cmd_str = "";
    command *cmd = NULL;

    int res = parse_command(len, cmd_str, &cmd);

    ck_assert_msg(res == -1, "Parsing should have failed");
    ck_assert_msg(cmd == NULL, "Command should not have parsed properly");
} END_TEST

START_TEST(test_parse_command_null_out) {
    int res = parse_command(0, "", NULL);

    ck_assert_msg(res == -1, "Parsing should have failed");
} END_TEST

Suite * make_test_suite() {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Command tests");

    tc_core = tcase_create("parsing");
    tcase_add_test(tc_core, test_parse_command);
    tcase_add_test(tc_core, test_parse_command_no_str);
    tcase_add_test(tc_core, test_parse_command_null_out);

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