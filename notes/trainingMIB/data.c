#include <time.h>
#include <stdio.h>
#include <dirent.h>    /* DIR */
#include <sys/types.h> /* int stat() */
#include <sys/stat.h>  /* struct stat */
#include "data.h"

#define VERSION 2

time_t t;
pathInfo_t *info = {0};
scalar_t sdata = {};

void init_head_node(void)
{

    pathInfo_t *head_info;

    head_info = (pathInfo_t *)malloc(sizeof(pathInfo_t));
    if (!head_info) {
        return NULL;
    }

    head_info->next = NULL;
    info = head_info;
    getcwd(sdata.cwd, PATH_LENGTH);
    init_version();

    return ;
}

void free_memory(pathInfo_t *path_info)
{
    if (!path_info) {
        return;
    }

    pathInfo_t *info1 = path_info;
    pathInfo_t *info2;

    while (info1) {
        info2 = info1->next;
        info1->next = NULL;
        free(info1);
        info1 = info2;
    }
}

int read_dir(void)
{
    pathInfo_t * tmp = info;
    info = (pathInfo_t *)malloc(sizeof(pathInfo_t));
    free_memory(tmp);

    pathInfo_t * path_info = info;
    const char *dir_path = &sdata.cwd;
    struct stat sta;
    char path[PATH_LENGTH];
    pathInfo_t *pre = NULL, *last = NULL;
    struct dirent **name_list;
    int ret_val = 0;

    path_info->next = last;
    pre = path_info;

    if (!dir_path || !path_info) {
        return -1;
    }

    int n = scandir(dir_path, &name_list, 0, alphasort);
    if (n < 0) {
        printf("scandir error, return %d\n", n);
        return -2;
    }

    for (int i = 2; i < n; i++) {
        memset(path, 0, sizeof(path));
        strcpy(path, dir_path);
        if (path[strlen(path) - 1] != '/') {
            strcat(path, "/");
        }

        strcat(path, name_list[i]->d_name);

        if (stat(path, &sta)) { /* 获得文件大小和类型等 */
            ret_val = -3;
            printf("error, stat\n");
            return ret_val;
        }

        // printf("name %s, ino %d, mode %d, size %d\n", entry->d_name, sta.st_ino, sta.st_mode, sta.st_size);
        last = (pathInfo_t *)malloc(sizeof(pathInfo_t));
        last->next = NULL;
        memset(last->data.fileName, 0, sizeof(last->data.fileName));
        memcpy(last->data.fileName, name_list[i]->d_name, MIN(strlen(name_list[i]->d_name), PATH_LENGTH));
        last->data.fileLen = sta.st_size;
        pre->next = last;
        pre = pre->next;
        // printf("%s %d, ", last->data.fileName, strlen(name_list[i]->d_name));
    }

    return ret_val;
}

void init_version(void)
{
    time(&t);
    snprintf(sdata.version, 30, "%s", ctime(&t));
}

scalar_t *get_scalar(void)
{
    return &sdata;
}

file_t *get_data(char *fileName)
{
    pathInfo_t *tmp;
    tmp = info->next;

    while (tmp) {
        if (strcmp(tmp->data.fileName, fileName) == 0) {
            return &tmp->data;
        }

        tmp = tmp->next;
    }

    return NULL;
}

file_t *getn_data(char *fileName)
{
    pathInfo_t *tmp;
    tmp = info->next;

    while (tmp) {
        if (strcmp(tmp->data.fileName, fileName) > 0) {
            return &tmp->data;
        }
        // printf("%s %d, ", tmp->data.fileName, strlen(tmp->data.fileName));
        tmp = tmp->next;
    }

    // putchar(10);
    return NULL;
}

int destroy_file(char *fileName)
{
    pathInfo_t *tmp;

    tmp = info->next;

    while (strcmp(tmp->data.fileName, fileName) != 0) {
        tmp = tmp->next;

        if (tmp == NULL) {
            return NOSUCHINSTANCE;
        }
    }

    if (remove(fileName) == 0) {
        return 0;
    }
    else {
        return REMOVEFILEFAILED;
    }
}

FILE *create_file(char *fileName)
{
    FILE *fp;
    fp = fopen(fileName, "w");
    return fp;
}
