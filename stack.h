#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

enum StackErr_t
{
    STACK_OK = 0,       ///< ошибок нет
    STACK_ERR_NULL_PTR, ///< ошибка нулевого указателя
    STACK_ERR_DAMAGED,  ///< ошибка повреждения  стека (в т.ч. capacity = 0)
    STACK_ERR_OVERFLOW, ///< ошибка переполнения стека
    STACK_ERR_EMPTY,    ///< ошибка пустого стека
    STACK_ERR_INVALID_REGISTER
};

struct Stack_t
{
    int *data;
    int size;
    int capacity;
};

enum Register_t
{
    REG_RAX = 0,
    REG_RBX = 1,
    REG_RCX = 2,
    REG_RDX = 3 
};

StackErr_t StackInit    (Stack_t *stk, unsigned capacity);
StackErr_t StackPush    (Stack_t *stk, int value);
int        StackPop     (Stack_t *stk, StackErr_t *err);
StackErr_t StackDestroy (Stack_t *stk);
StackErr_t StackVerify  (const Stack_t *stk);
void       StackDump    (const Stack_t *stk);

StackErr_t StackPushReg (Stack_t *stk, Register_t reg);
StackErr_t StackPopReg  (Stack_t *stk, Register_t reg);

#endif //STACK_H