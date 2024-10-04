#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantic.h"

static ScopedSymbolTable scopedTable;

SymbolTable createSymbolTable()
{
        SymbolTable symbolTable;
        symbolTable.size = 0;
        return symbolTable;
}

void printSymbolTable()
{
        for (int i = 0; i <= scopedTable.currentScope; i++)
        {
                printf("Scope %d:\n Size = %d", i, scopedTable.tables[i].size);
                for (int j = 0; j < scopedTable.tables[i].size; j++)
                {
                        printf("  Name: %s, dType: %d\n",
                               scopedTable.tables[i].symbols[j].name,
                               scopedTable.tables[i].symbols[j].dType);
                }
        }
}

void enterNewScope()
{
        if (scopedTable.currentScope < scopedTable.maxScopes - 1)
        {
                scopedTable.currentScope++;
                scopedTable.tables[scopedTable.currentScope] = createSymbolTable();
        }
}

void exitScope()
{
        if (scopedTable.currentScope <= 0)
        {
                fprintf(stderr, "Symbol Table Error: No scope exists\n");
                exit(EXIT_FAILURE);
        }
        scopedTable.currentScope--;
}

void addSymbol(const char *name, NodeType type, DataType dType)
{

        if (scopedTable.tables[scopedTable.currentScope].size >= SYMBOL_TABLE_SIZE)
        {
                fprintf(stderr, "Symbol table overflow: cannot add more symbols.\n");
                exit(EXIT_FAILURE);
        }

        strncpy(scopedTable.tables[scopedTable.currentScope].symbols[scopedTable.tables[scopedTable.currentScope].size].name, name, MAX_SYMBOL_NAME_LENGTH - 1);
        scopedTable.tables[scopedTable.currentScope].symbols[scopedTable.tables[scopedTable.currentScope].size].name[MAX_SYMBOL_NAME_LENGTH - 1] = '\0'; // Ensure null-termination
        scopedTable.tables[scopedTable.currentScope].symbols[scopedTable.tables[scopedTable.currentScope].size].type = type;
        scopedTable.tables[scopedTable.currentScope].symbols[scopedTable.tables[scopedTable.currentScope].size].dType = dType;
        scopedTable.tables[scopedTable.currentScope].size++;

        printf("Added symbol: %s with type: %d\n", name, type);
}

NodeType getSymbolType(const char *name)
{
        printf("get symbol type %d , name :%s\n", scopedTable.currentScope, name);
        for (int i = scopedTable.currentScope; i >= 0; i--)
        {
                SymbolTable currentSymbolTable = scopedTable.tables[i];
                for (int j = 0; j < currentSymbolTable.size; j++)
                {
                        if (strcmp(currentSymbolTable.symbols[j].name, name) == 0)
                        { // Fix indexing to j
                                return currentSymbolTable.symbols[j].dType;
                        }
                }
        }
        return -1; // Indicates that the symbol is not found
}

void checkDeclaration(Node *node)
{
        printf("Checking declaration: %s , current scope: %d\n", node->value, scopedTable.currentScope);
        if (getSymbolType(node->value) != -1)
        {
                printf("Semantic error: Redeclaration of variable '%s'\n", node->value);
                exit(EXIT_FAILURE);
        }
        addSymbol(node->value, node->type, node->dType);
        printSymbolTable();
}

void checkDeclarationAssignment(Node *node)
{
        printf("Checking declaration and assignment: %s,, current scope: %d\n", node->value, scopedTable.currentScope);
        if (getSymbolType(node->value) != -1)
        {
                printf("Semantic error: Redeclaration of variable '%s'\n", node->value);
                exit(EXIT_FAILURE);
        }
        addSymbol(node->value, node->children[0]->type, node->children[0]->dType);
        if (node->children[0]->type == BIN_OP_NODE)
        {
                checkBinaryOperation(node->children[0]);
        }
        else
        {
                DataType valueType = node->children[0]->dType;
                if (getSymbolType(node->value) != valueType)
                {
                        printf("Semantic error: Type mismatch in assignment to '%s'\n", node->value);
                        exit(EXIT_FAILURE);
                }
        }
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
        if (node->children[0] != NULL)
        {
                if (node->children[0]->type == BIN_OP_NODE)
                {
                        checkBinaryOperation(node->children[0]);
                }
                else
                {
                        DataType valueType = node->children[0]->dType;
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
        if (strcmp(node->value, "+") == 0 || strcmp(node->value, ">") == 0 || strcmp(node->value, "<") == 0 || strcmp(node->value, "==") == 0)
        {
                analyzeNode(node->children[0]);
                analyzeNode(node->children[1]);
                DataType leftType = getSymbolType(node->children[0]->value);
                DataType rightType = getSymbolType(node->children[1]->value);

                if (node->children[0]->type == LITERAL_NODE || node->children[0]->type == IDENTIFIER_NODE)
                {
                        if (node->children[0]->type == IDENTIFIER_NODE)
                        {
                                if (getSymbolType(node->children[0]->value) == -1)
                                {
                                        printf("Semantic error: Variable '%s' referenced before declaration\n", node->children[0]->value);
                                        exit(EXIT_FAILURE);
                                }
                        }
                        leftType = node->children[0]->dType; // Assuming literals are integers for simplicity
                }

                if (node->children[1]->type == LITERAL_NODE || node->children[1]->type == IDENTIFIER_NODE)
                {
                        if (node->children[1]->type == IDENTIFIER_NODE)
                        {
                                if (getSymbolType(node->children[1]->value) == -1)
                                {
                                        printf("Semantic error: Variable '%s' referenced before declaration\n", node->children[1]->value);
                                        exit(EXIT_FAILURE);
                                }
                        }
                        rightType = node->children[1]->dType; // Assuming literals are integers for simplicity
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
        DataType varType = getSymbolType(node->children[0]->value);
        if (varType == -1)
        {
                printf("Semantic error: Undeclared variable '%s'\n", node->children[0]->value);
                exit(EXIT_FAILURE);
        }
}

void checkForLoop(Node *node)
{
        enterNewScope(); // Enter new scope for the for loop
        if (node->numChildren != 2)
        {
                fprintf(stderr, "Error: Invalid for loop structure\n");
                exit(EXIT_FAILURE);
        }
        Node *initNode = node->children[0]; // variable declaration
        checkDeclaration(initNode);
        if (initNode->numChildren != 2)
        {
                fprintf(stderr, "Error: Invalid for loop structure\n");
                exit(EXIT_FAILURE);
        }
        Node *fromExpr = initNode->children[0];
        Node *toExpr = initNode->children[1];

        if (initNode->dType != fromExpr->dType || initNode->dType != toExpr->dType)
        {
                fprintf(stderr, "Semantic error: Type mismatch in for loop variable '%s'\n", initNode->value);
                exit(EXIT_FAILURE);
        }

        analyzeNode(node->children[1]); // Analyze loop body
        printSymbolTable();
        exitScope(); // Exit scope after the loop
}

void processConditionals(Node *node)
{
        if (node->numChildren < 2)
        {
                printf("::::::::::::::here1 %s", node->value);

                fprintf(stderr, "Error: Invalid if statement structure\n");
                exit(EXIT_FAILURE);
        }

        Node *testNode = node->children[0];
        Node *bodyNode = node->children[1];

        if (testNode->numChildren != 1)
        {
                printf("::::::::::::::here");
                fprintf(stderr, "Error: Invalid if statement structure\n");
                exit(EXIT_FAILURE);
        }
        checkBinaryOperation(testNode->children[0]);
        analyzeNode(bodyNode);
        printSymbolTable();
}

void checkIfStatement(Node *node)
{
        enterNewScope();
        processConditionals(node);
        exitScope();
}

void checkElifStatement(Node *node)
{
        printf("---------_Elif clause node");
        enterNewScope();
        processConditionals(node);
        exitScope();
}

void checkElseStatement(Node *node)
{
        enterNewScope();
        printf("---------_Else clause node");
        if (node->numChildren == 3)
        {
                Node *elseNode = node->children[2];
                analyzeNode(elseNode);
                printSymbolTable();
        }

        exitScope();
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
                return;
        case IF_STATEMENT_NODE:
                printf("IF STATEMENT NODE\n");
                checkIfStatement(node);
                break;
        case ELIF_CLAUSE_NODE:
                printf("ELIF STATEMENT NODE\n");
                checkElifStatement(node);
                break;
        case ELSE_CLAUSE_NODE:
                printf("ELSE STATEMENT NODE\n");
                checkElseStatement(node);
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
        scopedTable.currentScope = -1;
        scopedTable.maxScopes = 10;
        scopedTable.tables = malloc(scopedTable.maxScopes * sizeof(SymbolTable));
        enterNewScope(); // Starting new root parse table
        analyzeNode(root);
}
