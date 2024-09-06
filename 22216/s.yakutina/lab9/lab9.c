#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(1);
    }

    int status;
    pid_t pid = fork();
    
    if (pid == -1){
        perror("fork");
        exit(1);
    } 

    else if (pid == 0) {
        execlp("cat", "cat", argv[1], (char *) 0);
        perror("execlp");
        exit(1);
    } 

    else {
        printf("This message is displayed before the child process is completed\n");
        if (wait(&status) == -1) {
            perror("wait");
            exit(1);
        }

        if (WIFEXITED(status)) {
            printf("The child process exit status is %d\n", WEXITSTATUS(status));
        } 

        else if (WIFSIGNALED(status)) {
            printf("The child process exited signal is %d\n", WTERMSIG(status));
        }
    }
    return 0;
}
