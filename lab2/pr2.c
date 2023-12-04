#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to generate a random sequence of given length
void generateSequence(int sequence[], int length) {
    for (int i = 0; i < length; i++) {
        sequence[i] = rand() % 10;
        printf("%d ", sequence[i]);
    }
    printf("\n");
}

// Function to check if 7 is present in the sequence
int containsSeven(int sequence[], int length) {
    for (int i = 0; i < length; i++) {
        if (sequence[i] == 7) {
            return 1; // true
        }
    }
    return 0; // false
}

// Function to count the number of duplicates in the sequence
int countDuplicates(int sequence[], int length) {
    int count = 0;
    for (int i = 0; i < length; i++) {
        for (int j = i + 1; j < length; j++) {
            if (sequence[i] == sequence[j]) {
                count++;
            }
        }
    }
    return count;
}

int main() {
    srand(time(NULL));
    int bet;
    int length;
    int win = 0;

    printf("¬ведите вашу ставку: ");
    scanf("%d", &bet);

    printf("¬ведите количество чисел в последовательности: ");
    scanf("%d", &length);

    int sequence[length];
    
    generateSequence(sequence, length);

    if (containsSeven(sequence, length)) {
        win += bet;
    }

    int duplicateCount = countDuplicates(sequence, length);

    if (duplicateCount > 0) {
        win += bet;
    }

    if (duplicateCount >= length / 3) {
        win += bet;
    }

    printf("¬аш выигрыш: %d\n", win);

    return 0;
}

