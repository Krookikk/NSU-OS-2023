#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
    int desc = fileno(stdin);
    if (isatty(desc) == 0) {
        fprintf(stderr, "stdin not terminal\n");
        exit(1);
    }

    struct termios past_term;
    if (tcgetattr(desc, &past_term) == -1) {
        perror("attributes cannot be set");
        exit(1);
    }

    struct termios cur_term = past_term;
    cur_term.c_lflag &= ~(ICANON);
    cur_term.c_cc[VMIN] = 1;

    if (tcsetattr(desc, TCSANOW, &cur_term) == -1) {
        perror("attributes cannot be set");
        exit(1);
    }

    char inp;
    printf("print one character\n");

    if (read(desc, &inp, 1) == -1){
        perror("cannot read a symbol");
        exit(1);
    }

    printf("\nexcellent\n");

    if (tcsetattr(desc, TCSANOW, &past_term) == -1) {
        perror("attributes cannot be set");
        exit(1);
    }

    return 0;
}
