//
// Created by carson on 6/25/22.
//

#ifndef TLS_SERVER_COMMANDS_H
#define TLS_SERVER_COMMANDS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define GET_COMMAND "get"
#define STORE_COMMAND "store"
#define DELETE_COMMAND "delete"
#define UNKNOWN_COMMAND "unknown"

typedef struct command {
    char *cmd_str;
    void *payload;
    size_t len;
    size_t payload_len;
} command;

typedef int (*execute_cmd)(command *);

typedef struct parsed_command {
    command *cmd;
    execute_cmd execute;
} parsed_command;

int parse_command(size_t len, const char* str, command **out);
int execute_command(const command * parsed);

void free_parsed_command(parsed_command *p_cmd);
void free_command(command *cmd);

#endif //TLS_SERVER_COMMANDS_H
