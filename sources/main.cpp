#include "main.h"

int main() {
    struct hashTbl_t* tbl = tblCtor();

    struct aWord_t aWord = aWordCtor(file, tbl);

    int pos = tblSrch(tbl, "cat");
    printf("%d\n", pos);

    pos = tblSrch(tbl, "god");
    printf("%d\n", pos);

    for (int idx = 0; idx < tblSize; idx++) {
        int size = tbl->aTbl[idx]->capacity - tbl->aTbl[idx]->num_of_free_elem;
        printf("#%d - %d :: cap = %ld, free = %ld\n", idx, size, tbl->aTbl[idx]->capacity, tbl->aTbl[idx]->num_of_free_elem);
    }

    tblDtor(tbl);
    aWordDtor(&aWord);

    return 0;
}


aWord_t aWordCtor(const char *filename, hashTbl_t *tbl) {
    assert(filename);
    assert(tbl);

    struct aWord_t strc = {};

    strc.arr = (char**)calloc(maxNumWord, sizeof(char*));

    FILE *input = fopen(filename, "r");
    assert(input);

    char *word = "b";
    int cnt = 0;

    for (; strncmp(word, ".", 1); cnt++) {
        word = (char*)calloc(maxWordLen, sizeof(char));

        if ((fscanf(input, "%s", word)) != 1) 
            assert(0);

        tblIns(tbl, word);

        strc.arr[cnt] = word;        
    }
    strc.size = cnt;
    fclose(input);

    return strc;
}

void aWordDtor(aWord_t *strc) {
    assert(strc);

    for (int idx = 0 ; idx < strc->size; idx++)
        free(strc->arr[idx]);

    free(strc->arr);
}