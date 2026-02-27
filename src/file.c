#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "file.h"

/*
 * Read a file and return a file buffer pointer.
 * If the file cannot be opened, return NULL.
 */
char* get_file_buffer(const char* filepath) {
    FILE* file = fopen(filepath, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* file_buffer = malloc(file_size + 1);

    if(file_buffer == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    int read_bytes = fread(file_buffer, 1, file_size, file);
    file_buffer[read_bytes] = '\0';

    fclose(file);

    return file_buffer;
}

/*
 * Create a filepath from a folder and filename.
 * If either folder or filename is NULL, return NULL.
 * After creating the filepath, free the memory allocated for the path.
 */
char* create_filepath(const char* folder, const char* filename){
    if(!folder || !filename) return NULL;

    size_t path_size = strlen(folder) + strlen(filename) + 2; // / + \0
    char* path = malloc(path_size);

    snprintf(path, path_size, "%s/%s", folder, filename);
    return path;
}
