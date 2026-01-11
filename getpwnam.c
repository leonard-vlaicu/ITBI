#include <stdio.h>
#include <pwd.h>
#include <string.h>

void customGetpwnam(const char *name) {
    setpwent();

    const struct passwd *pw = getpwent();
    bool found = false;

    while (pw != NULL) {
        if (strcmp(pw->pw_name, name) == 0) {
            found = true;

            printf("%s\n", pw->pw_gecos);
            break;
        }

        pw = getpwent();
    }

    if (!found) {
        printf("User not found\n");
    }

    endpwent();
}


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s username\n", argv[0]);
        return 1;
    }

    const char* name = argv[1];

    customGetpwnam(name);
}