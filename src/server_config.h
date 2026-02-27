#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <stdio.h>
#include <stdint.h>

typedef uint16_t port_t;

typedef struct RouteConfig {
    char* path;
    char* file;
} RouteConfig;

typedef struct ServerConfig {
    port_t* port;
    RouteConfig* routes;
    size_t routes_count;
} ServerConfig;

ServerConfig* load_server_config();
void free_config(ServerConfig* config);

#endif
