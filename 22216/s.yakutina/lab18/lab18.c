#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>
#include <unistd.h>  

#define MAX_PATH_LEN 2048

int main(int argc, char* argv[])
{

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file or directory> [file or directory ...]\n", argv[0]);
        exit(1);

    }

    for (int i = 1; i < argc; i++) {
        char* filename = argv[i];
        struct stat file_info;

        if (lstat(filename, &file_info) == -1) {
            perror(filename);
            continue;
        }

        mode_t mode = file_info.st_mode;
        printf(S_ISDIR(mode) ? "d" :
            S_ISREG(mode) ? "-" :
            S_ISFIFO(mode) ? "p" :
            S_ISCHR(mode) ? "c" :
            S_ISBLK(mode) ? "b" :
            S_ISLNK(mode) ? "l" :
            S_ISSOCK(mode) ? "s" :
            "?");
        printf((mode & S_IRUSR) ? "r" : "-");
        printf((mode & S_IWUSR) ? "w" : "-");
        printf((mode & S_ISUID) ? ((mode & S_IXUSR) ? "s" : "S") : ((mode & S_IXUSR) ? "x" : "-"));
        printf((mode & S_IRGRP) ? "r" : "-");
        printf((mode & S_IWGRP) ? "w" : "-");
        printf((mode & S_ISGID) ? ((mode & S_IXGRP) ? "s" : "S") : ((mode & S_IXGRP) ? "x" : "-"));
        printf((mode & S_IROTH) ? "r" : "-");
        printf((mode & S_IWOTH) ? "w" : "-");
        printf((mode & S_ISVTX) ? ((mode & S_IXOTH) ? "t" : "T") : ((mode & S_IXOTH) ? "x" : "-"));

        printf("  %-5u", file_info.st_nlink);

        struct passwd* pw = getpwuid(file_info.st_uid);
        struct group* grp = getgrgid(file_info.st_gid);

        if (pw) {
            printf("  %-10s", pw->pw_name);
        }
        else {
            printf("  %-10u", file_info.st_uid);
        }

        if (grp) {
            printf("  %-10s", grp->gr_name);
        }
        else {
            printf("  %-10u", file_info.st_gid);
        }
        

        if (S_ISREG(mode)) { 
            printf("  %-10lld", (long long int)file_info.st_size);
        }
        else {
            printf("  %-10s", "");
        }

        printf("  %.24s", ctime(&file_info.st_mtime));

        printf("  %s\n", basename(filename));

        if (S_ISLNK(mode)) {
            long max_len = pathconf(filename, _PC_SYMLINK_MAX);
            if (max_len == -1) {
                if (errno == 0) {
                    max_len = MAX_PATH_LEN; 
                } else {
                    perror("pathconf");
                    continue;
                }   
            }


            if (max_len > MAX_PATH_LEN) {
                max_len = MAX_PATH_LEN; 
            }

            char link_target[max_len + 1];

            ssize_t link_len = readlink(filename, link_target, max_len);
            if (link_len != -1) {
                link_target[link_len] = '\0';
                printf(" -> %s", link_target);
            } else {
                perror("readlink"); 
                continue;
            }

        }
        printf("\n");

    }
    exit(0);
}
