#ifndef _DATA_H_
#define _DATA_H_

#define PATH_LENGTH         256
#define FILE_NAME_LENGTH    50
#define NOSUCHINSTANCE      -1
#define REMOVEFILEFAILED    -2
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct scalarEntry {
    char version[30];
    char cwd[PATH_LENGTH];
}scalar_t;

typedef struct fileEntry {
    char fileName[FILE_NAME_LENGTH];
    int fileLen;
}file_t;

typedef struct _pathInfo {
    struct _pathInfo *next;
    file_t data;
} pathInfo_t;

void init_head_node(void);
int read_dir(void);
scalar_t *get_scalar(void);
file_t *get_data(char *fileName);
file_t *getn_data(char *fileName);
int destroy_file(char *fileName);
FILE *create_file(char *fileName);

#endif