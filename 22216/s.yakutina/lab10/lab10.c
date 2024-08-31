#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [arguments...]\n", argv[0]);
        exit(1);
    }

    int status;
    pid_t pid = fork();
    
    if (pid == -1){
        perror("fork");
        exit(1);
    } 

    else if (pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("execvp");
        exit(1);
    } 

    else {
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
