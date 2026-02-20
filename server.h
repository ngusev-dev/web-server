#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER_SIZE 1024
#define PORT 8080
#define DEFAULT_RESPONSE "HTTP/1.1 200 OK\r\n" \
                         "Content-Type: text/html\r\n\r\n" \
                         "<html><body><h1>Hello, World!</h1></body></html>"

int create_socket(void);
int bind_socket(int socketfd, struct sockaddr_in *addr);
int listen_socket(int socketfd);
int accept_client(int sockfd);
void handle_client(int client_sockfd);
int close_socket(int sockfd);
