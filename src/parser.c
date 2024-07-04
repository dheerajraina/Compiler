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
                if (token.type == INT_TOKEN)
                {
                        printf("Entered parse token INT\n\n");
                        return parseDeclaration(parser);
                }
                else if (token.type == IDENTIFIER_TOKEN && parser->tokens[parser->pos + 1].type == ASSIGN_TOKEN)
                {
                        printf("Entered parse token IDENTIFIER\n\n");
                        return parseAssignment(parser);
                }
                else if (token.type == PRINT_TOKEN)
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

        if (parser->tokens[parser->pos].type == IDENTIFIER_TOKEN)
        {
                printf("inside declaration %s\n\n", parser->tokens[parser->pos].value);
                char *identifier = parser->tokens[parser->pos++].value;
                if (parser->tokens[parser->pos].type != ASSIGN_TOKEN)
                {
                        Node *node = createNode(DECLARATION_NODE, identifier);
                        return node;
                }
                else
                {
                        Node *node = createNode(DECLARATION_ASSIGNMENT_NODE, identifier);
                        parser->pos++; // Skip ':='
                        if (parser->tokens[parser->pos].type == LITERAL_INT_TOKEN)
                        {
                                if (parser->tokens[parser->pos + 1].type != PLUS_TOKEN)
                                        node->children[0] = createNode(LITERAL_NODE, parser->tokens[parser->pos++].value);
                                else
                                {
                                        printf("bin op encountered\n\n");
                                        Node *left = createNode(LITERAL_NODE, parser->tokens[parser->pos++].value);
                                        printf("left value %s\n\n", left->value);
                                        while (parser->tokens[parser->pos].type == PLUS_TOKEN)
                                        {
                                                Token operator= parser->tokens[parser->pos++];
                                                Token next_token = parser->tokens[parser->pos++];
                                                Node *right = createNode(LITERAL_NODE, next_token.value);
                                                Node *bin_op_node = createNode(BIN_OP_NODE, operator.value);
                                                printf("right value %s\n\n", next_token.value);
                                                printf("op value %s\n\n", operator.value);
                                                // node->children[0] = bin_op_node;
                                                bin_op_node->children[0] = left;
                                                bin_op_node->children[1] = right;
                                                // node->children[1] = right;
                                                left = bin_op_node;
                                                printf("Index value %s\n\n", parser->tokens[parser->pos].value);
                                        }
                                        node->children[0] = left;
                                }

                                return node;
                        }
                }
        }
        return NULL;
}

Node *parseAssignment(Parser *parser)
{
        Node *node = createNode(ASSIGNMENT_NODE, parser->tokens[parser->pos++].value);
        parser->pos++; // Skip ':='

        if (parser->tokens[parser->pos].type == LITERAL_INT_TOKEN)
        {
                node->children[0] = createNode(LITERAL_NODE, parser->tokens[parser->pos++].value);
        }

        return node;
}

Node *parsePrintStatement(Parser *parser)
{
        parser->pos++; // Skip 'print'

        if (parser->tokens[parser->pos].type == LPAREN_TOKEN)
        {
                parser->pos++; // Skip '('
        }

        if (parser->tokens[parser->pos].type == IDENTIFIER_TOKEN)
        {
                Node *node = createNode(PRINT_STATEMENT_NODE, "print");
                node->children[0] = createNode(IDENTIFIER_NODE, parser->tokens[parser->pos++].value);

                if (parser->tokens[parser->pos].type == RPAREN_TOKEN)
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

        Node *root = createNode(PROGRAM_NODE, "root");

        size_t childIndex = 0;

        while (parser.pos < parser.size)
        {
                printf("parse function while %ld, %ld \n", parser.pos, parser.size);
                Node *child = parseToken(&parser);
                if (child)
                {
                        printf("\n\n----------node value %s\n\n", child->value);

                        root->children[childIndex++] = child;
                        printf("root %s\n\n", root->children[childIndex - 1]->value);
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
