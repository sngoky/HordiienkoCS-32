#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handleUserSignal(int signum) {
    printf("Received SIGUSR1 signal. Performing cleanup...\n");
    exit(0);
}

void handleInterrupt(int signum) {
    printf("Received SIGINT signal. Processing termination...\n");
}

void handleTermination(int signum) {
    printf("Received SIGTERM signal. Processing termination...\n");
}

int main() {
    signal(SIGINT, handleInterrupt);
    signal(SIGTERM, handleTermination);
    signal(SIGUSR1, handleUserSignal);

    signal(SIGPROF, SIG_DFL);
    signal(SIGHUP, SIG_IGN);

    while (1) {
        pause();
    }

    return 0;
}

