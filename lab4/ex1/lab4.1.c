#include <stdio.h>
#include <stdlib.h>

int displayEnvironmentVariable(int argumentCount, char *argumentValues[]) {
    if (argumentCount != 2) {
        printf("Usage: %s <variable_name>\n", argumentValues[0]);
        return 1;
    }

    const char *variableName = argumentValues[1];
    char *variableValue = getenv(variableName);

    if (variableValue) {
        printf("%s=%s\n", variableName, variableValue);
    } else {
        printf("Environment variable with name %s not found.\n", variableName);
    }

    return 0;
}

int main(int argc, char *argv[]) {
    return displayEnvironmentVariable(argc, argv);
}

