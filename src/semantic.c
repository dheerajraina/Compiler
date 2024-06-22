#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

static SymbolTable symbolTable;

void addSymbol(const char *name, NodeType type)
{
        if (symbolTable.size >= SYMBOL_TABLE_SIZE)
        {
                fprintf(stderr, "Symbol table overflow: cannot add more symbols.\n");
                exit(EXIT_FAILURE);
        }

        strncpy(symbolTable.symbols[symbolTable.size].name, name, MAX_SYMBOL_NAME_LENGTH - 1);
        symbolTable.symbols[symbolTable.size].name[MAX_SYMBOL_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        symbolTable.symbols[symbolTable.size].type = type;
        symbolTable.size++;

        printf("Added symbol: %s with type: %d\n", name, type);
}

NodeType getSymbolType(const char *name)
{
        for (size_t i = 0; i < symbolTable.size; i++)
        {
                if (strcmp(symbolTable.symbols[i].name, name) == 0)
                {
                        return symbolTable.symbols[i].type;
                }
        }
        return -1; // Indicates that the symbol is not found
}

void checkDeclaration(Node *node)
{
        printf("Checking declaration: %s\n", node->value);
        if (getSymbolType(node->value) != -1)
        {
                printf("Semantic error: Redeclaration of variable '%s'\n", node->value);
                exit(EXIT_FAILURE);
        }
        addSymbol(node->value, node->children[0]->type);
}

void checkAssignment(Node *node)
{
        printf("Checking assignment to: %s\n", node->value);
        NodeType varType = getSymbolType(node->value);
        if (varType == -1)
        {
                printf("Semantic error: Undeclared variable '%s'\n", node->value);
                exit(EXIT_FAILURE);
        }
        if (node->children[0] != NULL)
        {
                NodeType valueType = node->children[0]->type;
                if (varType != valueType)
                {
                        printf("Semantic error: Type mismatch in assignment to '%s'\n", node->value);
                        exit(EXIT_FAILURE);
                }
        }
}

void checkPrintStatement(Node *node)
{
        printf("Checking print statement for variable: %s\n", node->children[0]->value);
        NodeType varType = getSymbolType(node->children[0]->value);
        if (varType == -1)
        {
                printf("Semantic error: Undeclared variable '%s'\n", node->children[0]->value);
                exit(EXIT_FAILURE);
        }
}

void analyzeNode(Node *node)
{
        if (node == NULL)
                return;

        switch (node->type)
        {
        case DECLARATION:
                checkDeclaration(node);
                break;
        case ASSIGNMENT:
                checkAssignment(node);
                break;
        case PRINT_STATEMENT:

                checkPrintStatement(node);
                break;
        default:
                break;
        }

        for (int i = 0; i < 2; i++)
        {
                if (node->children[i] != NULL)
                {
                        analyzeNode(node->children[i]);
                }
        }
}

void analyze(Node *root)
{
        symbolTable.size = 0;
        analyzeNode(root);
}
