#include <stdio.h>
#include <stdlib.h>
#define M_ST 100

struct S {
    char n[50];
    char sn[50];
    int a;
};

void cb() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    struct S s[M_ST];
    int ns;

    printf("How many students are in your group -> ");
    scanf("%d", &ns);

    if (ns <= 0 || ns > M_ST) {
        printf("Error: Please enter a valid number of students (1-%d)\n", M_ST);
        return 1;
    }

    cb();

    for (int i = 0; i < ns; i++) {
        printf("\nStudent %d\n", i + 1);

        printf("Enter name: ");
        fgets(s[i].n, sizeof(s[i].n), stdin);

        printf("Enter surname: ");
        fgets(s[i].sn, sizeof(s[i].sn), stdin);

        printf("Enter age: ");
        scanf("%d", &s[i].a);
        cb();
    }

    printf("\nPress Enter to check all students...\n");
    cb();

    printf("\nList of students:\n");
    for (int i = 0; i < ns; i++) {
        printf("\nStudent %d\n", i + 1);

        printf("Name: %s", s[i].n);
        printf("Surname: %s", s[i].sn);
        printf("Age: %d\n", s[i].a);
    }

    return 0;
}

