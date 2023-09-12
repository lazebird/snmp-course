#include <stdio.h>
#include "data.h"

port_t tbl[] = {{.grpid = 1, .portid = 1, .admin_enable = 2, .type = "type1"}, {.grpid = 1, .portid = 2, .admin_enable = 2, .type = "type2"}};

port_t *get_data(int grpid, int portid)
{
    for (int i = 0; i < sizeof(tbl) / sizeof(tbl[0]); i++) {
        if (tbl[i].grpid != grpid || tbl[i].portid != portid) continue;
        return &tbl[i];
    }
    return NULL;
}

port_t *getn_data(int grpid, int portid)
{
    for (int i = 0; i < sizeof(tbl) / sizeof(tbl[0]); i++) {
        if (tbl[i].grpid < grpid) continue;
        if (tbl[i].grpid == grpid && tbl[i].portid <= portid) continue;
        return &tbl[i];
    }
    return NULL;
}
