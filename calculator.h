#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "stack.h"

StackErr_t StackAdd     (Stack_t *stk);
StackErr_t StackSub     (Stack_t *stk);
StackErr_t StackMul     (Stack_t *stk);
StackErr_t StackDiv     (Stack_t *stk);
StackErr_t StackPrint   (Stack_t *stk);
StackErr_t StackPrintAll(Stack_t *stk);

int* readCommandsFromFile(const char* filename, int* commandCount);
void executeCommands     (int* commands, int commandCount);
void writeCommandsToFile (const char* filename, int* commands, int commandCount);

#endif // CALCULATOR_H