/*!
\file
\brief Заголовочный файл для работы со стеком

Файл содержит enum-ы - виды ошибок в стековых функциях,
              структуру стека - массив, размер и объём
              прототипы всех стековых функций
*/
#include "steck.h"

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
        // обнуление указателя, неинициализированный стек
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
        const char *poison = (i < stk->size) ? "" : " (POIZON)";
        printf("    %s[%d] = %d%s\n", marker, i, stk->data[i], poison);
    }
    
    printf("  }\n",
           "}\n\n");
}

int main()
{
    Stack_t stk1 = {0};
    
    StackErr_t err = StackInit(&stk1, 5);
    if (err != STACK_OK) {
        printf("Ошибка инициализации: %d\n", err);
        return 1;
    }
    
    StackPush(&stk1, 10);
    StackPush(&stk1, 20);
    StackPush(&stk1, 30);

    StackDump(&stk1);
    
    StackErr_t pop_err;
    int x = StackPop(&stk1, &pop_err);
   
    StackDump(&stk1);
    
    StackDestroy(&stk1);
    
    return 0;
}