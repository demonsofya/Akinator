#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdlib.h>
#include <sys/types.h>
//#include <unistd.h>


//=============================================================================
//=============================================================================


#define Return_If_Error(stk)                                        \
    {                                                               \
        int error = StackVerify(stk);                               \
        if (error != 0) {                                           \
            StackDump(stk, __FILE__, __FUNCTION__, __LINE__);       \
            return error;                                           \
        }                                                           \
    }

#define Return_Error(stk)                                           \
    {                                                               \
        int error = StackVerify(stk);                               \
        if (error != 0)                                             \
            StackDump(stk, __FILE__, __FUNCTION__, __LINE__);       \
        return error;                                               \
    }

//-----------------------------------------------------------------------------

const int CANARY = 0xDEADBABE;

const int POISON = 1984;

struct Stack_t {
    int     *data;
    size_t stack_size;
    size_t capacity;
    unsigned long int hash;
};

enum StackErr_t {
    No_Error        = 0,  // double pow(double, double); pow(2, 0) pow(2, 1) ... pow(2, n)
    Pointer_Error   = 1 << 0,
    Buffer_Error    = 1 << 1,
    Size_Error      = 1 << 2,
    Capacity_Error  = 1 << 3,
    Canary_Error    = 1 << 4,
    Hash_Error      = 1 << 5
};

int                 StackVerify     (Stack_t *stk);
void                StackDump       (Stack_t *stk, const char *function_name,
                                     const char *file_name, int line_number);

int                 StackPop        (Stack_t *stk, int *value);
int                 StackPush       (Stack_t *stk, int value);

int                 StackCtor       (Stack_t *stk, int capacity);
int                 StackDtor       (Stack_t *stk);

bool                CanaryCheck     (Stack_t *stk);

unsigned long int   CountHash       (Stack_t *stk);
unsigned long int   CountByteHash   (Stack_t *stk);

void ExitFunction();

//=============================================================================

#endif // STACK_H_INCLUDED
