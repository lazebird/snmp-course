#ifndef _DATA_H
#define _DATA_H

#include <arpa/inet.h>    /* u_long */
typedef struct tagEntry
{
  u_long grpid;
  u_long portid;
  u_long admin_enable;
  char type[40];
} port_t;

extern port_t tbl[];

port_t *get_data(int grpid, int portid);
port_t *getn_data(int grpid, int portid);

#endif