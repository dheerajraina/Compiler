#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

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

} NodeType;

typedef struct Node
{
        NodeType type;
        char value[100];
        struct Node *children[2];
} Node;

Node *parse(TokenList *tokenList);

#endif
