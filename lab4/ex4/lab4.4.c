#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <unistd.h>

int main() {
    // Get information about the current user by UID
    uid_t current_user_id = getuid();
    struct passwd *current_user_info = getpwuid(current_user_id);

    if (current_user_info) {
        printf("Information about the current user by UID:\n");
        printf("Username: %s\n", current_user_info->pw_name);
        printf("Full name: %s\n", current_user_info->pw_gecos);
    } else {
        perror("Error retrieving user information by UID");
    }

    // Get information about the "root" user by username
    const char *root_username = "root";
    struct passwd *root_user_info = getpwnam(root_username);

    if (root_user_info) {
        printf("\nInformation about the 'root' user by username:\n");
        printf("Username: %s\n", root_user_info->pw_name);
        printf("Full name: %s\n", root_user_info->pw_gecos);
    } else {
        perror("Error retrieving user information for the 'root' user by username");
    }

    return 0;
}

