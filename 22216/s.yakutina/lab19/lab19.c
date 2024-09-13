#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <fnmatch.h>
#include <unistd.h>
#include <limits.h>
#include <errno.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pattern>\n", argv[0]);
        exit(1);
    }

    if (strchr(argv[1], '/') != NULL) {
        fprintf(stderr, "Pattern cannot contain the /\n");
        exit(1);
    }

    DIR* dir;

    if (!(dir = opendir("."))) {
        perror("Opendir failed");
        exit(1);
    }

    struct dirent* dir_ent;
    int count = 0;
    errno = 0;

    while ((dir_ent = readdir(dir)) != NULL) {
        if (fnmatch(argv[1], dir_ent->d_name, FNM_PATHNAME) == 0) {
            printf("%s\n", dir_ent->d_name);
            count++;
        }
    }

    if (errno != 0) {
        perror("Failed to read directory");
        exit(1);
    }

    if (count == 0) {
        printf("No files were found that match the pattern %s\n", argv[1]);
    }

    closedir(dir);
    return 0;
}
