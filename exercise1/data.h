#ifndef _DATA_H_
#define _DATA_H_

typedef struct {
    int grpid;
    int portid;
    long admin_enable;
    char type[40];
} port_t;

extern port_t tbl[];

#endif
