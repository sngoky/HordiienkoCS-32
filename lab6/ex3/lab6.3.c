#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

int g_iterations;
int g_min_num;
int g_max_num;
int g_target_num;
int g_exit_signal = 0;

struct ThreadInfo {
    char *thread_name;
    char *message;
};

void *custom_thread_function(void *arg) {
    struct ThreadInfo *info = (struct ThreadInfo *)arg;
    for (int i = 1; i <= g_iterations; i++) {
        if (g_exit_signal) {
            printf("Thread %s. Exiting due to signal.\n", info->thread_name);
            pthread_exit(NULL);
        }
        printf("Thread %s. %s %d\n", info->thread_name, info->message, i);
        int random = g_min_num + rand() % (g_max_num - g_min_num + 1);
        printf("Thread %s. Random Number: %d\n", info->thread_name, random);
        if (random == g_target_num) {
            printf("Thread %s. Received %d, setting exit signal.\n", info->thread_name, g_target_num);
            g_exit_signal = 1;
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread_array[2];
    struct ThreadInfo thread_info_array[] = {
            {"First", "Message1"},
            {"Second", "Message2"}
    };
    printf("Enter the number of iterations: ");
    scanf("%d", &g_iterations);
    printf("Enter the minimum number: ");
    scanf("%d", &g_min_num);
    printf("Enter the maximum number: ");
    scanf("%d", &g_max_num);
    printf("Enter the target number: ");
    scanf("%d", &g_target_num);
    srand(time(NULL));
    for (int i = 0; i < 2; i++) {
        if (pthread_create(&thread_array[i], NULL, custom_thread_function, &thread_info_array[i]) != 0) {
            perror("pthread_create");
            exit(1);
        }
    }
    for (int i = 0; i < 2; i++) {
        if (pthread_join(thread_array[i], NULL) != 0) {
            perror("pthread_join");
            exit(1);
        }
    }
    printf("Main Thread. All threads have completed.\n");
    return 0;
}

