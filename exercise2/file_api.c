#include <stdlib.h>
#include <time.h>
#include <limits.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "file_api.h"

char cwd[PATH_MAX] = ".";

char *version_get()
{
    static char s[80];
    time_t tt = time(NULL);
    strftime(s, sizeof(s), "fake version: %Y-%m-%d %H:%M:%S", localtime(&tt));
    return s;
}
char *cwd_get()
{
    return cwd;
}
int cwd_set(char *path)
{
    return realpath(path, cwd) != NULL; // attention: cwd[] 's size should be PATH_MAX
}

static long filesize(char *path, char *name)
{
    struct stat statbuf = {.st_size = 0};
    char buf[MAX_PATH_LEN];
    snprintf(buf, sizeof(buf), "%s/%s", path, name);
    return stat(buf, &statbuf) ? 0 : statbuf.st_size;
}

void file_free(fileinfo_t **p) // attention: only free, no change to pointer
{
    if (!p) return;
    for (fileinfo_t *q = *p; q; q++) free(q); // free NULL is ok
    free(p);
}
void file_dump(fileinfo_t **p)
{
    printf("dumping fileinfos %p:\n", p);
    for (int i = 0; p[i]; i++) printf("name %s, len %d\n", p[i]->name, p[i]->len);
}

static int cmp(const void *p1, const void *p2)
{
    return strcmp((*(fileinfo_t **)p1)->name, (*(fileinfo_t **)p2)->name);
}

fileinfo_t **file_read(char *path)
{
    fileinfo_t **p = NULL;
    int i, count = 0;
    struct dirent *ent;
    DIR *dir;
    // printf("read cwd %s\n", path);
    if (!(dir = opendir(path))) return NULL;
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type & DT_DIR) continue;
        count++;
    }
    if (count <= 0) goto _error;
    p = calloc(1, (count + 1) * sizeof(p));
    if (!p) goto _error;
    for (i = 0; i < count; i++)
        if (!(p[i] = malloc(sizeof(**p)))) goto _error;
    i = 0;
    rewinddir(dir);
    while ((ent = readdir(dir)) != NULL) {
        if (ent->d_type & DT_DIR) continue;
        snprintf(p[i]->name, sizeof(p[i]->name), "%s", ent->d_name);
        p[i]->len = filesize(path, ent->d_name);
        // printf("get file %s, len %d\n", ent->d_name, p[i]->len);
        i++;
    }
    closedir(dir);
    qsort(p, count, sizeof(*p), cmp);
    file_dump(p);
    return p;
_error:
    closedir(dir);
    file_free(p);
    return NULL;
}

fileinfo_t *file_get(fileinfo_t **p, char *name)
{
    for (int i = 0; p[i]; i++)
        if (!strcmp(p[i]->name, name)) return p[i];
    return NULL;
}

fileinfo_t *file_getn(fileinfo_t **p, char *name)
{
    for (int i = 0; p[i]; i++)
        if (strcmp(p[i]->name, name) > 0) return p[i];
    return NULL;
}

int file_set(char *name, int status)
{
    char path[MAX_PATH_LEN];
    snprintf(path, sizeof(path), "%s/%s", cwd, name);
    if (status == 1) { // create
        FILE *fp = fopen(path, "a");
        if (fp) fclose(fp);
        return !!fp;
    }
    if (status == 6) { // destroy
        return remove(path) == 0;
    }
    return 0; // unsupported
}