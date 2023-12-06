#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <string.h>

union SemaphoreUnion {
    int value;
    struct semid_ds *buffer;
    unsigned short *array;
    struct seminfo *__buffer;
};

void acquireSemaphore(int semaphoreId) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = -1;
    operation.sem_flg = SEM_UNDO;
    if (semop(semaphoreId, &operation, 1) == -1) {
        perror("acquireSemaphore failed");
        exit(EXIT_FAILURE);
    }
}

void releaseSemaphore(int semaphoreId) {
    struct sembuf operation;
    operation.sem_num = 0;
    operation.sem_op = 1;
    operation.sem_flg = SEM_UNDO;
    if (semop(semaphoreId, &operation, 1) == -1) {
        perror("releaseSemaphore failed");
        exit(EXIT_FAILURE);
    }
}

int sharedMemoryId;
void *sharedMemoryPtr;
int semaphoreId;

void signalHandler(int signal) {
    printf("Client: Signal received from Signal %d\n", signal);
    if (signal == SIGUSR1) {
        int sum = 0, i = 0, value;

        printf("Client: Calculating sum of data...\n");
        acquireSemaphore(semaphoreId);

        do {
            memcpy(&value, (int *)sharedMemoryPtr + i, sizeof(int));
            if (value == 0) break;
            sum += value;
            i++;
        } while (1);

        memcpy(sharedMemoryPtr, &sum, sizeof(int));

        printf("Client: Sum calculated and written to shared memory...\n");
        releaseSemaphore(semaphoreId);
        printf("Client: Notifying the server about data processing completion...\n");
        kill(getppid(), SIGUSR1);
    }
}

void connectSharedMemoryAndSemaphore(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <shm_id> <sem_id>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Client: Connecting to the shared memory...\n");
    sharedMemoryId = atoi(argv[1]);
    semaphoreId = atoi(argv[2]);

    sharedMemoryPtr = shmat(sharedMemoryId, NULL, 0);
    if (sharedMemoryPtr == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    printf("Client: Connected to shared memory.\n");
    signal(SIGUSR1, signalHandler);
}

void runClient() {
    while (1) {
        pause();
    }
}

int main(int argc, char *argv[]) {
    connectSharedMemoryAndSemaphore(argc, argv);
    runClient();
    shmdt(sharedMemoryPtr);
    return 0;
}

