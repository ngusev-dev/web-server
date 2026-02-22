#include <sys/socket.h>
#include <netinet/in.h>

#include "server_config.h"

#define BUFFER_SIZE 1024
#define PORT 8080

int create_socket(void);
int bind_socket(int socketfd, struct sockaddr_in *addr);
int listen_socket(int socketfd);
int accept_client(int sockfd);
void handle_client(int client_sockfd, ServerConfig* config);
int close_socket(int sockfd);
