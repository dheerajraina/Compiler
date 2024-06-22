#ifndef SEMANTIC_H
#define SEMANTIC_H

#include "parser.h"

#define SYMBOL_TABLE_SIZE 128
#define MAX_SYMBOL_NAME_LENGTH 32

typedef struct
{
        char name[MAX_SYMBOL_NAME_LENGTH];
        NodeType type;
} Symbol;

typedef struct
{
        Symbol symbols[SYMBOL_TABLE_SIZE];
        size_t size;
} SymbolTable;

void analyze(Node *root);

#endif
