#include "structs.h"
#include "processor.h"

// сложение верхних элементов стека
StackErr_t StackAdd(Stack_t *stk)
{
    if (!stk) return STACK_ERR_NULL_PTR;
    
    StackErr_t err;
    int b = StackPop(stk, &err);
    if (err != STACK_OK) return err;
    
    int a = StackPop(stk, &err);
    if (err != STACK_OK) {
        StackPush(stk, b);
        return err;
    }
    
    int result = a + b;
    return StackPush(stk, result);
}

// вычитание верхних элементов стека
StackErr_t StackSub(Stack_t *stk)
{
    if (!stk) return STACK_ERR_NULL_PTR;
    
    StackErr_t err;
    int b = StackPop(stk, &err);
    if (err != STACK_OK) return err;
    
    int a = StackPop(stk, &err);
    if (err != STACK_OK) {
        StackPush(stk, b);
        return err;
    }
    
    int result = a - b;
    return StackPush(stk, result);
}

// умножение верхних элементов стека
StackErr_t StackMul(Stack_t *stk)
{
    if (!stk) return STACK_ERR_NULL_PTR;
    
    StackErr_t err;
    int b = StackPop(stk, &err);
    if (err != STACK_OK) return err;
    
    int a = StackPop(stk, &err);
    if (err != STACK_OK) {
        StackPush(stk, b);
        return err;
    }
    
    int result = a * b;
    return StackPush(stk, result);
}

// деление верхних элементов стека
StackErr_t StackDiv(Stack_t *stk)
{
    if (!stk) return STACK_ERR_NULL_PTR;
    
    StackErr_t err;
    int b = StackPop(stk, &err);
    if (err != STACK_OK) return err;
    
    if (b == 0)
    {
        StackPush(stk, b);
        printf("Ошибка: деление на ноль\n");
        return STACK_ERR_DAMAGED;
    }

    int a = StackPop(stk, &err);
    if (err != STACK_OK) {
        StackPush(stk, b);
        return err;
    }

    int result = a / b;
    return StackPush(stk, result);
}

// вывод верхнего элемента стека
StackErr_t StackPrint(Stack_t *stk)
{
    if (!stk) return STACK_ERR_NULL_PTR;
    
    StackErr_t err = StackVerify(stk);
    if (err != STACK_OK) return err;

    if (stk->size == 0)
    {
        printf("Стек пуст\n");
        return STACK_OK;
    }

    printf("Верхний элемент: %d\n", stk->data[stk->size - 1]);
    return STACK_OK;
}

// функция выполнения (вместо копипаста)
StackErr_t execute_instruction(StackErr_t (*operation)(Stack_t*), Stack_t *stk, StackErr_t *err, const char* op_name)
{
    if (err != NULL) { *err = STACK_OK; }
    
    StackErr_t result = operation(stk);
    
    if (err != NULL) { *err = result; }
    
    if (result == STACK_OK) { printf("Команда %s выполнена успешно\n", op_name); }
    else { printf("Ошибка выполнения %s: %d\n", op_name, result); }
    
    return result;
}

// инициализация структуры для процессора
StackErr_t processor_init(Processor* proc, unsigned stack_capacity)
{
    if (!proc) { return STACK_ERR_NULL_PTR; }
    
    // инициализация стека
    StackErr_t err = StackInit(&proc->stack, stack_capacity);
    if (err != STACK_OK) { return err; }
    
    // инициализация регистров
    for (int i = 0; i < 4; i++)
    {
        proc->registers[i] = 0;
    }
    
    // инициализация указателя на команды
    proc->ip = 0;
    
    return STACK_OK;
}

StackErr_t StackJump(int* commands, int commandCount, int* current_index, int jump_offset)
{
    if (!commands || !current_index)
    {
        return STACK_ERR_NULL_PTR;
    }
    
    // считаем новый индекс
    int new_index = *current_index + jump_offset;
    if (new_index < 0 || new_index >= commandCount)
    {
        printf("Ошибка JUMP: выход за границы массива команд\n");
        return STACK_ERR_DAMAGED;
    }
    
    *current_index = new_index;
    
    return STACK_OK;
}

// выполнение команд из массива (процессор)
StackErr_t execute_commands(int* commands, int commandCount)
{
    // создаём процессор
    Processor proc;
    StackErr_t err = processor_init(&proc, 100);
    if (err != STACK_OK)
    {
        printf("Ошибка инициализации процессора: %d\n", err);
        return err;
    }
    
    int i = 0;
    while (i < commandCount)
    {
        int opcode = commands[i];
        switch (opcode)
        {
            case OP_PUSHR:
                if (i + 1 < commandCount)
                {
                    Register_t reg = (Register_t)commands[i + 1];
                    err = StackPushReg(&proc.stack, reg);
                    if (err != STACK_OK)
                    {
                        printf("PUSHR ERROR: %d\n", err);
                    }
                    i++;
                }
                else
                {
                    printf("ERROR: нет регистра после PUSHR\n");
                }
                break;
                
            case OP_POPR:
                if (i + 1 < commandCount)
                {
                    Register_t reg = (Register_t)commands[i + 1];
                    err = StackPopReg(&proc.stack, reg);
                    if (err != STACK_OK)
                    {
                        printf("POPR ERROR: %d\n", err);
                    }
                    i++;
                }
                else
                {
                    printf("ERROR: нет регистра после POPR\n");
                }
                break;

            case OP_JUMP:
                if (i + 1 < commandCount)
                {
                    int jump_offset = commands[i+1];
                    err = StackJump(commands, commandCount, &i, jump_offset);
                    if (err != STACK_OK)
                    {
                        printf("JUMP ERROR: %d\n", err);
                    }
                    i++;
                }
                else
                {
                    printf("ERROR: нет сдвига после JUMP\n");
                }
                break;

            case OP_EXIT:
                StackDestroy(&proc.stack);
                printf("Выход из программы\n");
                return STACK_OK;
                
            case OP_PUSH:
                if (i + 1 < commandCount)
                {
                    int value = commands[i + 1];
                    err = StackPush(&proc.stack, value);
                    if (err == STACK_OK)
                    {
                        printf("Добавлено: %d\n", value);
                    }
                    else
                    {
                        printf("Ошибка добавления: %d\n", err);
                    }
                    i++;
                }
                else
                {
                    printf("ERROR: нет числа после PUSH\n");
                }
                break;
                
            case OP_POP:
                {
                    StackErr_t pop_err;
                    int popped = StackPop(&proc.stack, &pop_err);
                    if (pop_err == STACK_OK) { printf("Извлечено: %d\n", popped); }
                    else { printf("Ошибка извлечения: %d\n", pop_err); }
                }
                break;
                
            case OP_ADD:
                execute_instruction(StackAdd, &proc.stack, &err, "ADD");
                break;
            
            case OP_SUB:
                execute_instruction(StackSub, &proc.stack, &err, "SUB");
                break;
                
            case OP_MUL:
                execute_instruction(StackMul, &proc.stack, &err, "MUL");
                break;
                
            case OP_DIV:
                execute_instruction(StackDiv, &proc.stack, &err, "DIV");
                break;
                
            case OP_PRINT:
                execute_instruction(StackPrint, &proc.stack, &err, "PRINT");
                break;
                   
            default:
                printf("Неизвестный код операции: %d\n", opcode);
                break;
        }

        i++;
    }
    
    StackDestroy(&proc.stack);
    return STACK_OK;
}