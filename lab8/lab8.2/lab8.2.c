#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handleTerminate(int signum) {
    printf("Received SIGTERM signal. Processing termination...\n");
}

void handleInterrupt(int signum) {
    printf("Received SIGINT signal. Processing termination...\n");
}

void handleUserSignal(int signum) {
    printf("Received SIGUSR1 signal. Finalizing operations...\n");
    exit(0);
}

int main() {
    struct sigaction signalAction;

    signalAction.sa_handler = handleInterrupt;
    sigaction(SIGINT, &signalAction, NULL);

    signalAction.sa_handler = handleTerminate;
    sigaction(SIGTERM, &signalAction, NULL);

    signalAction.sa_handler = handleUserSignal;
    sigaction(SIGUSR1, &signalAction, NULL);

    signal(SIGPROF, SIG_DFL);
    signal(SIGHUP, SIG_IGN);

    while (1) {
        pause();
    }

    return 0;
}

