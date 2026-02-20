#include "server.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    struct sockaddr_in addr;
    int socketfd;

    socketfd = create_socket();
    if (socketfd == -1) {
        return EXIT_FAILURE;
    }

    if (bind_socket(socketfd, &addr) == -1) {
        close_socket(socketfd);
        return EXIT_FAILURE;
    }

    if (listen_socket(socketfd) == -1) {
        close_socket(socketfd);
        return EXIT_FAILURE;
    }

    printf("Server started on port %d\n", PORT);


    while (1) {
        int client_socket = accept_client(socketfd);
        if (client_socket == -1) {
            continue;
        }
        handle_client(client_socket);

        if (close(client_socket) == -1) {
            perror("Error closing client socket");
        }
    }

    close_socket(socketfd);
    return EXIT_SUCCESS;
}
