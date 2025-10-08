#include "calculator.h"
#include <string.h>

const int MAX_LINE_LENGTH = 1000;

enum OperationCode
{
    OP_EXIT  = 0,
    OP_PUSH  = 1,
    OP_POP   = 2,
    OP_ADD   = 3,
    OP_SUB   = 4,
    OP_MUL   = 5,
    OP_DIV   = 6,
    OP_PRINT = 7,
    OP_PUSHR = 8,
    OP_POPR  = 9
};

// сложение верхних элементов стека
StackErr_t StackAdd(Stack_t *stk)
{
    if (!stk)
    {
        return STACK_ERR_NULL_PTR;
    }
    StackErr_t err;

    int b = StackPop(stk, &err);
    int a = StackPop(stk, &err);

    int result = a + b;
    return StackPush(stk, result);
}

// вычитание верхних элементов стека
StackErr_t StackSub(Stack_t *stk)
{
    if (!stk)
    {
        return STACK_ERR_NULL_PTR;
    }
    StackErr_t err;

    int b = StackPop(stk, &err);
    int a = StackPop(stk, &err);

    int result = a - b;
    return StackPush(stk, result);
}

// умножение верхних элементов стека
StackErr_t StackMul(Stack_t *stk)
{
    if (!stk)
    {
        return STACK_ERR_NULL_PTR;
    }
    StackErr_t err;

    int b = StackPop(stk, &err);
    int a = StackPop(stk, &err);
    
    int result = a * b;
    return StackPush(stk, result);
}

// деление верхних элементов стека
StackErr_t StackDiv(Stack_t *stk)
{
    if (!stk)
    {
        return STACK_ERR_NULL_PTR;
    }
    StackErr_t err;

    int b = StackPop(stk, &err);
    if (b == 0)
    {
        StackPush(stk, b);
        printf("Ошибка: деление на ноль\n");
        return STACK_ERR_DAMAGED;
    }

    int a = StackPop(stk, &err);

    int result = a / b;
    return StackPush(stk, result);
}

// вывод верхнего элемента стека
StackErr_t StackPrint(Stack_t *stk)
{
    if (!stk)
    {
        return STACK_ERR_NULL_PTR;
    }
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

Register_t ParseRegisterName(const char* regName)
{
    if (strcmp(regName, "RAX") == 0) return REG_RAX;
    if (strcmp(regName, "RBX") == 0) return REG_RBX;
    if (strcmp(regName, "RCX") == 0) return REG_RCX;
    if (strcmp(regName, "RDX") == 0) return REG_RDX;
    return (Register_t)-1; // Неверный регистр
}

// чтение из файла и преобразование в массив
int* read_commands_from_file(const char* filename, int* commandCount)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Ошибка открытия файла %s\n", filename);
        return NULL;
    }
    
    int line_count = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF)
    {
        if (ch == '\n')
        {
            line_count++;
        }
    }

    // временный массив для хранения команд
    int* commands = (int*)calloc(line_count * 2, sizeof(int));
    int count = 0;
    char line[MAX_LINE_LENGTH];
    
    // заполняем ммассив данными
    while (fgets(line, sizeof(line), file))
    {
        // удаляем символ новой строки
        line[strcspn(line, "\n")] = 0;

        // преобразуем команды в числа
        if (strncmp(line, "PUSHR ", 6) == 0)
        {
            const char* regName = line + 6;
            Register_t reg = ParseRegisterName(regName);
            if (reg != (Register_t)-1)
            {
                commands[count++] = OP_PUSHR;
                commands[count++] = (int)reg;
            }
            else
            {
                printf("ERROR: неверный регистр %s\n", regName);
            }
        }

        else if (strncmp(line, "POPR ", 5) == 0)
        {
            const char* regName = line + 5;
            Register_t reg = ParseRegisterName(regName);
            if (reg != (Register_t)-1)
            {
                commands[count++] = OP_POPR;
                commands[count++] = (int)reg;
            }
            else
            {
                printf("ERROR: неверный регистр %s\n", regName);
            }
        }

        else if (strcmp(line, "EXIT") == 0) { commands[count++] = OP_EXIT; }
        else if (strncmp(line, "PUSH ", 5) == 0)
        {
            commands[count++] = OP_PUSH;

            int value = 0;
            // сдвигаем указатель на 5 вправо и записываем число в value
            if (sscanf(line + 5, "%d", &value) == 1)
            {
                commands[count++] = value;
            }
            else
            {
                printf("Ошибка: неверный формат числа в PUSH\n");
            }
        }
        else if (strcmp(line, "POP") == 0)   { commands[count++] = OP_POP;   }
        else if (strcmp(line, "ADD") == 0)   { commands[count++] = OP_ADD;   }
        else if (strcmp(line, "SUB") == 0)   { commands[count++] = OP_SUB;   }
        else if (strcmp(line, "MUL") == 0)   { commands[count++] = OP_MUL;   }
        else if (strcmp(line, "DIV") == 0)   { commands[count++] = OP_DIV;   }
        else if (strcmp(line, "PRINT") == 0) { commands[count++] = OP_PRINT; }
        else
        {
            printf("ОШИБКА: %s\n", line);
        }
    }
    
    fclose(file);
    *commandCount = count;
    return commands;
}

// выполнение команд из массива, здесь же создание стека
void execute_commands(int* commands, int commandCount)
{
    // создаём стек
    Stack_t stk = {};
    StackErr_t err = StackInit(&stk, 10); // задаётся capacity 
    if (err != STACK_OK) {
        printf("Ошибка инициализации стека: %d\n", err);
        return;
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
                    err = StackPushReg(&stk, reg);
                    if (err != STACK_OK)
                    {
                        printf("PUSHR ERROR: %d\n", err);
                    }
                    i += 2;
                }
                else
                {
                    printf("ERROR: нет регистра после PUSHR\n");
                    i++;
                }
                break;
                
            case OP_POPR:
                if (i + 1 < commandCount)
                {
                    Register_t reg = (Register_t)commands[i + 1];
                    err = StackPopReg(&stk, reg);
                    if (err != STACK_OK)
                    {
                        printf("POPR ERROR: %d\n", err);
                    }
                    i += 2;
                }
                else
                {
                    printf("ERROR: нет регистра после POPR\n");
                    i++;
                }
                break;

            case OP_EXIT:
                StackDestroy(&stk);
                return ;
                
            case OP_PUSH:
                if (i + 1 < commandCount)
                {
                    int value = commands[i + 1];
                    err = StackPush(&stk, value);
                    if (err == STACK_OK)
                    {
                        printf("Добавлено: %d\n", value);
                    }
                    else
                    {
                        printf("Ошибка добавления: %d\n", err);
                    }
                    i += 2; // двигаемся на 2 элемента
                }
                else
                {
                    printf("ERROR: нет числа после PUSH\n");
                    i++;
                }
                break;
                
            case OP_POP:
                {
                    StackErr_t pop_err;
                    int popped = StackPop(&stk, &pop_err);
                    if (pop_err == STACK_OK) {
                        printf("Извлечено: %d\n", popped);
                    } else {
                        printf("Ошибка извлечения: %d\n", pop_err);
                    }
                    i++;
                }
                break;
                
            case OP_ADD:
                err = StackAdd(&stk);
                if (err == STACK_OK)
                {
                    printf("Сложение выполнено\n");
                }
                else
                {
                    printf("Ошибка сложения: %d\n", err);
                }
                printf("\n\n");
                StackDump(&stk);
                i++;
                break;
            // todo execute_instruction(StackAdd, stk, *err, "add");     
            case OP_SUB:
                err = StackSub(&stk);
                if (err == STACK_OK)
                {
                    printf("Вычитание выполнено\n");
                }
                else
                {
                    printf("Ошибка вычитания: %d\n", err);
                }
                i++;
                break;
                
            case OP_MUL:
                err = StackMul(&stk);
                if (err == STACK_OK)
                {
                    printf("Умножение выполнено\n");
                }
                else
                {
                    printf("Ошибка умножения: %d\n", err);
                }
                i++;
                break;
                
            case OP_DIV:
                err = StackDiv(&stk);
                if (err == STACK_OK)
                {
                    printf("Деление выполнено\n");
                }
                else
                {
                    printf("Ошибка деления: %d\n", err);
                }
                i++;
                break;
                
            case OP_PRINT:
                err = StackPrint(&stk);
                if (err != STACK_OK)
                {
                    printf("Ошибка вывода: %d\n", err);
                }
                i++;
                break;
                   
            default:
                printf("Неизвестный код операции: %d\n", opcode);
                i++;
                break;
        }
    }
    
    StackDestroy(&stk);

    return ;
}

// преобразование числового массива обратно в текст и запись в файл
void write_commands_to_file(const char* filename, int* commands, int commandCount)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        printf("Ошибка создания файла %s\n", filename);
        return;
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
                    i += 2;
                }
                else
                {
                    printf("ERROR: нет регистра после PUSHR\n");
                    i++;
                }
                break;
                
            case OP_POPR:
                if (i + 1 < commandCount)
                {
                    i += 2;
                }
                else
                {
                    printf("ERROR: нет регистра после POPR\n");
                    i++;
                }
                break;

            case OP_EXIT:
                fprintf(file, "EXIT\n");
                i++;
                break;
                
            case OP_PUSH:
                if (i + 1 < commandCount)
                {
                    int value = commands[i + 1];
                    fprintf(file, "PUSH %d\n", value);
                    i += 2;
                }
                else
                {
                    fprintf(file, "PUSH ERROR\n");
                    i++;
                }
                break;
                
            case OP_POP:
                fprintf(file, "POP\n");
                i++;
                break;
                
            case OP_ADD:
                fprintf(file, "ADD\n");
                i++;
                break;
                
            case OP_SUB:
                fprintf(file, "SUB\n");
                i++;
                break;
                
            case OP_MUL:
                fprintf(file, "MUL\n");
                i++;
                break;
                
            case OP_DIV:
                fprintf(file, "DIV\n");
                i++;
                break;
                
            case OP_PRINT:
                fprintf(file, "PRINT\n");
                i++;
                break;
                
            default:
                fprintf(file, "ERROR(%d)\n", opcode);
                i++;
                break;
        }
    }
    
    fclose(file);

    return ;
}


int main()
{
    const char* inputFile = "commands.txt";
    const char* outputFile = "output_commands.txt";
    
    // чтение команд из файла и преобразование в числовой массив
    int  commandCount = 0;
    int* commands = read_commands_from_file(inputFile, &commandCount);
    
    if (!commands)
    {
        printf("Не удалось прочитать команды из файла\n");
        return 1;
    }
    
    printf("Прочитано %d элементов из файла\n", commandCount);
    
    // вывод числового массива для отладки
    printf("Массив команд: ");
    for (int i = 0; i < commandCount; i++)
    {
        printf("%d ", commands[i]);
    }
    printf("\n\n");
    
    // выполнение команд
    execute_commands(commands, commandCount);

    write_commands_to_file(outputFile, commands, commandCount);
    
    // очистка памяти
    free(commands);

    return 0;
}