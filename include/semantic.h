#ifndef SEMANTIC_H
#define SEMANTIC_H

#include <stdbool.h>
#include "parser.h"

#define SYMBOL_TABLE_SIZE 128
#define MAX_SYMBOL_NAME_LENGTH 32

typedef struct
{
        char name[MAX_SYMBOL_NAME_LENGTH];
        NodeType type;
        DataType dType;
        int scopeLevel;    // 0 == global ; 1 == local
        int isInitialised; // 1 ==yes ; 0== no
} Symbol;

typedef struct
{
        Symbol symbols[SYMBOL_TABLE_SIZE];
        size_t size;
        int capacity;
} SymbolTable;

typedef struct ScopedSymbolTable
{
        SymbolTable *tables;
        int currentScope; // tracks the top of the stack(containing various parse tables)
        int maxScopes;
} ScopedSymbolTable;

void enterNewScope();
void exitScope();
void addSymbol(const char *name, NodeType type, DataType dType);
bool isSymbolDeclared(char *name);
void analyze(Node *root);

#endif
