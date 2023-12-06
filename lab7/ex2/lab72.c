#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

#define SHARED_MEMORY_SIZE 1024

union SemaphoreUnion {
    int value;
    struct semid_ds *info;
    unsigned short *array;
};

int sharedMemoryId;
void *sharedMemoryPtr;
pid_t childProcessPid;
int semaphoreId;

void handleSignal(int sig);

void initializeSemaphore(int semaphoreId, int semaphoreValue);

void acquireSemaphore(int semaphoreId);

void releaseSemaphore(int semaphoreId);

void createSharedMemory();

void createSemaphore();

void cleanupResources();

void launchChildProcess();

void processUserInput();

int main() {
    createSharedMemory();
    createSemaphore();

    signal(SIGUSR1, handleSignal);

    printf("Server: Initiating the summation process...\n");
    childProcessPid = fork();

    if (childProcessPid == 0) {
        launchChildProcess();
    } else {
        processUserInput();

        kill(childProcessPid, SIGTERM);
        waitpid(childProcessPid, NULL, 0);
        cleanupResources();
    }

    return 0;
}

void handleSignal(int sig) {
    if (sig == SIGUSR1) {
        int sum;
        memcpy(&sum, sharedMemoryPtr, sizeof(int));
        printf("Sum: %d\n", sum);
    }
}

void initializeSemaphore(int semaphoreId, int semaphoreValue) {
    union SemaphoreUnion semaphoreArgument;
    semaphoreArgument.value = semaphoreValue;
    if (semctl(semaphoreId, 0, SETVAL, semaphoreArgument) == -1) {
        perror("semctl");
        exit(1);
    }
}

void acquireSemaphore(int semaphoreId) {
    struct sembuf semaphoreOperations[1];
    semaphoreOperations[0].sem_num = 0;
    semaphoreOperations[0].sem_op = -1;
    semaphoreOperations[0].sem_flg = 0;
    if (semop(semaphoreId, semaphoreOperations, 1) == -1) {
        perror("semop acquire");
        exit(1);
    }
}

void releaseSemaphore(int semaphoreId) {
    struct sembuf semaphoreOperations[1];
    semaphoreOperations[0].sem_num = 0;
    semaphoreOperations[0].sem_op = 1;
    semaphoreOperations[0].sem_flg = 0;
    if (semop(semaphoreId, semaphoreOperations, 1) == -1) {
        perror("semop release");
        exit(1);
    }
}

void createSharedMemory() {
    sharedMemoryId = shmget(IPC_PRIVATE, SHARED_MEMORY_SIZE, IPC_CREAT | 0666);
    sharedMemoryPtr = shmat(sharedMemoryId, NULL, 0);

    if (sharedMemoryPtr == (void *)-1) {
        perror("shmat");
        exit(1);
    }
}

void createSemaphore() {
    semaphoreId = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
    if (semaphoreId == -1) {
        perror("semget");
        exit(1);
    }

    initializeSemaphore(semaphoreId, 1);
}

void cleanupResources() {
    shmdt(sharedMemoryPtr);
    shmctl(sharedMemoryId, IPC_RMID, NULL);
    semctl(semaphoreId, 0, IPC_RMID, 0);
}

void launchChildProcess() {
    char sharedMemoryIdStr[10];
    char semaphoreIdStr[10];
    sprintf(sharedMemoryIdStr, "%d", sharedMemoryId);
    sprintf(semaphoreIdStr, "%d", semaphoreId);

    execlp("./childsProgram", "childsProgram", sharedMemoryIdStr, semaphoreIdStr, (char *)NULL);

    perror("execlp");
    exit(1);
}

void processUserInput() {
    while (1) {
        int numberOfElements, i, userInput;
        printf("Enter the number of elements to sum (0 - exit): ");
        scanf("%d", &numberOfElements);

        if (numberOfElements <= 0) {
            break;
        }

        printf("Server: Blocking semaphore before writing data...\n");
        acquireSemaphore(semaphoreId);

        for (i = 0; i < numberOfElements; i++) {
            printf("Enter a number: ");
            scanf("%d", &userInput);
            memcpy((int *)sharedMemoryPtr + i, &userInput, sizeof(int));
        }

        printf("Server: Unlocking semaphore after writing data...\n");
        releaseSemaphore(semaphoreId);

        printf("Server: Signaling the child process to compute the sum...\n");
        kill(childProcessPid, SIGUSR1);

        printf("Server: Waiting for the child process to complete computation...\n");
        pause();
    }
}

