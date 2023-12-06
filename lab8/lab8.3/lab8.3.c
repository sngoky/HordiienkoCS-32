#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void handleCustomAlarm(int signal) {
}

void configureCustomAlarm(int duration, const char *notificationText) {
    signal(SIGALRM, handleCustomAlarm);
    alarm(duration);
    pause();
    printf("Scheduled Notification: %s\n", notificationText);
}

int main(int argumentCount, char *argumentValues[]) {
    if (argumentCount != 3) {
        fprintf(stderr, "Usage: %s <duration in seconds> <message>\n", argumentValues[0]);
        return 1;
    }

    int notificationDuration = atoi(argumentValues[1]);
    char *notificationMessage = argumentValues[2];

    pid_t childProcessID = fork();

    if (childProcessID < 0) {
        perror("Fork failed");
        exit(1);
    }

    if (childProcessID == 0) {
        configureCustomAlarm(notificationDuration, notificationMessage);
        exit(0);
    } else {
        printf("Notification set for %d seconds. Child process PID: %d\n", notificationDuration, childProcessID);
        exit(0);
    }

    return 0;
}

