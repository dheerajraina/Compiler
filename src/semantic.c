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

void checkDeclarationAssignment(Node *node)
{
        printf("Checking declaration and assignment: %s\n", node->value);
        if (getSymbolType(node->value) != -1)
        {
                printf("Semantic error: Redeclaration of variable '%s'\n", node->value);
                exit(EXIT_FAILURE);
        }
        addSymbol(node->value, node->children[0]->type);

        if (node->children[0]->type == BIN_OP_NODE)
                checkBinaryOperation(node->children[0]);
        else if (node->children[0]->type == LITERAL_NODE)
                checkAssignment(node);
        else
                exit(EXIT_FAILURE);
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

void checkBinaryOperation(Node *node)
{
        if (strcmp(node->value, "+") == 0)
        {

                analyzeNode(node->children[0]);
                analyzeNode(node->children[1]);
                TokenType leftType = getSymbolType(node->children[0]->value);
                TokenType rightType = getSymbolType(node->children[1]->value);

                if (node->children[0]->type == LITERAL_NODE)
                {
                        leftType = LITERAL_INT_TOKEN; // Assuming literals are integers for simplicity
                }

                if (node->children[1]->type == LITERAL_NODE)
                {
                        rightType = LITERAL_INT_TOKEN; // Assuming literals are integers for simplicity
                }

                if (leftType != rightType)
                {
                        fprintf(stderr, "Error: Type mismatch in binary operation. Left type: %d, Right type: %d\n", leftType, rightType);
                        exit(EXIT_FAILURE);
                }
        }
}
void checkPrintStatement(Node *node)
{
        printf("Checking print statement for variable: %s\n", node->children[0]->value);
        NodeType varType = getSymbolType(node->children[0]->value);
        printf(" print node %s %d", node->children[0]->value, node->children[0]->type);
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
        case DECLARATION_NODE:
                checkDeclaration(node);
                break;
        case ASSIGNMENT_NODE:
                checkAssignment(node);
                break;
        case PRINT_STATEMENT_NODE:

                checkPrintStatement(node);
                break;
        case BIN_OP_NODE:
                checkBinaryOperation(node);
                break;
        case DECLARATION_ASSIGNMENT_NODE:
                checkDeclarationAssignment(node);
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
