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
    STACK_ERR_EMPTY     ///< ошибка пустого стека
};

struct Stack_t
{
    int *data;
    int size;
    int capacity;
};

StackErr_t StackInit    (Stack_t *stk, unsigned capacity);
StackErr_t StackPush    (Stack_t *stk, int value);
int        StackPop     (Stack_t *stk, StackErr_t *err);  // Изменено на StackErr_t*
StackErr_t StackDestroy (Stack_t *stk);
StackErr_t StackVerify  (const Stack_t *stk);
void       StackDump    (const Stack_t *stk);

#endif //STACK_H