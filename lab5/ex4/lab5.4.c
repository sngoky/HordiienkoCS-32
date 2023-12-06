#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

double generateRandomNumber() {
    return (double)rand() / RAND_MAX;
}

int main(int argc, char *argv[]) {
    int childProcessesCount = 0;
    char *outputFileName = NULL;

    int option;
    while ((option = getopt(argc, argv, "n:f:")) != -1) {
        switch (option) {
            case 'n':
                childProcessesCount = atoi(optarg);
                break;
            case 'f':
                outputFileName = optarg;
                break;
            default:
                fprintf(stderr, "Usage: %s -n <number of child processes> -f <file name>\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (outputFileName == NULL) {
        outputFileName = "default_output_filename";
    }

    for (int i = 1; i <= childProcessesCount; i++) {
        pid_t childPid = fork();

        if (childPid == 0) {
            char childOutputFileName[100];
            sprintf(childOutputFileName, "%s_child%d.txt", outputFileName, i);

            FILE *outputFile = fopen(childOutputFileName, "w");
            if (outputFile == NULL) {
                perror("Failed to open file");
                exit(EXIT_FAILURE);
            }

            for (int j = 0; j < i; j++) {
                double randomValue = generateRandomNumber();
                fprintf(outputFile, "%.6f\n", randomValue);
            }

            fclose(outputFile);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < childProcessesCount; i++) {
        wait(NULL);
    }

    return 0;
}

