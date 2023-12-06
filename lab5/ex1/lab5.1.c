#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int execute_commands(char *command_sequence) {
    char *tokenized_command = strtok(command_sequence, ";");

    while (tokenized_command != NULL) {
        int exit_status = system(tokenized_command);

        if (WIFEXITED(exit_status)) {
            int exit_code = WEXITSTATUS(exit_status);
            printf("Command '%s' exited with status %d\n", tokenized_command, exit_code);
        } else {
            printf("Failed to execute command '%s'\n", tokenized_command);
        }

        tokenized_command = strtok(NULL, ";");
    }

    return 0;
}

int main(int argument_count, char *arguments[]) {
    if (argument_count != 2) {
        printf("Usage: %s \"command(s)\"\n", arguments[0]);
        return 1;
    }

    char *command_sequence = arguments[1];
    int result = execute_commands(command_sequence);

    return result;
}

