#include "list.h"
#include "hash.h"

const uint32_t SEED = 2; // for hash

List* list_ctor(){
    struct List *strc = (List*)calloc(1, sizeof(List));

    strc->arr = arr_ctor();

    stackCtor(&strc->free_elem);
    for (int num = size_of_list; num >= 1; num--){
        stackPush(&strc->free_elem, num);
    }

    strc->num_of_free_elem = size_of_list;
    strc->head = 1;
    strc->tail = 0;
    strc->capacity = size_of_list;

    strc->list_left_canary  = CANARY;
    strc->list_right_canary = CANARY;

    strc->arr_hash               = MurmurHash1(strc->arr, sizeof(strc->arr), SEED);  
    strc->num_of_free_elem_hash  = strc->num_of_free_elem;
    strc->head_hash              = strc->head;
    strc->tail_hash              = strc->tail;
    strc->capacity_hash          = strc->capacity;

    return strc;
}

void list_dtor(List *strc){
    CHECK_LIST;
    ASSERT(strc->capacity == 0, "Repearted list_dtor");


    strc->arr = (Arr*)((canary_t*)strc->arr - 1);
    free(strc->arr);

    stackDtor(&strc->free_elem);

    strc->head = -1;
    strc->tail = -1;
    strc->capacity = 0;

    free(strc);
}

Arr *arr_ctor(){
    struct Arr *arr = (Arr *)calloc((size_of_list + 1) * sizeof(Arr) + 2 * sizeof(canary_t), sizeof(char));
    ASSERT(arr == NULL, "Can't alloc memeory");

    *(canary_t*)arr = CANARY;
    arr = (Arr*)((canary_t*)arr + 1);
    *(canary_t*)(arr + size_of_list + 1) = CANARY;

    for (int idx = 0; idx <= size_of_list; idx++) {
        arr[idx].data = 0;
        arr[idx].next = -1;
        arr[idx].prev = -1;
    }

    arr[0].next = 0;
    arr[0].prev = 0;

    return arr;
}


//-----------------------------------------------------------------------------


size_t idx_of_head(List *strc){
    CHECK_LIST;
    return strc->head;
}

size_t idx_of_tail(List *strc){
    CHECK_LIST;
    return strc->tail;
}

size_t idx_of_next(List *strc, size_t idx){
    CHECK_LIST;
    ASSERT(idx <= 0, "Index <= 0");
    return strc->arr[idx].next;
}

size_t idx_of_prev(List *strc, size_t idx){
    CHECK_LIST;
    ASSERT(idx <= 0, "Index <= 0");
    return strc->arr[idx].prev;
}


//-----------------------------------------------------------------------------


void ins_at_the_beg(List *strc, double val){
    CHECK_LIST;

    CHECK_AND_REALLOC;

    GET_IDX_OF_FREE_ELEM;

    strc->arr[idx].data = val;
    strc->arr[idx].next = strc->head;
    strc->arr[idx].prev = 0;

    strc->arr[strc->head].prev = idx;

    strc->arr[0].next = idx;

    strc->head = idx;

    CALC_LIST_HASH;

    CHECK_LIST;
}

void ins_at_the_end(List *strc, double val){
    CHECK_LIST;

    CHECK_AND_REALLOC;

    GET_IDX_OF_FREE_ELEM;

    strc->arr[idx].data = val;
    strc->arr[idx].next = 0;
    strc->arr[idx].prev = strc->tail;

    strc->arr[strc->tail].next = idx;

    strc->arr[0].prev = idx;

    strc->tail = idx;

    CALC_LIST_HASH;

    CHECK_LIST;
}


//-----------------------------------------------------------------------------


void ins_before(List *strc, size_t elem, double val){
    CHECK_LIST;
    ASSERT((elem <= 0) || (strc->arr[elem].next == -1), "Incorrect element");

    CHECK_AND_REALLOC;

    GET_IDX_OF_FREE_ELEM;

    size_t prev_of_elem = strc->arr[elem].prev;

    strc->arr[idx].data = val;
    strc->arr[idx].next = elem;
    strc->arr[idx].prev = prev_of_elem;

    strc->arr[elem].prev = idx;

    strc->arr[prev_of_elem].next = idx;

    CALC_LIST_HASH;

    CHECK_LIST;
}

void ins_after(List *strc, size_t elem, double val){
    CHECK_LIST;
    //ASSERT((elem <= 0) || (strc->arr[elem].next == -1), "Incorrect element");

    CHECK_AND_REALLOC;

    GET_IDX_OF_FREE_ELEM;

    size_t next_of_elem = strc->arr[elem].next;

    strc->arr[idx].data = val;
    strc->arr[idx].next = next_of_elem;
    strc->arr[idx].prev = elem;

    strc->arr[elem].next = idx;

    strc->arr[next_of_elem].prev = idx;

    CALC_LIST_HASH;

    CHECK_LIST;
}


//-----------------------------------------------------------------------------


void del_elem(List *strc, size_t elem){
    CHECK_LIST;
    ASSERT((elem <= 0) || (strc->arr[elem].next == -1), "Incorrect element");

    if (elem == strc->head)
        strc->head = strc->arr[elem].next;

    if (elem == strc->tail)
        strc->tail = strc->arr[elem].prev;

    size_t next_of_elem = strc->arr[elem].next;
    size_t prev_of_elem = strc->arr[elem].prev;

    strc->arr[next_of_elem].prev = prev_of_elem;
    strc->arr[prev_of_elem].next = next_of_elem;

    strc->arr[elem].data = 0;
    strc->arr[elem].next = -1;
    strc->arr[elem].prev = -1;

    stackPush(&strc->free_elem, elem);
    strc->num_of_free_elem++;

    CALC_LIST_HASH;

    CHECK_LIST;
}

void del_ALL_elem(List *strc){
    CHECK_LIST;

    for (unsigned idx = 0; idx <= strc->capacity; idx++) {
        strc->arr[idx].data = 0;
        strc->arr[idx].next = -1;
        strc->arr[idx].prev = -1;         
    }

    CALC_LIST_HASH;

    CHECK_LIST;
}


//-----------------------------------------------------------------------------


void print_list(List *strc){

    printf("   --- HEAD  --- TAIL  ---\n");
    printf("   --- %5ld --- %5ld\n", strc->head, strc->tail);

    printf("\n   --- DATA  --- NEXT  --- PREV  ---\n");

    for (unsigned num = 0; num <= strc->capacity; num++) {
        printf("%3d--- %5.1f --- %5d --- %5d ---\n", num, strc->arr[num].data, strc->arr[num].next, strc->arr[num].prev);
    }

    printf("\n--- STACK OF FREE ELEM ---\n");
    prinStack(&strc->free_elem);

    printf("%5ld --- NUM OF FREE ELEM\n", strc->num_of_free_elem);

    CALC_LIST_HASH;
}


//-----------------------------------------------------------------------------


void realloc_list(List *strc){
    CHECK_LIST;

    size_t old_capacity = strc->capacity;
    strc->capacity *= resize_coeff;

    strc->arr = (Arr*)((canary_t*)strc->arr - 1);

    Arr *tmp = (Arr *)realloc(strc->arr, (strc->capacity + 1) * sizeof(Arr) + 2 * sizeof(canary_t));
    if (tmp == NULL) 
        ASSERT(tmp == NULL, "Can't realloc memeory");
    else 
        strc->arr = tmp;
    
    *(canary_t*)strc->arr = CANARY;
    strc->arr = (Arr*)((canary_t*)strc->arr + 1);
    *(canary_t*)(strc->arr + strc->capacity + 1) = CANARY;

    for (unsigned idx = strc->capacity; idx >= old_capacity + 1; idx--) {
        stackPush(&strc->free_elem, idx);
        strc->num_of_free_elem++;

        strc->arr[idx].data = 0;
        strc->arr[idx].next = -1;
        strc->arr[idx].prev = -1;  
    }

    CALC_LIST_HASH;

    CHECK_LIST;
}


//-----------------------------------------------------------------------------


void sort_list_by_prev_slow_slow(List *strc){
    CHECK_LIST;
    qsort(strc->arr, strc->capacity, sizeof(Arr), list_cmp);
    CALC_LIST_HASH;
    CHECK_LIST;
}

int list_cmp(const void *a, const void *b){
    const Arr *ptr_a = (const Arr*)a;
    const Arr *ptr_b = (const Arr*)b;

    if (ptr_a->prev == -1) 
        return 1;
    
    if (ptr_b->prev == -1)
        return -1;
    
    if (ptr_a->prev > ptr_b->prev)
        return 1;
    else if (ptr_a->prev == ptr_b->prev)
        return 0;
    else 
        return -1;
}


//-----------------------------------------------------------------------------


size_t search_elem_slow_slow(List *strc, double val){
    CHECK_LIST;

    for (unsigned num = 1; num <= strc->capacity; num++){
        if (isequal(strc->arr[num].data, val))
            return num;
    }

    ASSERT(1, "There is no such number in the list");
    return 404; 

    CHECK_LIST;   
}

bool isequal(double a, double b){
    const double EPSILON = 0.0001; //measurement error
    
    if (fabs(a - b) <= EPSILON)
        return 1;
    else 
        return 0;
}


//-----------------------------------------------------------------------------


int listOK(List *strc){
    if (strc == NULL) return VOID_LIST;
    
    if (strc->list_left_canary != CANARY) return LIST_CANARY_LEFT_ERROR;
    if (strc->list_right_canary != CANARY) return LIST_CANARY_RIGHT_ERROR;
    
    Arr *old_arr = (Arr*)((canary_t*)strc->arr - 1);
    if (*(canary_t*)old_arr != CANARY) return ARR_CANARY_LEFT_ERROR;
    if (*(canary_t*)(strc->arr + strc->capacity + 1) != CANARY) return ARR_CANARY_RIGHT_ERROR;

    if (MurmurHash1(strc->arr, sizeof(strc->arr), SEED) != strc->arr_hash) return ARR_HASH_ERROR;
    if (strc->head != strc->head_hash) return HEAD_HASH_ERROR;
    if (strc->num_of_free_elem != strc->num_of_free_elem_hash) return NUM_OF_FREE_ELEM_HASH_ERROR;
    if (strc->tail != strc->tail_hash) return TAIL_HASH_ERROR;
    if (strc->capacity != strc->capacity_hash) return LIST_CAPACITY_HASH_ERROR;

    return NO_LIST_ERRORS;
}

void list_dump(List *strc, int error){
    switch (error){
        case VOID_LIST:{
            printf("\t\tERROR CODE: Void list ptr\n");
            print_list(strc);
            break;
        }
        
        case LIST_CANARY_LEFT_ERROR:{
            printf("\t\tERROR CODE: Incorrect list left canary\n");
            printf("EXPEXTED VALUE: %lld\n", CANARY);
            printf("RECIEVED VALUE: %lld\n", strc->list_left_canary);
            print_list(strc);
            break;
        }
        
        case LIST_CANARY_RIGHT_ERROR:{
            printf("\t\tERROR CODE: Incorrect list right canary\n");
            printf("EXPEXTED VALUE: %lld\n", CANARY);
            printf("RECIEVED VALUE: %lld\n", strc->list_right_canary);
            print_list(strc);
            break;
        }

        case ARR_CANARY_LEFT_ERROR:{
            printf("\t\tERROR CODE: Incorrect arr left canary\n");
            Arr *old_arr = (Arr*)((canary_t*)strc->arr - 1);
            printf("EXPEXTED VALUE: %lld\n", CANARY);
            printf("RECIEVED VALUE: %lld\n", *(canary_t*)old_arr);
            print_list(strc);
            break;
        }

        case ARR_CANARY_RIGHT_ERROR:{
            printf("\t\tERROR CODE: Incorrect arr right canary\n");
            printf("EXPEXTED VALUE: %lld\n", CANARY);
            printf("RECIEVED VALUE: %lld\n", *(canary_t*)(strc->arr + strc->capacity + 1));
            print_list(strc);
            break;
        }
        
        case ARR_HASH_ERROR:
            printf("\t\tERROR CODE: Incorrect arr hash\n");
            printf("EXPEXTED VALUE: %u\n", strc->arr_hash);
            printf("RECIEVED VALUE: %u\n", MurmurHash1(strc->arr, sizeof(strc->arr), SEED));
            print_list(strc);
            break;
        
        case NUM_OF_FREE_ELEM_HASH_ERROR:{
            printf("\t\tERROR CODE: Incorrect num_of_free_elem hash\n");
            printf("EXPEXTED VALUE: %lu\n", strc->num_of_free_elem_hash);
            printf("RECIEVED VALUE: %lu\n", strc->num_of_free_elem);
            print_list(strc);
            break;
        }

        case HEAD_HASH_ERROR:{
            printf("\t\tERROR CODE: Incorrect head hash\n");
            printf("EXPEXTED VALUE: %lu\n", strc->head_hash);
            printf("RECIEVED VALUE: %lu\n", strc->head);
            print_list(strc);
            break;
        }

        case TAIL_HASH_ERROR:{
            printf("\t\tERROR CODE: Incorrect tail hash\n");
            printf("EXPEXTED VALUE: %lu\n", strc->tail_hash);
            printf("RECIEVED VALUE: %lu\n", strc->tail);
            print_list(strc);
            break;
        }

        case CAPACITY_HASH_ERROR:{
            printf("\t\tERROR CODE: Incorrect capacity hash\n");
            printf("EXPEXTED VALUE: %lu\n", strc->capacity_hash);
            printf("RECIEVED VALUE: %lu\n", strc->capacity);
            print_list(strc);
            break;
        }
    }
}


//-----------------------------------------------------------------------------


void make_graph_dump(List *strc) {
    FILE* output = fopen("GRAPH.dot", "wb");
    ASSERT(output == NULL, "Can't open file");

    fputs("digraph line{\n", output);
    fputs("\trankdir = LR;\n", output);
    
    for (unsigned num = 0;  num <= strc->capacity; num++) {
        if (strc->arr[num].next != -1) {
            fprintf(output, "\tstruct%d [shape = \"record\", style = \"filled\", fillcolor = \"beige\", label = \"<d>%d| %.2lf | <n>%d | %d\"];\n",
                            num, num, strc->arr[num].data, strc->arr[num].next, strc->arr[num].prev);
        }
    }

    fputs("subgraph cluster0 {\n", output);
    for (unsigned num = 0;  num <= strc->capacity; num++) {
        if (strc->arr[num].next == -1) 
            fprintf(output, "\tstruct%d [shape = \"octagon\", style = \"filled\", fillcolor = \"darkseagreen1\", label = \"%d\"];\n", num, num);
    }

    fputs("\tlabel = \"Free elements\";\n", output);
    fputs("\tcolor = green;\n", output);
    fprintf(output, "}\n");

    fprintf(output, "\tstructa [shape = \"house\", style = \"filled\", fillcolor = \"pink\", label = \"%ld\"];\n", strc->tail);
    fprintf(output, "\tstructb [shape = \"invhouse\", style = \"filled\", fillcolor = \"plum1\", label = \"%ld\"];\n", strc->head);

    for (unsigned num = 0; num <= strc->capacity; num++) {
        if (strc->arr[num].next !=-1) {
            fprintf(output, "\tstruct%d:<n> -> struct%d:<d>;\n", num, strc->arr[num].next);
        }
    }

    fprintf(output, "}");

    fclose(output);
}