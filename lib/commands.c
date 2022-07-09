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

int parse_command(size_t len, const char* str, command **out) {
    if (out == NULL) {
        return -1;
    }

    if (len <= 0 || str == NULL) {
        return -1;
    }

    command *cmd = (command *)malloc(sizeof(command));
    cmd->cmd_str = NULL;
    cmd->payload = NULL;
    cmd->len = 0;
    cmd->payload_len = 0;

    size_t i = 0;
    while (!is_whitespace(str[i]) && i < len) { i++; }

    if (i >= len) {
        free(cmd);
        return -1;
    }

    cmd->len = i;
    cmd->cmd_str = (char *)calloc(cmd->len, sizeof(char));
    memcpy(cmd->cmd_str, str, cmd->len);

    cmd->payload_len = len - cmd->len -1;
    cmd->payload = (char *)calloc(cmd->len, sizeof(char));
    memcpy(cmd->payload, (str + cmd->len + 1), cmd->payload_len);

    (*out) = cmd;
    cmd = NULL;

    return 0;
}

void free_command(command *cmd) {
    if (cmd->cmd_str) {
        free(cmd->cmd_str);
    }
    if (cmd->payload) {
        free(cmd->payload);
    }
    free(cmd);
    cmd = NULL;
}

void free_parsed_command(parsed_command *p_cmd) {
    if (p_cmd->cmd) {
        free_command(p_cmd->cmd);
    }
    free(p_cmd);
}
