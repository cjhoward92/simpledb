//
// Created by carson on 6/25/22.
//

#include "commands.h"

static inline u_int8_t is_whitespace(char c) {
    if (c == ' '
    || c == '\n'
    || c == '\t'
    || c == '\r'
    || c == '\v'
    || c == '\f') {
        return 1;
    }
    return 0;
}

int parse_command(size_t len, const char* str, parsed_command **out) {
    if (out == NULL) {
        return -1;
    }

    if (len <= 0 || str == NULL) {
        return -1;
    }

    (*out) = (parsed_command *)malloc(sizeof(parsed_command));
    command *cmd = (command *)malloc(sizeof(command));

    size_t i = 0;
    while (!is_whitespace(str[i]) && i < len) { i++; }

    if (i >= len) {
        free(cmd);
        free(*out);
        return -1;
    }

    cmd->len = i;
    cmd->cmd_str = (char *)malloc(cmd->len);
    memcpy(cmd->cmd_str, str, cmd->len);

    cmd->payload_len = len - cmd->len - 1;
    cmd->payload = (char *)malloc(cmd->payload_len);
    memcpy(cmd->payload, (str + cmd->len + 1), cmd->payload_len);

    (*out)->cmd = cmd;
    cmd = NULL;

    return 0;
}

