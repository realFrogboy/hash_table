#ifndef LIST_H_
#define LIST_H_

#include "stack.h"
#include <math.h>

#ifndef CHECK_AND_REALLOC
#define CHECK_AND_REALLOC   if (strc->num_of_free_elem == 0) \
                                realloc_list(strc);          
#endif

#ifndef GET_IDX_OF_FREE_ELEM
#define GET_IDX_OF_FREE_ELEM    int idx = strc->free_elem.data[strc->free_elem.Size - 1]; \
                                stackPop(&strc->free_elem);                               \
                                strc->num_of_free_elem--;                             
#endif

#ifndef ASSERT
#define ASSERT(statement, text) do {                                                                                    \
                                     if (statement) {                                                                   \
                                        printf (" %s:%d, IN FUNCTION %s:\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);  \
                                        printf (#text);                                                                 \
                                    }                                                                                   \
                                } while (0)
#endif

#ifndef CALC_LIST_HASH
#define CALC_LIST_HASH  strc->arr_hash              = MurmurHash1(strc->arr, sizeof(strc->arr), SEED);   \
                        strc->num_of_free_elem_hash = strc->num_of_free_elem;                      \
                        strc->head_hash             = strc->head;                                  \
                        strc->tail_hash             = strc->tail;                                  \
                        strc->capacity_hash         = strc->capacity;
#endif

#ifndef CHECK_LIST
#define CHECK_LIST do {                                                                                         \
                        int error = listOK(strc);                                                               \
                        if (error){                                                                             \
                            printf (" %s:%d, IN FUNCTION %s:\n.", __FILE__, __LINE__, __PRETTY_FUNCTION__);     \
                            list_dump(strc, error);                                                             \
                        }                                                                                       \
                    } while(0);
#endif

struct Arr {
    double data;
    int next;
    int prev;
};

struct List {
    canary_t list_left_canary;

    struct Arr *arr;
    struct Stack free_elem;
    size_t num_of_free_elem;
    size_t head;
    size_t tail;
    size_t capacity;

    size_t num_of_free_elem_hash;
    size_t head_hash;
    size_t tail_hash;
    size_t capacity_hash;
    unsigned arr_hash;

    canary_t list_right_canary;
};

enum LIST_ERRORS{
    NO_LIST_ERRORS              = 0,
    VOID_LIST                   = 20102,
    LIST_CANARY_LEFT_ERROR      = 20202,
    LIST_CANARY_RIGHT_ERROR     = 20302,
    ARR_CANARY_LEFT_ERROR       = 20402,
    ARR_CANARY_RIGHT_ERROR      = 20502,
    ARR_HASH_ERROR              = 20602,
    NUM_OF_FREE_ELEM_HASH_ERROR = 20702,
    HEAD_HASH_ERROR             = 20802,
    TAIL_HASH_ERROR             = 20902,
    LIST_CAPACITY_HASH_ERROR    = 201002
};

const int size_of_list = 10;
const int resize_coeff = 2;

List* list_ctor(); 
void list_dtor(List *strc);
Arr* arr_ctor();

size_t idx_of_head(List *strc);
size_t idx_of_tail(List *strc);
size_t idx_of_next(List *strc, size_t idx);
size_t idx_of_prev(List *strc, size_t idx);

void ins_at_the_beg(List *strc, double val);
void ins_at_the_end(List *strc, double val);

void ins_before(List *strc, size_t elem, double val);
void ins_after(List *strc, size_t elem, double val);

void del_elem(List *strc, size_t elem);
void del_ALL_elem(List *strc);

void realloc_list(List *strc);

void sort_list_by_prev_slow_slow(List *strc);
int list_cmp(const void *a, const void *b);

size_t search_elem_slow_slow(List *strc, double val);
bool isequal(double a, double b);

void print_list(List *strc);
int listOK(List *strc);
void list_dump(List *strc, int error);
void make_graph_dump(List *strc);

#endif