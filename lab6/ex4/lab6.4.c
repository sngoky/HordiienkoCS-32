#include <stdio.h>
#include <pthread.h>

unsigned long long calculateCatalan(int n) {
    if (n <= 1) {
        return 1;
    }
    unsigned long long result = 0;
    for (int i = 0; i < n; i++) {
        result += calculateCatalan(i) * calculateCatalan(n - i - 1);
    }
    return result;
}

int isPrimeNumber(int number) {
    if (number <= 1) {
        return 0;
    }
    for (int i = 2; i * i <= number; i++) {
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}

struct ThreadArguments {
    int inputN;
    unsigned long long catalanResult;
    int primeCount;
};

void *threadFunction(void *arg) {
    struct ThreadArguments *args = (struct ThreadArguments *)arg;
    for (int i = 0; i <= args->inputN; i++) {
        args->catalanResult = calculateCatalan(i);
        printf("Thread: Calculated Catalan Number for N = %d: %llu\n", i, args->catalanResult);
        args->primeCount = 0;  
        for (int j = 2; j <= args->catalanResult; j++) {
            if (isPrimeNumber(j)) {
                args->primeCount++;
            }
        }
        printf("Thread: Count of Prime Numbers for N = %d: %d\n", i, args->primeCount);
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    struct ThreadArguments arguments;
    printf("Enter the number 'N' of Catalan Numbers to calculate: ");
    scanf("%d", &arguments.inputN);
    pthread_create(&thread, NULL, threadFunction, &arguments);
    pthread_join(thread, NULL);
    printf("Last calculated Catalan Number: %llu\n", arguments.catalanResult);
    printf("Count of Prime Numbers up to last calculated Catalan Number: %d\n", arguments.primeCount);
    return 0;
}

