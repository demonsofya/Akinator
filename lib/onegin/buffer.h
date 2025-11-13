#ifndef BUFFER_H_INCLUDED
#define BUFFER_H_INCLUDED

#include <stdlib.h>
#include <stdio.h>

size_t GetFileSize(const char *file_name);
char *GetBuffer(const char *file_name, size_t *file_size);

#endif // BUFFER_H_INCLUDED
