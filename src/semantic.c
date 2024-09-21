#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

static SymbolTable symbolTable;

void addSymbol(const char *name, NodeType type, DataType dType)
{
        if (symbolTable.size >= SYMBOL_TABLE_SIZE)
        {
                fprintf(stderr, "Symbol table overflow: cannot add more symbols.\n");
                exit(EXIT_FAILURE);
        }

        strncpy(symbolTable.symbols[symbolTable.size].name, name, MAX_SYMBOL_NAME_LENGTH - 1);
        symbolTable.symbols[symbolTable.size].name[MAX_SYMBOL_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        symbolTable.symbols[symbolTable.size].type = type;
        symbolTable.symbols[symbolTable.size].dType = dType;
        symbolTable.size++;

        printf("Added symbol: %s with type: %d\n", name, type);
}

NodeType getSymbolType(const char *name)
{
        for (size_t i = 0; i < symbolTable.size; i++)
        {
                if (strcmp(symbolTable.symbols[i].name, name) == 0)
                {
                        return symbolTable.symbols[i].dType;
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

        addSymbol(node->value, node->type, node->dType);
        for (int i = 0; i < node->numChildren; i++)
        {
                analyzeNode(node->children[i]);
        }
}

void checkDeclarationAssignment(Node *node)
{
        printf("Checking declaration and assignment: %s\n", node->value);
        if (getSymbolType(node->value) != -1)
        {
                printf("Semantic error: Redeclaration of variable '%s'\n", node->value);
                exit(EXIT_FAILURE);
        }
        addSymbol(node->value, node->children[0]->type, node->children[0]->dType);
        for (int i = 0; i < node->numChildren; i++)
        {
                analyzeNode(node->children[i]);
        }

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
        DataType varType = getSymbolType(node->value);
        if (varType == -1)
        {
                printf("Semantic error: Undeclared variable '%s'\n", node->value);
                exit(EXIT_FAILURE);
        }

        for (int i = 0; i < node->numChildren; i++)
        {
                analyzeNode(node->children[i]);
        }
        if (node->children[0] != NULL)
        {
                if (node->children[0]->type == BIN_OP_NODE)
                {
                        checkBinaryOperation(node->children[0]);
                }
                else
                {
                        DataType valueType = node->children[0]->dType;
                        printf("-----types %d %d ", varType, valueType);
                        if (varType != valueType)
                        {
                                printf("Semantic error: Type mismatch in assignment to '%s'\n", node->value);
                                exit(EXIT_FAILURE);
                        }
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
        printf(" print node %s  %d\n", node->children[0]->value, node->children[0]->type);
        if (varType == -1)
        {
                printf("Semantic error: Undeclared variable '%s'\n", node->children[0]->value);
                exit(EXIT_FAILURE);
        }
}

void checkForLoop(Node *node)
{
        if (node->numChildren != 1) // FOR LOOP NODE TO HAVE 2 CHILDREN -> DECLARED VARIABLE WITH ITS RANGE AS CHILDREN AND OTHER CONTAINING LOOP BODY
        {
                fprintf(stderr, "Error: Invalid for loop structure");
                exit(EXIT_FAILURE);
        }
        Node *initNode = node->children[0]; // variable declaration
        if (initNode->numChildren != 2)
        {
                fprintf(stderr, "Error: Invalid for loop structure");
                exit(EXIT_FAILURE);
        }
        Node *fromExpr = initNode->children[0];
        Node *toExpr = initNode->children[1];

        if (initNode->dType != fromExpr->dType || initNode->dType != toExpr->dType)
        {
                fprintf(stderr, "Semantic error: Type mismatch in assignment to '%s in for loop'\n", initNode->value);
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
        case FOR_LOOP_NODE:
                checkForLoop(node);
                break;
        default:
                break;
        }

        for (int i = 0; i < node->numChildren; i++)
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
