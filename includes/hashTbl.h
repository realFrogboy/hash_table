#ifndef HASHTBL_H_
#define HASHTBL_H_

#include "list.h"
#include "hash.h"

const int resizeCoeff = 2;
const int startSize = 50;
const int tblSize = 50;
const int SEED = 1;

struct hashTbl_t {
    List *aTbl[tblSize];
    char **aStr;
    size_t aStrSize;
    size_t aStrCapacity;
};

hashTbl_t* tblCtor();
void tblDtor(hashTbl_t *tbl);
int tblSrch(hashTbl_t *tbl, char *word);
void tblIns(hashTbl_t *tbl, char *word);

#endif