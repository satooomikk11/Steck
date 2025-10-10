#include "structs.h"
#include "disassembler.h"

// преобразование массива обратно в текст (деассемблер)
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
                    fprintf(file, "PUSHR\n");
                    i++;
                }
                else
                {
                    fprintf(file, "PUSHR ERROR\n");
                }
                break;
                
            case OP_POPR:
                if (i + 1 < commandCount)
                {
                    fprintf(file, "POPR\n");
                    i++;
                }
                else
                {
                    fprintf(file, "POPR ERROR\n");
                }
                break;

            case OP_EXIT:
                fprintf(file, "EXIT\n");
                break;
                
            case OP_PUSH:
                if (i + 1 < commandCount)
                {
                    int value = commands[i + 1];
                    fprintf(file, "PUSH %d\n", value);
                    i++;
                }
                else
                {
                    fprintf(file, "PUSH ERROR\n");
                }
                break;
                
            case OP_POP:
                fprintf(file, "POP\n");
                break;
                
            case OP_ADD:
                fprintf(file, "ADD\n");
                break;
                
            case OP_SUB:
                fprintf(file, "SUB\n");
                break;
                
            case OP_MUL:
                fprintf(file, "MUL\n");
                break;
                
            case OP_DIV:
                fprintf(file, "DIV\n");
                break;
                
            case OP_PRINT:
                fprintf(file, "PRINT\n");
                break;
                
            default:
                fprintf(file, "UNKNOWN(%d)\n", opcode);
                break;
        }

        i++;
    }
    
    fclose(file);
}