#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define SOCKET_PATH "sock.server"
#define BUFFER_SIZE 256

int main() {
    int client_sock;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    if ((client_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    if (connect(client_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("connect");
        exit(1);
    }

    int received_bytes;
    while((received_bytes = read(0, buffer, BUFFER_SIZE)) > 0) {
        if (write(client_sock, buffer, received_bytes) == -1) {
            perror("write");
            exit(1);
        }
    }

    if (received_bytes == -1) {
        perror("read");
        exit(1);
    }

    close(client_sock);

    return 0;
}
