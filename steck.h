#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

typedef enum
{
    STACK_OK = 0,       ///< ошибок нет
    STACK_ERR_NULL_PTR, ///< ошибка нулевого указателя
    STACK_ERR_DAMAGED,  ///< ошибка повреждения  стека (в т.ч. capacity = 0)
    STACK_ERR_OVERFLOW, ///< ошибка переполнения стека
    STACK_ERR_EMPTY     ///< ошибка пустого стека
} StackErr_t;

/*!
	\brief Структура стека

	Структура состоит из массива элементов стека, 
    его размера (size) и его объёма (capacity) 
*/

typedef struct
{
    int *data;
    unsigned size, capacity;
} Stack_t;

/*!
	\brief Прототипы стековых функций
*/

StackErr_t StackInit   (Stack_t *stk, unsigned capacity);
StackErr_t StackPush   (Stack_t *stk, int value);
int        StackPop    (Stack_t *stk, int *err = NULL);
StackErr_t StackDestroy(Stack_t *stk);
StackErr_t StackVerify (const Stack_t *stk);
void       StackDump   (const Stack_t *stk);