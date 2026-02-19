#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 8080

int main() {
  char buffer[BUFFER_SIZE];
  char response[] = "HTTP/1.1 200 OK\r\n"
                    "Content-Type: text/html\r\n\r\n"
                    "<html><body><h1>Hello, World!</h1></body></html>";

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1) {
    perror("Error creating socket");
    return 1;
  }
  printf("Socket created successfully\n");

  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(PORT);
  addr.sin_addr.s_addr = htonl(INADDR_ANY);

  int bind_sock = bind(
      sockfd,
      (struct sockaddr *)&addr,
      sizeof(addr)
  );
  if(bind_sock == -1) {
      perror("Error binding socket");
      return 1;
  }
  printf("Socket bound successfully to address\n");

  int listen_socket = listen(sockfd, SOMAXCONN);
  if(listen_socket == -1) {
      perror("Error listening on socket");
      return 1;
  }
  printf("Socket listening on port 8080\n");

  while(1) {
      int client_sockfd = accept(sockfd, NULL, NULL);
      if(client_sockfd == -1) {
          perror("Error accepting connection");
          continue;
      }
      printf("Client connected = %d\n", client_sockfd);

      int valread = read(client_sockfd, buffer, BUFFER_SIZE);
      if (valread < 0) {
          perror("webserver (read)");
          continue;
      }

      int valwrite = write(client_sockfd, response, strlen(response));
      if (valwrite < 0) {
          perror("webserver (write)");
          continue;
      }

      if(close(client_sockfd) == -1) {
          perror("Error closing client socket");
          return 1;
      }
  }

  return 0;
}
