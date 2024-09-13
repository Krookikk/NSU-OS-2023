#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>
#include <errno.h>

#define BUFFER_SIZE 2048
#define MAX_CLIENTS 10

void sigcatch(int sig) {
    unlink("./socket");
    _exit(0);
}

int main() {
    char *sock_path = "./socket";
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    struct pollfd poll_fds[MAX_CLIENTS + 1];
    char buffer[BUFFER_SIZE];

    if ((server_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, sock_path, sizeof(server_addr.sun_path) - 1);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    signal(SIGINT, sigcatch);
    signal(SIGQUIT, sigcatch);
    
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        unlink(sock_path);
        perror("listen");
        exit(1);
    }

    poll_fds[0].fd = server_fd;
    poll_fds[0].events = POLLIN;

    for (int i = 1; i <= MAX_CLIENTS; i++) {
        poll_fds[i].fd = -1;
    } 

    while (1) {
        int poll_count = poll(poll_fds, MAX_CLIENTS + 1, -1);

        if (poll_count == -1) {
            unlink(sock_path);
            perror("poll");
            exit(1);
        }

        if (poll_fds[0].revents & POLLIN) {
            if ((client_fd = accept(server_fd, NULL, NULL)) == -1) {
                unlink(sock_path);
                perror("accept");
                exit(1);
            }

            for (int i = 1; i <= MAX_CLIENTS; i++) {
                if (poll_fds[i].fd == -1) {
                    poll_fds[i].fd = client_fd;
                    poll_fds[i].events = POLLIN;
                    break;
                }
            }
        }

        for (int i = 1; i <= MAX_CLIENTS; i++) {
            if (poll_fds[i].revents & POLLIN) {
                memset(buffer, 0, BUFFER_SIZE);
                int bytes_read = read(poll_fds[i].fd, buffer, BUFFER_SIZE);

                if (bytes_read > 0) {
                    for (int j = 0; j < bytes_read; j++) {
                        buffer[j] = toupper(buffer[j]);
                        printf("%c", buffer[j]);
                    }
                } else {
                    close(poll_fds[i].fd);
                    poll_fds[i].fd = -1;
                } 
            }
        }
    }
}
