#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void display_environment() {
    extern char **environ;
    char **env_ptr = environ;
    while (*env_ptr) {
        printf("%s\n", *env_ptr);
        env_ptr++;
    }
}

void display_help() {
    printf("Usage: environment [options]\n");
    printf("Options:\n");
    printf("  -h, --help           Display this help message\n");
    printf("  -i, --info <variable> Display the value of the specified variable\n");
    printf("  -s, --set <variable=value> Set the value of the specified variable\n");
    printf("  -d, --del <variable> Delete the specified variable\n");
    printf("  -a, --assign <variable> --value <value> Assign a new value to the specified variable\n");
    printf("  -c, --clear          Clear the environment\n");
}

void display_variable(const char *variable) {
    char *value = getenv(variable);
    if (value) {
        printf("%s=%s\n", variable, value);
    } else {
        printf("Variable %s is not set.\n", variable);
    }
}

void set_variable_value(const char *variable, const char *value) {
    setenv(variable, value, 1);
    printf("%s=%s\n", variable, value);
}

void delete_variable(const char *variable) {
    unsetenv(variable);
}

void assign_value(const char *variable, const char *value) {
    if (value) {
        setenv(variable, value, 1);
        printf("%s=%s\n", variable, value);
    } else {
        printf("No value specified for assignment.\n");
    }
}

void clear_environment() {
    clearenv();
}

int main(int argc, char *argv[]) {
    if (argc == 1) {
        // Display information about the current environment
        display_environment();
    } else if (argc == 2 && (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0)) {
        // Display help
        display_help();
    } else if (argc == 3 && (strcmp(argv[1], "-i") == 0 || strcmp(argv[1], "--info") == 0)) {
        // Display the value of the specified variable
        display_variable(argv[2]);
    } else if (argc == 4 && (strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "--set") == 0)) {
        // Set the value of the specified variable
        set_variable_value(argv[2], argv[3]);
    } else if (argc == 3 && (strcmp(argv[1], "-d") == 0 || strcmp(argv[1], "--del") == 0)) {
        // Delete the specified variable
        delete_variable(argv[2]);
    } else if (argc == 5 && strcmp(argv[1], "-a") == 0 && strcmp(argv[3], "-v") == 0) {
        // Assign a new value to the specified variable
        assign_value(argv[2], argv[4]);
    } else if ((argc == 2 && strcmp(argv[1], "-c") == 0) || (strcmp(argv[1], "--clear") == 0)) {
        // Clear the environment
        clear_environment();
    } else {
        // Invalid command format, display help
        display_help();
    }

    return 0;
}

