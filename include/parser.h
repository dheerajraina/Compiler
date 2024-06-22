#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

typedef enum
{
        DECLARATION,
        ASSIGNMENT,
        PRINT_STATEMENT,
        LITERAL
} NodeType;

typedef struct Node
{
        NodeType type;
        char value[100];
        struct Node *children[2];
} Node;

Node *parse(TokenList *tokenList);

#endif
