#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

#ifndef CHECK_STACK
#define CHECK_STACK do{                                                                                                                                             \
                int error = 0;                                                                                                                                      \
                if ((error = stackOK(st)) != 0)                                                                                                                     \
                {                                                                                                                                                   \
                    stackDump (error);                                                                                                                              \
                    printf (" %s:%d, IN FUNCTION %s:\n.", __FILE__, __LINE__, __PRETTY_FUNCTION__);                                                                 \
                    printf ("Stack:\n");                                                                                                                            \
                    /*printf ("%ld\n", st->Size); */                                                                                                                \
                    prinStack (st);                                                                                                                                 \
                    printf ("                 Left stack canary  Right stack canary  Left data canary  Right data canary  Data hash  Capacity hash  Stack hash\n"   \
                            "Expexted values: %lld               %lld                %lld              %lld               %u         %ld            %ld \n"         \
                            "Received values: %lld               %lld                %lld              %lld               %u         %ld            %ld \n",        \
                            CANARY, CANARY, CANARY, CANARY, st->data_hash, st->capacity_hash, st->size_hash,                                                        \
                            st->leftCanary, st->rightCanary, *(canary_t*)(st->data - 1), *(canary_t*)(st->data + st->capacity),                                     \
                            MurmurHash1 (st->data, sizeof(st->data), SEED), st->capacity, st->Size);                                                                \
                    /*abort ();*/                                                                                                                                   \
                }                                                                                                                                                   \
            } while (0)
#endif

#ifndef ERROR_INFO
#define ERROR_INFO(statement, text) do {                                                                                    \
                                        if (statement) {                                                                    \
                                            printf ("%s:%d, IN FUNCTION %s\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);    \
                                            printf (#text);                                                                 \
                                        }                                                                                   \
                                    } while (0)
#endif

#ifndef PUT_CANARY
#define PUT_CANARY *(canary_t*)(st->data - 1) = CANARY; *(canary_t*)(st->data + st->capacity) = CANARY;
#endif

#ifndef CALC_HASH
#define CALC_HASH   do {                                                                  \
                        st->data_hash = MurmurHash1 (st->data, sizeof(st->data), SEED);   \
                        st->capacity_hash = st->capacity;                                 \
                        st->size_hash = st->Size;                                         \
                    } while (0)
#endif


enum ERRORS
{
    NO_ERRORS                = 0,
    STACK_UNDERFLOW          = 10101,
    STACK_OVERFLOW           = 10201,
    ALLOC_ERROR              = 10301,
    REALLOC_ERROR            = 10401,
    VOID_STACK               = 10501,
    DATA_CANARY_LEFT_ERROR   = 10601,
    DATA_CANARY_RIGHT_ERROR  = 10701,
    STACK_CANARY_LEFT_ERROR  = 10801,
    STACK_CANARY_RIGHT_ERROR = 10901,
    DATA_HASH_ERROR          = 101001,
    CAPACITY_HASH_ERROR      = 101101,
    SIZE_HASH_ERROR          = 101201,
    POISON_ERROR             = 101301,
    MEMSET_ERROR             = 101401
};


typedef long long canary_t;

const canary_t CANARY        = 0xBADDCAFE;
const int START_STACK_SIZE   = 4;
const int RESIZE_COEFFICIENT = 2;

const double POISON          = 0xDEADBEEF;

const int DEBUG_LEVEL = 1;


struct Stack
{

    canary_t leftCanary;

    double *data;
    size_t capacity;
    size_t Size;

    unsigned data_hash;
    size_t capacity_hash;
    size_t size_hash ;

    canary_t rightCanary;
};


ERRORS stackCtor (Stack* st);
ERRORS stackPush (Stack* st, double value);
ERRORS stackPop (Stack* st); 
ERRORS stackDtor (Stack* st);
ERRORS reallocate (Stack* st, size_t newSize);

void stackDump (int error);
void prinStack (const Stack* st);
ERRORS stackOK (const Stack* st);

#endif