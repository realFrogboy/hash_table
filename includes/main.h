#ifndef MAIN_H_
#define MAIN_H_

#include "hashTbl.h"

struct aWord_t {
    char **arr;
    int size;
};

const char *file = "/mnt/c/code/hash_table/text.txt";
const int maxWordLen = 25;
const int maxNumWord = 1000;

aWord_t aWordCtor(const char *filename, hashTbl_t *tbl);
void    aWordDtor(aWord_t *strc);

#endif