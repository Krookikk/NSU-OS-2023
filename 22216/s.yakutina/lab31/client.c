#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define BUFFER_SIZE 1024

int main() {
    char *sock_path = "./socket";
    int client_fd, bytes_read;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    if ((client_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, sock_path, sizeof(server_addr.sun_path) - 1);

    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    while ((bytes_read = read(0, buffer, BUFFER_SIZE)) > 0) {
        if (write(client_fd, buffer, bytes_read) == -1) {
            perror("write");
            exit(1);
        }
    }

    return 0;
}
