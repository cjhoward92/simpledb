//
// Created by carson on 7/6/22.
//

#include <check.h>

#include "../lib/commands.h"

START_TEST(test_parse_command) {
    size_t len = 10;
    const char* cmd_str = "get my_key";
    parsed_command *cmd = NULL;

    int res = parse_command(len, cmd_str, &cmd);

    char *p = cmd->cmd->payload;
    ck_assert_msg(res == 0, "Parsing failed");
    ck_assert_msg(cmd != NULL, "Command was not parsed properly");
    ck_assert_msg(strcmp(cmd->cmd->cmd_str, GET_COMMAND) == 0, "Invalid command");
    ck_assert_msg(strcmp(p, "my_key") == 0, "Invalid payload");

    free(cmd->cmd->cmd_str);
    free(cmd->cmd->payload);
    free(cmd->cmd);
    free(cmd);
} END_TEST

Suite * make_test_suite() {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Command tests");

    tc_core = tcase_create("parsing");
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