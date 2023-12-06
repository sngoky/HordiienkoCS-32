#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void customSignalHandler(int signum) {
    printf("Custom handler for signal %d\n", signum);
    sleep(1);
}

int main(int argc, char *cmdArgs[]) {
    if (argc != 2) {
        printf("Usage: %s <num_of_handlers>\n", cmdArgs[0]);
        return 1;
    }

    int numHandlers = atoi(cmdArgs[1]);

    if (numHandlers <= 0) {
        printf("Invalid number of handlers\n");
        return 1;
    }

    for (int i = 1; i <= numHandlers; i++) {
        if (signal(SIGINT, customSignalHandler) == SIG_ERR) {
            perror("Failed to register signal handler");
            return 1;
        }
    }

    printf("Registered %d signal handlers. Waiting for signals...\n", numHandlers);

    while (1) {
        pause();
    }

    return 0;
}

