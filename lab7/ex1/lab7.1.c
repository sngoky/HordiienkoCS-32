#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define SHARED_MEMORY_SIZE 1024

int sharedMemoryId;
void *sharedMemoryPtr;
pid_t childProcessPid;

void handleSignal(int signalNumber) {
    if (signalNumber == SIGUSR1) {
        int sum;
        memcpy(&sum, sharedMemoryPtr, sizeof(int));
        printf("Parent Program: Sum of elements: %d\n", sum);
    }
}

void createSharedMemory() {
    sharedMemoryId = shmget(IPC_PRIVATE, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    sharedMemoryPtr = shmat(sharedMemoryId, NULL, 0);
    if (sharedMemoryPtr == (void *) -1) {
        perror("shmat");
        exit(1);
    }
}

void destroySharedMemory() {
    shmdt(sharedMemoryPtr);
    shmctl(sharedMemoryId, IPC_RMID, NULL);
}

void runChildProcess() {
    char sharedMemoryIdStr[10];
    sprintf(sharedMemoryIdStr, "%d", sharedMemoryId);
    execlp("./child_program", "child_program", sharedMemoryIdStr, NULL);
    perror("execlp");
    exit(1);
}

void handleParentProcess() {
    while (1) {
        int elementCount, i, input;
        printf("Parent Program: Enter the number of elements for summation (0 to exit): ");
        scanf("%d", &elementCount);
        if (elementCount <= 0) {
            break;
        }
        for (i = 0; i < elementCount; i++) {
            printf("Parent Program: Enter element %d: ", i + 1);
            scanf("%d", &input);
            memcpy(sharedMemoryPtr + i * sizeof(int), &input, sizeof(int));
        }
        int endMarker = 0;
        memcpy(sharedMemoryPtr + elementCount * sizeof(int), &endMarker, sizeof(int));
        kill(childProcessPid, SIGUSR1);
        pause();
    }
    kill(childProcessPid, SIGTERM);
    waitpid(childProcessPid, NULL, 0);
}

int main(int argc, char *argv[]) {
    createSharedMemory();
    signal(SIGUSR1, handleSignal);
    childProcessPid = fork();
    if (childProcessPid == 0) {
        runChildProcess();
    } else {
        handleParentProcess();
        destroySharedMemory();
    }
    return 0;
}

