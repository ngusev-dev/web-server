#include <string.h>
#include <sys/socket.h>
#include <stdio.h>

#include "http.h"

/*
 * Send an HTTP response to the client.
 *
 * @param status The HTTP status code to send.
 * @param socketfd The socket file descriptor to send the response on.
 * @return the number of bytes sent on success, -1 on failure.
 */
int send_http_response(HTTP_STATUS status, int socketfd, char* body) {
    if(socketfd == -1) {
        perror("Invalid socket file descriptor");
        return -1;
    }

    const char* status_string = get_http_status_string(status);

    char* response_template = "%s"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s\r\n";

    char response[RESPONSE_SIZE];
    int response_size = snprintf(
        response,
        sizeof(response),
        response_template,
        status_string,
        strlen(body),
        body);

    printf("Response size: %d\n", response_size);

    int sent_bytes = send(socketfd, response, response_size, 0);

    if(sent_bytes == -1) {
        perror("Error sending HTTP response");
        return -1;
    }

    return sent_bytes;
}

/*
 * Return the string representation of an HTTP status code.
 */
const char* get_http_status_string(HTTP_STATUS status) {
    switch(status) {
        case HTTP_OK:                       return "HTTP/1.1 200 OK\r\n";
        case HTTP_CREATED:                  return "HTTP/1.1 201 Created\r\n";
        case HTTP_ACCEPTED:                 return "HTTP/1.1 202 Accepted\r\n";
        case HTTP_NO_CONTENT:               return "HTTP/1.1 204 No Content\r\n";
        case HTTP_BAD_REQUEST:              return "HTTP/1.1 400 Bad Request\r\n";
        case HTTP_UNAUTHORIZED:             return "HTTP/1.1 401 Unauthorized\r\n";
        case HTTP_FORBIDDEN:                return "HTTP/1.1 403 Forbidden\r\n";
        case HTTP_NOT_FOUND:                return "HTTP/1.1 404 Not Found\r\n";
        case HTTP_METHOD_NOT_ALLOWED:       return "HTTP/1.1 405 Method Not Allowed\r\n";
        case HTTP_INTERNAL_SERVER_ERROR:    return "HTTP/1.1 500 Internal Server Error\r\n";
        case HTTP_NOT_IMPLEMENTED:          return "HTTP/1.1 501 Not Implemented\r\n";
        case HTTP_BAD_GATEWAY:              return "HTTP/1.1 502 Bad Gateway\r\n";
        case HTTP_SERVICE_UNAVAILABLE:      return "HTTP/1.1 503 Service Unavailable\r\n";
        case HTTP_GATEWAY_TIMEOUT:          return "HTTP/1.1 504 Gateway Timeout\r\n";
        default:                            return "HTTP/1.1 500 Internal Server Error\r\n";
    }
}

/*
 * Parse the request path from the request buffer.
 *
 * @param request_buffer The request buffer.
 * @return The request path.
 */
const char* parse_request_path(char* request_buffer) {
    const char* start = strstr(request_buffer, "GET ");
    if (!start) start = strstr(request_buffer, "POST ");
    if (!start) start = strstr(request_buffer, "HEAD ");
    if (!start) return NULL;

    start += 4;

    const char* end = strstr(start, " HTTP/");
    if (!end) return NULL;

    size_t path_len = end - start;
    static char path[1024];
    if (path_len >= sizeof(path)) return NULL;

    strncpy(path, start, path_len);
    path[path_len] = '\0';

    return path;
}
