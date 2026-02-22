#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#include "server.h"
#include "http.h"
#include "file.h"

char request_buffer[BUFFER_SIZE];

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

void handle_client(int client_sockfd, ServerConfig* config) {
    int valread = read(client_sockfd, request_buffer, BUFFER_SIZE);
    if (valread < 0) {
        perror("Error reading client request");
        return;
    }

    const char* path = parse_request_path(request_buffer);
    if(path == NULL) return;

    int is_route_found = 0;
    for(int i = 0; i < config->routes_count; i++){
        printf("Comparing '%s' with config '%s'\n", path, config->routes[i].path);

        if(strcmp(path, config->routes[i].path) == 0){
            char* file_buffer_ptr = get_file_buffer(config->routes[i].file);
            int send_bytes = send_http_response(HTTP_OK, client_sockfd, file_buffer_ptr);
            printf("Bytes sent: %d\n", send_bytes);

            free(file_buffer_ptr);
            if (send_bytes == -1) {
                perror("Error sending HTTP response");
                break;
            }
            is_route_found = 1;
            break;
        }
    }

    if (!is_route_found) {
        printf("No route found for path: '%s'\n", path);
        char* file_buffer_ptr = get_file_buffer("index.html");
        send_http_response(HTTP_NOT_FOUND, client_sockfd, file_buffer_ptr);
        free(file_buffer_ptr);
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
