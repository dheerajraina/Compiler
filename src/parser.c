#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

typedef struct
{
        Token *tokens;
        size_t pos;
        size_t size;
} Parser;

Node *createNode(NodeType type, const char *value)
{
        Node *node = malloc(sizeof(Node));
        if (node == NULL)
        {
                fprintf(stderr, "Memory allocation failed for node\n");
                exit(EXIT_FAILURE);
        }

        node->type = type;
        strncpy(node->value, value, sizeof(node->value) - 1);
        node->value[sizeof(node->value) - 1] = '\0';
        node->children[0] = NULL;
        node->children[1] = NULL;

        return node;
}

Node *parseDeclaration(Parser *parser);
Node *parseAssignment(Parser *parser);
Node *parsePrintStatement(Parser *parser);

Node *parseToken(Parser *parser)
{
        if (parser->pos < parser->size)
        {
                printf("Entered parse token \n");
                Token token = parser->tokens[parser->pos];
                printf("%d,%d \n\n", token.type, parser->tokens[parser->pos + 1].type);
                if (token.type == INT)
                {
                        printf("Entered parse token INT\n\n");
                        return parseDeclaration(parser);
                }
                else if (token.type == IDENTIFIER && parser->tokens[parser->pos + 1].type == ASSIGN)
                {
                        printf("Entered parse token IDENTIFIER\n\n");
                        return parseAssignment(parser);
                }
                else if (token.type == PRINT)
                {
                        printf("Entered parse token PRINT\n\n");
                        return parsePrintStatement(parser);
                }
        }
        return NULL;
}

Node *parseDeclaration(Parser *parser)
{

        char *data_type = strcat(parser->tokens[parser->pos].value, " ");

        parser->pos++; // Skip 'int' or 'integer'

        if (parser->tokens[parser->pos].type == IDENTIFIER)
        {
                printf("inside declaration %s\n\n", parser->tokens[parser->pos].value);
                char *identifier = parser->tokens[parser->pos++].value;
                Node *node = createNode(DECLARATION, identifier);
                // node->children[0] = createNode(IDENTIFIER, identifier);
                if (parser->tokens[parser->pos].type == ASSIGN)
                {
                        parser->pos++; // Skip ':='
                        if (parser->tokens[parser->pos].type == LITERAL_INT)
                        {
                                node->children[0] = createNode(LITERAL, parser->tokens[parser->pos++].value);
                        }
                }

                return node;
        }

        return NULL;
}

Node *parseAssignment(Parser *parser)
{
        Node *node = createNode(ASSIGNMENT, parser->tokens[parser->pos++].value);
        parser->pos++; // Skip ':='

        if (parser->tokens[parser->pos].type == LITERAL_INT)
        {
                node->children[0] = createNode(LITERAL, parser->tokens[parser->pos++].value);
        }

        return node;
}

Node *parsePrintStatement(Parser *parser)
{
        parser->pos++; // Skip 'print'

        if (parser->tokens[parser->pos].type == LPAREN)
        {
                parser->pos++; // Skip '('
        }

        if (parser->tokens[parser->pos].type == IDENTIFIER)
        {
                Node *node = createNode(PRINT_STATEMENT, "print");
                node->children[0] = createNode(IDENTIFIER, parser->tokens[parser->pos++].value);

                if (parser->tokens[parser->pos].type == RPAREN)
                {
                        parser->pos++; // Skip ')'
                }

                return node;
        }

        return NULL;
}

Node *parse(TokenList *tokenList)
{
        Parser parser;
        parser.tokens = tokenList->tokens;
        parser.pos = 0;
        parser.size = tokenList->size;

        Node *root = createNode(DECLARATION, "root");

        size_t childIndex = 0;

        while (parser.pos < parser.size)
        {
                printf("parse function while %ld, %ld \n", parser.pos, parser.size);
                Node *child = parseToken(&parser);
                if (child)
                {
                        root->children[childIndex++] = child;
                }
                else
                {
                        printf("No valid token");
                        // If no valid token is found, we should break to avoid an infinite loop
                        break;
                }
        }

        return root;
}
