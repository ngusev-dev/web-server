#ifndef SERVER_CONFIG_H
#define SERVER_CONFIG_H

#include <stdio.h>

typedef struct RouteConfig {
    char* path;
    char* file;
} RouteConfig;

typedef struct ServerConfig {
    RouteConfig* routes;
    size_t routes_count;
} ServerConfig;

ServerConfig* load_server_config();
void free_config(ServerConfig* config);

#endif
