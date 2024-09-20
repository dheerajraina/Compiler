#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum
{
        INT,
        NONE
} DataType;
typedef enum
{
        PROGRAM_NODE,
        DECLARATION_NODE,
        ASSIGNMENT_NODE,
        PRINT_STATEMENT_NODE,
        LITERAL_NODE,
        EXPRESSION_NODE,
        BIN_OP_NODE,
        DECLARATION_ASSIGNMENT_NODE,
        IDENTIFIER_NODE,
        FOR_LOOP_NODE,
        LOOP_BODY_NODE,

} NodeType;

typedef struct Node
{
        DataType dType;
        NodeType type;
        char value[100];
        struct Node **children;
        int numChildren;
        int capacity;
} Node;

Node *parse(TokenList *tokenList);
Node *createNode(NodeType type, const char *value, const DataType dType);
void addChild(Node *parent, Node *child);
void expandNodeChildren(Node *node);
void freeNode(Node *node);

#endif
