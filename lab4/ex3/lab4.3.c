#include <stdio.h>
#include <stdlib.h>

int main(int command_line_arguments_count, char *command_line_arguments[]) {
    if (command_line_arguments_count == 2) {
        const char *environment_variable_name = command_line_arguments[1];
        if (unsetenv(environment_variable_name) != 0) {
            perror("Error deleting environment variable");
            return 1;
        }
    } else {
        if (clearenv() != 0) {
            perror("Failed to delete all environment variables");
            return 1;
        }
    }

    extern char **environ;
    for (char **env = environ; env != NULL && *env != NULL; env++) {
        printf("%s\n", *env);
    }

    return 0;
}

