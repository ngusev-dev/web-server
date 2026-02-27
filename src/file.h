#include <stdio.h>

#define STATIC_FILES_FOLDER "./static/"
#define CONFIG_FOLDER "./config/"

char* get_file_buffer(const char* filepath);
char* create_filepath(const char* folder, const char* filename);
