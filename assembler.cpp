#include "structs.h"
#include "assembler.h"

// чтение из файла и преобразование в массив (ассемблер)
int* read_commands_from_file(const char* filename, int* commandCount)
{
    assert(commandCount);
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        printf("Ошибка открытия файла %s\n", filename);
        return NULL;
    }
    
    // временный массив для хранения команд
    int* tempCommands = (int*)calloc(1000, sizeof(int));
    int  count = 0;
    char line[MAX_LINE_LENGTH];
    
    // заполняем массив данными
    while (fgets(line, sizeof(line), file))
    {
        // удаляем символ новой строки
        line[strcspn(line, "\n")] = 0;

        // преобразуем команды в числа
        if (strncmp(line, "PUSHR ", 6) == 0)
        {
            const char* reg_name = line + 6;
            Register_t reg = ParseRegisterName(reg_name);
            if (reg != (Register_t)-1)
            {
                tempCommands[count++] = OP_PUSHR;
                tempCommands[count++] = (int)reg;
            }
            else
            {
                printf("ERROR: неверный регистр %s\n", reg_name);
            }
        }
        else if (strncmp(line, "POPR ", 5) == 0)
        {
            const char* reg_name = line + 5;
            Register_t reg = ParseRegisterName(reg_name);
            if (reg != (Register_t)-1)
            {
                tempCommands[count++] = OP_POPR;
                tempCommands[count++] = (int)reg;
            }
            else
            {
                printf("ERROR: неверный регистр %s\n", reg_name);
            }
        }
        else if (strncmp(line, "JUMP ", 5) == 0)
        {
            tempCommands[count++] = OP_JUMP;
            int offset = 0;
            if (sscanf(line + 5, "%d", &offset) == 1)
            {
                tempCommands[count++] = offset;
            }
            else
            {
                printf("Ошибка: неверный формат смещения в JUMP\n");
            }
        }
        else if (strcmp(line, "EXIT") == 0) { tempCommands[count++] = OP_EXIT; }
        else if (strncmp(line, "PUSH ", 5) == 0)
        {
            tempCommands[count++] = OP_PUSH;
            int value = 0;
            if (sscanf(line + 5, "%d", &value) == 1)
            {
                tempCommands[count++] = value;
            }
            else
            {
                printf("Ошибка: неверный формат числа в PUSH\n");
            }
        }
        else if (strcmp(line, "POP") == 0)   { tempCommands[count++] = OP_POP;   }
        else if (strcmp(line, "ADD") == 0)   { tempCommands[count++] = OP_ADD;   }
        else if (strcmp(line, "SUB") == 0)   { tempCommands[count++] = OP_SUB;   }
        else if (strcmp(line, "MUL") == 0)   { tempCommands[count++] = OP_MUL;   }
        else if (strcmp(line, "DIV") == 0)   { tempCommands[count++] = OP_DIV;   }
        else if (strcmp(line, "PRINT") == 0) { tempCommands[count++] = OP_PRINT; }
        else
        {
            printf("ОШИБКА: %s\n", line);
        }
    }
    
    fclose(file);
    
    // создаем финальный массив нужного размера
    int* commands = (int*)calloc(count, sizeof(int));
    memcpy(commands, tempCommands, count * sizeof(int));
    free(tempCommands);
    
    *commandCount = count;
    return commands;
}