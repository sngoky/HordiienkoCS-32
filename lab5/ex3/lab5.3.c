#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_BUFFER_SIZE 127

void display_help() {
    printf("Simple Command Processor\n");
    printf("Usage: cmd_processor [-h|--help] [-b|--buffer <buffer_size>]\n");
    printf("Options:\n");
    printf("  -h, --help            Show this help message\n");
    printf("  -b, --buffer <size>   Set the command buffer size (default: %d)\n", DEFAULT_BUFFER_SIZE);
}

int main(int argc, char *argv[]) {
    int buffer_size = DEFAULT_BUFFER_SIZE;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            display_help();
            return 0;
        } else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--buffer") == 0) {
            if (i + 1 < argc) {
                buffer_size = atoi(argv[i + 1]);
                if (buffer_size <= 0) {
                    printf("Invalid buffer size. Using default size (%d).\n", DEFAULT_BUFFER_SIZE);
                    buffer_size = DEFAULT_BUFFER_SIZE;
                }
            }
        }
    }

    char *user_name = getenv("USER");
    if (user_name == NULL) {
        printf("Unknown user\n");
        return 1;
    }

    printf("Welcome, %s!\n", user_name);
    printf("[%s]$", user_name);

    char user_input[buffer_size];
    while (1) {
        if (fgets(user_input, buffer_size, stdin) == NULL) {
            break;
        }

        if (strcmp(user_input, "stop\n") == 0) {
            printf("Wait 3 seconds...\n");
            sleep(3);
            printf("\033[H\033[J"); 
            break;
        }

        int execution_result = system(user_input);
        if (execution_result == -1) {
            printf("Error executing the command.\n");
        }
    }

    return 0;
}

