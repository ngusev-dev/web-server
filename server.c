#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "server.h"

char buffer[BUFFER_SIZE];

int create_socket(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if(sockfd == -1) {
      perror("Error creating socket");
      return -1;
    }

    printf("Socket created successfully\n");

    return sockfd;
}

int bind_socket(int socketfd, struct sockaddr_in *addr) {
    addr->sin_family = AF_INET;
    addr->sin_port = htons(PORT);
    addr->sin_addr.s_addr = htonl(INADDR_ANY);

    int bind_result = bind(socketfd, (struct sockaddr *)addr, sizeof(*addr));

    if(bind_result == -1) {
        perror("Error binding socket");
        return -1;
    }

    printf("Socket bound successfully to address\n");

    return 0;
}

int listen_socket(int socketfd) {
    int listen_result = listen(socketfd, SOMAXCONN);

    if(listen_result == -1) {
        perror("Error listening on socket");
        return -1;
    }

    printf("Socket listening on port %d\n", PORT);

    return 0;
}

int accept_client(int sockfd) {
    int client_sockfd = accept(sockfd, NULL, NULL);
    if(client_sockfd == -1) {
        perror("Error accepting connection");
        return -1;
    }
    printf("Client connected = %d\n", client_sockfd);

    return client_sockfd;
}

void handle_client(int client_sockfd) {
    int valread = read(client_sockfd, buffer, BUFFER_SIZE);
    if (valread < 0) {
        perror("webserver (read)");
        return;
    }
    int valwrite = write(client_sockfd, DEFAULT_RESPONSE, strlen(DEFAULT_RESPONSE));
    if (valwrite < 0) {
        perror("webserver (write)");
        return;
    }
}

int close_socket(int sockfd) {
    if (close(sockfd) == -1) {
        perror("Error closing socket");
        return -1;
    }

    printf("Socket closed successfully\n");
    return 0;
}
