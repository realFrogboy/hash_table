#include "hashTbl.h"

hashTbl_t* tblCtor() {
    struct hashTbl_t* tbl = (hashTbl_t*)calloc(1, sizeof(hashTbl_t));

    tbl->aStr = (char**)calloc(startSize, sizeof(char*));
    tbl->aStrCapacity = startSize;
    tbl->aStrSize = 0;

    for (int idx = 0; idx < tblSize; idx++) {
        struct List *elem = list_ctor();
        tbl->aTbl[idx] = elem;
    }

    return tbl;
}

void tblDtor(hashTbl_t *tbl) {
    assert(tbl);

    free(tbl->aStr);

    for (int idx = 0; idx < tblSize; idx++) 
        list_dtor(tbl->aTbl[idx]);

    free(tbl);
    return;
}

int tblSrch(hashTbl_t *tbl, char *word) {
    assert(tbl);
    assert(word);

    int lenWord = strlen(word);
    int wordHash = hash0(word, lenWord, SEED) % tblSize;
    int lstSize = tbl->aTbl[wordHash]->capacity - tbl->aTbl[wordHash]->num_of_free_elem;

    for (int idx = 1; idx < lstSize + 1; idx++) {
        int lenElem = strlen(tbl->aStr[int(tbl->aTbl[wordHash]->arr[idx].data)]);
        int len = 0;

        (lenElem > lenWord) ? len = lenWord : len = lenElem;

        if (!strncmp(tbl->aStr[int(tbl->aTbl[wordHash]->arr[idx].data)], word, len))
            return wordHash;
    }

    return -wordHash - 1;
}

void tblIns(hashTbl_t *tbl, char *word) {
    assert(tbl);
    assert(word);

    int wordHash = tblSrch(tbl, word);
    if (wordHash >= 0)
        return; 

    wordHash = -wordHash -1;

    tbl->aStr[tbl->aStrSize] = word;
    ins_at_the_end(tbl->aTbl[wordHash], tbl->aStrSize);
    tbl->aStrSize++;

    if (tbl->aStrSize == tbl->aStrCapacity) {
        tbl->aStrCapacity *= resizeCoeff;

        char **tmp = (char**)realloc(tbl->aStr, tbl->aStrCapacity * sizeof(char*));
        if (tmp == NULL) 
            assert(0);

        tbl->aStr = tmp;
    }

    return;
}