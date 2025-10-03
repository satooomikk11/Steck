#include "stack.h"

StackErr_t StackInit(Stack_t *stk, unsigned capacity)
{
    assert(stk);

    if (capacity == 0)
    {
        return STACK_ERR_DAMAGED;
    }

    // выделение памяти под массив стека с интовыми элементами
    stk->data = (int *)calloc(capacity, sizeof(int));
    if (!stk->data)
    {
        // неинициализированный стек
        return STACK_ERR_DAMAGED;
    }
     
    stk->size = 0;
    stk->capacity = capacity;

    return STACK_OK;
}

StackErr_t StackPush(Stack_t *stk, int value)
{
    // проверка стека (по лекции деда)
    StackErr_t err = StackVerify(stk);
    if (err) return err;

    if (stk->size >= stk->capacity)
    {
        printf("Ошибка: переполнение стека\n");
        return STACK_ERR_OVERFLOW;
    }

    stk->data[stk->size++] = value;

    err = StackVerify(stk);
    if (err) return err;

    return STACK_OK;
}

int StackPop(Stack_t *stk, StackErr_t *err)
{
    StackErr_t verify_err = StackVerify(stk);

    // обработка ошибки (если она есть)
    if (verify_err)
    {
        if (err != NULL)
        {
            // если стек повреждён, err будет со значением ошибки
            *err = verify_err;
        }
        return 0;
    }
    
    // если стек пустой и извлекать нечего
    if (stk->size == 0)
    {
        if (err != NULL)
        {
            *err = STACK_ERR_EMPTY;
        }
        return 0;
    }
    
    int value = stk->data[--stk->size];
    
    verify_err = StackVerify(stk);
    if (verify_err && err != NULL)
    {
        *err = verify_err;
    }
    
    return value;
}

StackErr_t StackDestroy(Stack_t *stk)
{
    if (!stk)
    {
        return STACK_ERR_NULL_PTR;
    }

    StackErr_t err = StackVerify(stk);
    if (err) return err;
    
    // освобождение памяти стека
    if (stk->data)
    {
        free(stk->data);
        stk->data = NULL;
    }
    
    stk->size     = 0;
    stk->capacity = 0;
    
    return STACK_OK;
}

StackErr_t StackVerify(const Stack_t *stk)
{
    if (!stk) {
        return STACK_ERR_NULL_PTR;
    }
    
    if (!stk->data || stk->size < 0 || stk->capacity <= 0 || stk->size > stk->capacity) {
        return STACK_ERR_DAMAGED;
    }
    
    return STACK_OK;
}

void StackDump(const Stack_t *stk)
{
    printf("StackDump called\n",
           "Stack[%p]\n", (void*)stk,
           "{\n");
    
    printf("  size     = %d\n", stk->size);
    printf("  capacity = %d\n", stk->capacity);
    printf("  data[%p]\n", (void*)stk->data);
    printf("  {\n");
    
    for (unsigned i = 0; i < stk->capacity; i++)
    {
        // заполнена ячейка (*) или нет ( )
        const char *marker = (i < stk->size) ? "*" : " ";
        printf("    %s[%d] = %d\n", marker, i, stk->data[i]);
    }
    
    printf("  }\n",
           "}\n\n");
}
