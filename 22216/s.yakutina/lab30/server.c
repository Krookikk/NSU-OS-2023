#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>

#define SOCKET_PATH "sock.server" 
#define BUFFER_SIZE 256

int main() {
    int server_sock, client_sock;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    if ((server_sock = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);

    unlink(SOCKET_PATH);

    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(server_sock, 1) == -1) {
        perror("listen");
        exit(1);
    }

    if ((client_sock = accept(server_sock, NULL, NULL)) == -1) {
        perror("accept");
        exit(1);
    }

    int received_bytes;
    while ((received_bytes = read(client_sock, buffer, BUFFER_SIZE)) > 0) {
        for (int i = 0; i < received_bytes; i++) {
            buffer[i] = toupper(buffer[i]);
            printf("%c", buffer[i]);
        }
    }

    if (received_bytes == -1) {
        perror("read");
        exit(1);
    }

    close(client_sock);
    close(server_sock);
    unlink(SOCKET_PATH);

    return 0;
}
