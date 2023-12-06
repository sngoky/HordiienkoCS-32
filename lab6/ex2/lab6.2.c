#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct ThreadData {
    char *thread_name;
    char *message;
    int count;
};

void *custom_thread_function(void *arg) {
    struct ThreadData *thread_data = (struct ThreadData *)arg;
    for (int i = 1; i <= thread_data->count; i++) {
        printf("Thread %s. %s %d\n", thread_data->thread_name, thread_data->message, i);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread_ids[4];
    struct ThreadData thread_args[] = {
            {"Alpha", "Message1", 3},
            {"Beta", "Message2", 2},
            {"Gamma", "Message3", 4},
            {"Delta", "Message4", 5}
    };

    for (int i = 0; i < 4; i++) {
        if (pthread_create(&thread_ids[i], NULL, custom_thread_function, &thread_args[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }

    for (int i = 0; i < 4; i++) {
        if (pthread_join(thread_ids[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }

    return 0;
}

