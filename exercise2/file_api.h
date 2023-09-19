#ifndef _FILE_API_H_
#define _FILE_API_H_
#include <stdio.h>

#define MAX_PATH_LEN 256
#define MAX_FILENAME_LEN 128

typedef struct {
    char name[MAX_FILENAME_LEN];
    int len;
    // int status;
} fileinfo_t;

char *version_get();

char *cwd_get();
int cwd_set(char *path);

void file_free(fileinfo_t **p); // attention: only free, no change to pointer
fileinfo_t **file_read(char *path);
fileinfo_t *file_get(fileinfo_t **p, char *name);
fileinfo_t *file_getn(fileinfo_t **p, char *name);
int file_set(char *name, int status);

#endif
