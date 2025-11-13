#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/stat.h>
#include <string.h>

#include "buffer.h"
//#include "output.h"

size_t GetFileSize(const char *file_name) {

    assert(file_name);

    struct stat file_info = {};
    stat(file_name, &file_info);

    return file_info.st_size;
}

char *GetBuffer(const char *file_name, size_t *file_size) {

    assert(file_name);
    assert(file_size);

    size_t file_full_size = GetFileSize(file_name);
    char *buffer = (char *) calloc((file_full_size + 1), sizeof(char));
    buffer[file_full_size] = '\0';

    FILE *file_ptr = fopen(file_name, "r");

    *file_size = fread(buffer, sizeof(char), file_full_size, file_ptr);

    if (buffer[*file_size - 1] != '\n')
        buffer[*file_size] = '\n';

    buffer[*file_size + 1] = '\0';

    fclose(file_ptr);

    return buffer;
}

