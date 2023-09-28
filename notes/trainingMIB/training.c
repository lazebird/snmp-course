/*
 * Note: this file originally auto-generated by mib2c
 * using mib2c.old-api.conf
 */

#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include "training.h"
#include "data.h"
#include "fileTrap.h"

/* 
 * training_variables_oid:
 *   this is the top level oid that we want to register under.  This
 *   is essentially a prefix, with the suffix appearing in the
 *   variable below.
 */

oid training_variables_oid[] = { 1,3,6,1,4,1,57430,1,3 };

/* 
 * variable4 training_variables:
 *   this variable defines function callbacks and type return information 
 *   for the training mib section 
 */

struct variable4 training_variables[] = {
/*  magic number        , variable type , ro/rw , callback fn  , L, oidsuffix */
#define TRAININGMODULE		1
{TRAININGMODULE,  ASN_INTEGER,  NETSNMP_OLDAPI_RONLY,
 var_training, 1,  { 1 }},
#define VERSION		2
{VERSION,  ASN_OCTET_STR,  NETSNMP_OLDAPI_RONLY,
 var_training, 1,  { 2 }},
#define CWD		3
{CWD,  ASN_OCTET_STR,  NETSNMP_OLDAPI_RWRITE,
 var_training, 1,  { 3 }},

#define FILENAME		1
{FILENAME,  ASN_OCTET_STR,  NETSNMP_OLDAPI_RONLY,
 var_fileTable, 3,  { 5 , 1, 1 }},
#define FILELEN		2
{FILELEN,  ASN_INTEGER,  NETSNMP_OLDAPI_RONLY,
 var_fileTable, 3,  { 5 , 1, 2 }},
#define FILEROWSTATUS		3
{FILEROWSTATUS,  ASN_INTEGER,  NETSNMP_OLDAPI_RWRITE,
 var_fileTable, 3,  { 5 , 1, 3 }},
};
/*    (L = length of the oidsuffix) */


/** Initializes the training module */
void
init_training(void)
{

    DEBUGMSGTL(("training", "Initializing\n"));

    /* register ourselves with the agent to handle our mib tree */
    REGISTER_MIB("training", training_variables, variable4,
               training_variables_oid);
    /* place any other initialization junk you need here */
    init_head_node();
    read_dir();
}

/*
 * var_training():
 *   This function is called every time the agent gets a request for
 *   a scalar variable that might be found within your mib section
 *   registered above.  It is up to you to do the right thing and
 *   return the correct value.
 *     You should also correct the value of "var_len" if necessary.
 *
 *   Please see the documentation for more information about writing
 *   module extensions, and check out the examples in the examples
 *   and mibII directories.
 */
unsigned char *
var_training(struct variable *vp, 
                oid     *name, 
                size_t  *length, 
                int     exact, 
                size_t  *var_len, 
                WriteMethod **write_method)
{
    /* variables we may use later */
    static long long_ret;
    static scalar_t *data;
    // pathInfo_t *info;

    if (header_generic(vp,name,length,exact,var_len,write_method)
                                  == MATCH_FAILED )
    return NULL;

    data = get_scalar();
    switch(vp->magic) {
    case VERSION:
        *var_len = strlen(data->version);
        return (u_char*) data->version;
    case CWD:
        *write_method = write_cwd;
        *var_len = strlen(data->cwd);
        return (u_char*) data->cwd;
    default:
      ERROR_MSG("");
    }
    return NULL;
}

static int oid2name(struct variable *vp, oid *name, size_t *length, char *buf)
{
    int k = 0;
    static int vp_name_len;

    if (vp) {
        vp_name_len = vp->namelen;
    }

    if (*length <= vp_name_len) {
        return 1;
    }

    for (int i = vp_name_len; i < *length && *length < FILE_NAME_LENGTH; i++) {
        buf[k++] = (char)name[i];   /* 强制转换，否则编译时会警告 */
    }

    buf[k] = 0;

    return 1;
}

static int name2oid(struct variable *vp, oid *name, size_t *length, char *buf) {
    int k;

    // memset(name, 0, sizeof(name));
    memcpy(name, vp->name,  vp->namelen * sizeof(name[0]));
    
    for (int i = 0, k = vp->namelen; i < strlen(buf) && buf[i] != 0; i++) {
        name[k++] = buf[i];
    }

    *length = vp->namelen + strlen(buf);

    return 1;
}

static char *oid2str(oid *name, int length)
{
#define BUFNUM 4
#define BUFLEN 512
    static char buf[BUFNUM][BUFLEN];
    static int bufid = 0;
    char *p = buf[bufid];
    int offset = 0;

    for (int i = 0; i < length; i++) {
        offset += snprintf(p + offset, BUFLEN - offset, ".%ld", name[i]);
    }

    bufid = (bufid + 1) % BUFNUM;

    return p;
}

/*
 * var_fileTable():
 *   Handle this table separately from the scalar value case.
 *   The workings of this are basically the same as for var_training above.
 */
unsigned char *
var_fileTable(struct variable *vp,
    	    oid     *name,
    	    size_t  *length,
    	    int     exact,
    	    size_t  *var_len,
    	    WriteMethod **write_method)
{
    /* variables we may use later */
    static long long_ret;
    static file_t *fdata;
    char buf[FILE_NAME_LENGTH];
    memset(buf, 0, sizeof(buf));
    
    oid2name(vp, name, length, buf);
    printf("\nvp->name %s, name %s, length %d, vp->namelen %d, exact %d, magic %d, buf %s\n", oid2str(vp->name, vp->namelen), oid2str(name, *length), *length, vp->namelen, exact, vp->magic, buf);
    fdata = exact ? get_data(buf) : getn_data(buf);

    if (!fdata && vp->magic != FILEROWSTATUS) {
        printf("invalid name %s\n", buf);
        return NULL;
    }

    if (!exact && fdata) {
        name2oid(vp, name, length, fdata->fileName);
    }

    switch(vp->magic) {
    case FILENAME:
        *var_len = strlen(fdata->fileName);
        return (u_char*) &fdata->fileName;
    case FILELEN:
        *var_len = sizeof(fdata->fileLen);
        return (u_char*) &fdata->fileLen;
    case FILEROWSTATUS:         /* 存在active，不存在destroy，不需要保存值 */
        *write_method = write_fileRowStatus;
        long_ret = 1;
        *var_len = sizeof(long);
        return (u_char*) &long_ret;
    default:
      ERROR_MSG("");
    }
    return NULL;
}



int
write_cwd(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    char value;
    int size;

    if (action != COMMIT) return SNMP_ERR_NOERROR;
    if (var_val_type != ASN_OCTET_STR) return SNMP_ERR_WRONGTYPE;
    if (var_val_len > PATH_LENGTH) return SNMP_ERR_WRONGLENGTH;
    
    scalar_t *p = get_scalar();
    // printf("p %s, var_val %s, var_val_len %d, PATH_LENGTH %d\n", p->cwd, var_val, var_val_len, PATH_LENGTH);
    memcpy(p->cwd, var_val, MIN(var_val_len, PATH_LENGTH));
    p->cwd[var_val_len] = 0;
    read_dir();

    return SNMP_ERR_NOERROR;
}

int
write_fileRowStatus(int      action,
            u_char   *var_val,
            u_char   var_val_type,
            size_t   var_val_len,
            u_char   *statP,
            oid      *name,
            size_t   name_len)
{
    long value;
    int ret;
    char buf[FILE_NAME_LENGTH];
    file_t *p;

    if (action != COMMIT) {
        return SNMP_ERR_NOERROR;
    }

    if (var_val_type != ASN_INTEGER) {
        return SNMP_ERR_WRONGTYPE;
    }

    if (var_val_len != sizeof(long)) {
        return SNMP_ERR_WRONGLENGTH;
    }

    value = *(long *)var_val;
    if (value != ACTIVE && value != NOREADY && value != CREATEANDWAIT && value != DESTROY) {
        return SNMP_ERR_BADVALUE;
    }

    oid2name(NULL, name, &name_len, buf);
    printf("var_val %s, var_val_type %d, var_val_len %d, name %s, namelen %d, buf %s\n", oid2str(var_val, var_val_len), var_val_type, var_val_len, oid2str(name, name_len), name_len, buf);
    
    switch (value) {
    case ACTIVE:
        p = get_data(buf);
        if (p) {
            printf("no change\n");
            return SNMP_ERR_NOERROR;
        }

        create_file(buf);
        send_fileChange_trap(buf, value);
        read_dir();
        break;
    case DESTROY:
        ret = destroy_file(buf);

        if (ret == NOSUCHINSTANCE) {
            return SNMP_NOSUCHINSTANCE;
        } else if (ret == REMOVEFILEFAILED) {
            printf("destroy error\n");
            return SNMP_ERR_BADVALUE;
        } else if (ret == 0) {
            send_fileChange_trap(buf, value);
            read_dir();
        } else {
            return SNMP_NOSUCHINSTANCE;
        }
        
        break;
    default:
        printf("error");
        break;
    }

    return SNMP_ERR_NOERROR;
}