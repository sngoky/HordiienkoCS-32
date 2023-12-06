#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void *childFunction(void *arg) {
    int iteration;
    for (iteration = 1; iteration <= 10; iteration++) {
        printf("Child Thread. Iteration: %d\n\n", iteration);
        sleep(2);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t childThread;
    int iteration;
    if (pthread_create(&childThread, NULL, childFunction, NULL) != 0) {
        perror("pthread_create");
        exit(1);
    }
    for (iteration = 1; iteration <= 10; iteration++) {
        printf("Main Thread. Iteration: %d\n\n", iteration);
        sleep(2);
    }
    if (pthread_join(childThread, NULL) != 0) {
        perror("pthread_join");
        exit(1);
    }
    return 0;
}

