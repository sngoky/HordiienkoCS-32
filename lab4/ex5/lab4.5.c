#include <stdio.h>
#include <unistd.h>
#include <sys/utsname.h>

int main() {
    char computer_name[256];
    struct utsname system_data;

    // ???????? ??'? ????'?????
    if (gethostname(computer_name, sizeof(computer_name)) == 0) {
        printf("Hostname: %s\n", computer_name);
    } else {
        perror("Error getting hostname");
    }

    // ???????? ?????????? ??? ????'????
    if (uname(&system_data) == 0) {
        printf("System information:\n");
        printf("OS: %s\n", system_data.sysname);
        printf("Kernel version: %s\n", system_data.release);
        printf("OS version: %s\n", system_data.version);
        printf("Machine type: %s\n", system_data.machine);
    } else {
        perror("Error getting system information");
    }

    return 0;
}

