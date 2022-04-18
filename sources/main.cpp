#include "hashTbl.h"

int main() {
    struct hashTbl_t* tbl = tblCtor();

    tblIns(tbl, "cat");
    tblIns(tbl, "hause");
    tblIns(tbl, "people");
    tblIns(tbl, "road");

    int pos = tblSrch(tbl, "cat");
    printf("%d\n", pos);

    pos = tblSrch(tbl, "god");
    printf("%d\n", pos);

    tblDtor(tbl);

    return 0;
}