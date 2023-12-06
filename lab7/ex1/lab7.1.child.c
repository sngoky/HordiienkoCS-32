#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <string.h>

#define SHARED_MEMORY_SIZE 1024

int sharedMemoryId;
void *sharedMemoryPointer;

void handleSignal(int signalNumber);

void attachSharedMemory(char *sharedMemoryIdString);

void registerSignalHandler();

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: %s <Shared_Memory_Id>\n", argv[0]);
        return 1;
    }

    attachSharedMemory(argv[1]);
    registerSignalHandler();

    while (1) {
        pause();
    }

    shmdt(sharedMemoryPointer);
    return 0;
}

void handleSignal(int signalNumber) {
    printf("\nChild Program: Signal handler called: %d\n", signalNumber);
    if (signalNumber == SIGUSR1) {
        int sum = 0, i = 0, value;
        printf("Child Program: Reading data from shared memory...\n");

        do {
            memcpy(&value, sharedMemoryPointer + i * sizeof(int), sizeof(int));
            if (value == 0) break;
            sum += value;
            i++;
        } while (1);

        printf("Child Program: Read sum: %d\n", sum);

        memcpy(sharedMemoryPointer, &sum, sizeof(int));

        kill(getppid(), SIGUSR1);
        printf("Child Program: Signal sent to the parent process\n");
    }
}

void attachSharedMemory(char *sharedMemoryIdString) {
    sharedMemoryId = atoi(sharedMemoryIdString);
    printf("\nChild Program: Attaching to shared memory with id: %d\n", sharedMemoryId);
    sharedMemoryPointer = shmat(sharedMemoryId, NULL, 0);

    if (sharedMemoryPointer == (void *) -1) {
        perror("Child Program: shmat");
        exit(1);
    } else {
        printf("Child Program: Successfully attached memory at address: %p\n", sharedMemoryPointer);
    }
}

void registerSignalHandler() {
    struct sigaction signalAction;
    memset(&signalAction, 0, sizeof(signalAction));
    signalAction.sa_handler = handleSignal;
    sigaction(SIGUSR1, &signalAction, NULL);
}

