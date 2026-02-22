#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server_config.h"
#include "file.h"
#include "cJSON.h"

/*
 * Loads the server configuration from a JSON file.
 */
ServerConfig* load_server_config() {
    char* config_buffer = get_file_buffer("config/web-server.config.json");
    if(config_buffer == NULL) {
        perror("Failed to read config file\n");
        return NULL;
    }

    cJSON* config_json = cJSON_Parse(config_buffer);
    free(config_buffer);

    if(config_json == NULL) {
        perror("Failed to parse config file\n");
        return NULL;
    }

    cJSON* config_routes = cJSON_GetObjectItem(config_json, "routes");

    if (!cJSON_IsArray(config_routes)) {
        perror("No 'routes' array found\n");
        cJSON_Delete(config_json);
        return NULL;
    }

    ServerConfig* config = malloc(sizeof(ServerConfig));
    if(config == NULL) {
        perror("Failed to allocate memory for server config\n");
        cJSON_Delete(config_json);
        return NULL;
    }

    if(cJSON_IsArray(config_routes)) {
        config->routes_count = cJSON_GetArraySize(config_routes);
        config->routes = malloc(config->routes_count * sizeof(RouteConfig));

        for(int i = 0; i < config->routes_count; i++) {
            cJSON* route_json = cJSON_GetArrayItem(config_routes, i);
            if(cJSON_IsObject(route_json)) {
                cJSON* path = cJSON_GetObjectItem(route_json, "path");
                cJSON* file = cJSON_GetObjectItem(route_json, "file");

                config->routes[i].path = malloc(strlen(path->valuestring) + 1);
                config->routes[i].file = malloc(strlen(file->valuestring) + 1);

                strcpy(
                    config->routes[i].path,
                    path ?  path->valuestring : NULL
                );
                strcpy(
                    config->routes[i].file ,
                    file ?  file->valuestring : NULL
                );

                printf("Route[%d]: path='%s' file='%s'\n", i,
                               config->routes[i].path, config->routes[i].file);
            }
        }
    }

    cJSON_Delete(config_json);
    return config;
}

/*
 * Frees the memory allocated for the server configuration.
 */
void free_config(ServerConfig* config) {
    if(config == NULL) {
        return;
    }

    for (size_t i = 0; i < config->routes_count; i++) {
        free(config->routes[i].path);
        free(config->routes[i].file);
    }

    free(config->routes);
    free(config);
}
