#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main() {
    int fd[2]; 
    pid_t pid; 
    char buffer_p[BUFSIZ];
    char buffer_s[BUFSIZ]; 

    if (pipe(fd) == -1) { 
        perror("pipe");
        exit(1);
    }

    pid = fork();   

    if (pid == -1) {
        perror("fork");
        exit(1);
    } 

    else if (pid == 0) {
        close(fd[1]);
        ssize_t count_p; 

        while ((count_p = read(fd[0], buffer_p, BUFSIZ)) > 0) { 
            for (int i = 0; i < count_p; i++) {         
                printf("%c", toupper(buffer_p[i]));
            }
        }

        if (count_p == -1) {
            perror("read");
            exit(1);
        }

        close(fd[0]);  
    } 

    else { 
        close(fd[0]);

        ssize_t count_s; 
        while ((count_s = read(STDIN_FILENO, buffer_s, BUFSIZ)) > 0) {
            if (write(fd[1], buffer_s, count_s) == -1) {
                perror("write");
                exit(1);
            }
        }

        if (count_s == -1) {
            perror("read");
            exit(1);
        }

        close(fd[1]);
    }

    return 0;
}
