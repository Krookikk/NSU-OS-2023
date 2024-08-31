#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

int main() {
    int fd[2]; 
    pid_t pid; 
    char buffer[BUFSIZ]; 

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
        ssize_t count; 

        while ((count = read(fd[0], buffer, BUFSIZ)) > 0) { 
            for (int i = 0; i < count; i++) {         
                printf("%c", toupper(buffer[i]));
            }
        }

        if (count == -1) {
            perror("read");
            exit(1);
        }

        close(fd[0]);  
    } 

    else { 
        close(fd[0]);

        char *text = "WwwwwwWWWW wwWwWw\n"; 
        if (write(fd[1], text, strlen(text)) == -1) {
            perror("write");
            exit(1);
        }

        close(fd[1]);
    }

    return 0;
}
