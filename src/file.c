#include <stdio.h>
#include <stdlib.h>

#include "file.h"

/*
 * Read a file and return a file buffer pointer.
 * If the file cannot be opened, return NULL.
 */
char* get_file_buffer(char* filename) {
    char path[512];
    snprintf(path, sizeof(path), "%s/%s", STATIC_FILES_FOLDER, filename);

    FILE* file = fopen(path, "r");
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
