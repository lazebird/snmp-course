/*
 * Note: this file originally auto-generated by mib2c
 * using mib2c.old-api.conf
 */
#ifndef TRAINING_H
#define TRAINING_H

#define ACTIVE          1
#define NOREADY         3
#define CREATEANDWAIT   5
#define DESTROY         6

/* function declarations */
void init_training(void);
FindVarMethod var_training;
FindVarMethod var_fileTable;
    WriteMethod write_cwd;
    WriteMethod write_fileRowStatus;

#endif /* TRAINING_H */
