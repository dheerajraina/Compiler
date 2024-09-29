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

Node *createNode(NodeType type, const char *value, DataType dType)
{
        Node *node = malloc(sizeof(Node));
        if (node == NULL)
        {
                fprintf(stderr, "Memory allocation failed for node\n");
                exit(EXIT_FAILURE);
        }

        node->type = type;
        strncpy(node->value, value, sizeof(node->value) - 1);
        node->numChildren = 0;
        node->capacity = 2;
        node->children = malloc(sizeof(Node *) * node->capacity);
        node->dType = dType;

        return node;
}

void addChild(Node *parent, Node *child)
{
        if (parent->numChildren >= parent->capacity)
        {
                expandNodeChildren(parent);
        }
        parent->children[parent->numChildren++] = child;
}
void expandNodeChildren(Node *node)
{
        node->capacity *= 2;
        node->children = realloc(node->children, sizeof(Node *) * node->capacity);
}

void freeNode(Node *node)
{
        for (int i = 0; i < node->numChildren; i++)
        {
                freeNode(node->children[i]);
        }
        free(node->children);
        free(node);
}
Node *parseDeclaration(Parser *parser);
Node *parseAssignment(Parser *parser);
Node *parsePrintStatement(Parser *parser);
Node *parseForLoop(Parser *parser);
Node *parseIfStatement(Parser *parser);

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
                else if (token.type == FOR_KEYWORD)
                {
                        printf("-----------FOR KEYWORD");
                        return parseForLoop(parser);
                }
                else if (token.type == IF_KEYWORD)
                {
                        printf("-----------------IF KEYWORD");
                        return parseIfStatement(parser);
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
                        Node *node = createNode(DECLARATION_NODE, identifier, INT);
                        return node;
                }
                else
                {
                        Node *node = createNode(DECLARATION_ASSIGNMENT_NODE, identifier, INT);
                        parser->pos++; // Skip ':='
                        if (parser->tokens[parser->pos].type == LITERAL_INT_TOKEN)
                        {
                                if (parser->tokens[parser->pos + 1].type != PLUS_TOKEN)

                                        addChild(node, createNode(LITERAL_NODE, parser->tokens[parser->pos++].value, INT));
                                else
                                {
                                        printf("bin op encountered\n\n");
                                        Node *left = createNode(LITERAL_NODE, parser->tokens[parser->pos++].value, INT);
                                        printf("left value %s\n\n", left->value);
                                        while (parser->tokens[parser->pos].type == PLUS_TOKEN)
                                        {
                                                Token operator= parser->tokens[parser->pos++];
                                                Token next_token = parser->tokens[parser->pos++];
                                                Node *right = createNode(LITERAL_NODE, next_token.value, INT);
                                                Node *bin_op_node = createNode(BIN_OP_NODE, operator.value, NONE);
                                                printf("right value %s\n\n", next_token.value);
                                                printf("op value %s\n\n", operator.value);
                                                // node->children[0] = bin_op_node;
                                                addChild(bin_op_node, left);
                                                addChild(bin_op_node, right);

                                                left = bin_op_node;
                                                printf("Index value %s\n\n", parser->tokens[parser->pos].value);
                                        }
                                        addChild(node, left);
                                }

                                return node;
                        }
                }
        }
        return NULL;
}

Node *parseAssignment(Parser *parser)
{
        Node *node = createNode(ASSIGNMENT_NODE, parser->tokens[parser->pos++].value, INT); // TODO: fetch variable's datatype from declaration

        parser->pos++; // Skip ':='

        if (parser->tokens[parser->pos].type == LITERAL_INT_TOKEN)
        {
                if (parser->tokens[parser->pos + 1].type != PLUS_TOKEN)
                {
                        addChild(node, createNode(LITERAL_NODE, parser->tokens[parser->pos++].value, INT));
                }
                else
                {
                        printf("bin op encountered\n\n");
                        Node *left = createNode(LITERAL_NODE, parser->tokens[parser->pos++].value, INT);
                        printf("left value %s\n\n", left->value);
                        while (parser->tokens[parser->pos].type == PLUS_TOKEN)
                        {
                                Token operator= parser->tokens[parser->pos++];
                                Token next_token = parser->tokens[parser->pos++];
                                Node *right = createNode(LITERAL_NODE, next_token.value, INT);
                                Node *bin_op_node = createNode(BIN_OP_NODE, operator.value, NONE);
                                printf("right value %s\n\n", next_token.value);
                                printf("op value %s\n\n", operator.value);
                                // node->children[0] = bin_op_node;
                                addChild(bin_op_node, left);
                                addChild(bin_op_node, right);

                                left = bin_op_node;
                                printf("Index value %s\n\n", parser->tokens[parser->pos].value);
                        }
                        addChild(node, left);
                }
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
                Node *node = createNode(PRINT_STATEMENT_NODE, "print", NONE);
                addChild(node, createNode(IDENTIFIER_NODE, parser->tokens[parser->pos++].value, NONE));

                if (parser->tokens[parser->pos].type == RPAREN_TOKEN)
                {
                        parser->pos++; // Skip ')'
                }

                return node;
        }

        return NULL;
}

Node *parseForLoop(Parser *parser)
{

        Node *forNode = createNode(FOR_LOOP_NODE, "for", NONE);
        Node *variable;
        Node *rangeStart;
        Node *rangeEnd;
        parser->pos++; // Skip for
        if (parser->tokens[parser->pos].type != IDENTIFIER_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        printf("\n\n1\n");
        char *identifier = parser->tokens[parser->pos++].value;
        variable = createNode(DECLARATION_NODE, identifier, INT);
        addChild(forNode, variable);
        if (parser->tokens[parser->pos++].type != FROM_KEYWORD || parser->tokens[parser->pos].type != LITERAL_INT_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        printf("\n\n2\n\n");
        rangeStart = createNode(FROM_EXPR_NODE, parser->tokens[parser->pos++].value, INT);
        addChild(variable, rangeStart);
        if (parser->tokens[parser->pos++].type != TO_KEYWORD || parser->tokens[parser->pos].type != LITERAL_INT_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        printf("3");
        rangeEnd = createNode(TO_EXPR_NODE, parser->tokens[parser->pos++].value, INT);
        addChild(variable, rangeEnd);

        if (parser->tokens[parser->pos++].type != OPEN_BRACE_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        Node *loopBody = createNode(LOOP_BODY_NODE, "loop_body", NONE); // subtree representing body of for loop

        while (parser->tokens[parser->pos].type != CLOSE_BRACE_TOKEN)
        {
                // printf("\nEntered for loop body -> %d\n", parser->tokens[parser->pos].type);

                addChild(loopBody, parseToken(parser));
                // parser->pos++;
        }

        if (parser->tokens[parser->pos++].type != CLOSE_BRACE_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        if (loopBody)
        {
                addChild(forNode, loopBody);
        }

        return forNode;
}

Node *parseIfStatement(Parser *parser)
{
        Node *ifNode = createNode(IF_STATEMENT_NODE, "if", NONE);
        parser->pos++;
        if (parser->tokens[parser->pos++].type != LPAREN_TOKEN || (parser->tokens[parser->pos].type != IDENTIFIER_TOKEN && parser->tokens[parser->pos].type != LITERAL_INT_TOKEN))
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        Node *left = parser->tokens[parser->pos].type == LITERAL_INT_TOKEN ? createNode(LITERAL_NODE, parser->tokens[parser->pos].value, INT) : createNode(IDENTIFIER_NODE, parser->tokens[parser->pos++].value, INT);
        if (parser->tokens[parser->pos].type != EQUAL_TO_TOKEN && parser->tokens[parser->pos].type != LESS_THAN_TOKEN && parser->tokens[parser->pos].type != GREATER_THAN_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s ; Comparison operator expected'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        Node *bin_op_node = createNode(BIN_OP_NODE, parser->tokens[parser->pos].value, NONE);
        parser->pos++;

        if (parser->tokens[parser->pos].type != IDENTIFIER_TOKEN && parser->tokens[parser->pos].type != LITERAL_INT_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        parser->pos++;
        if (parser->tokens[parser->pos].type != RPAREN_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        parser->pos--;
        Node *testNode = createNode(CONDITIONAL_TEST_NODE, "test", NONE);
        Node *right = parser->tokens[parser->pos].type == LITERAL_INT_TOKEN ? createNode(LITERAL_NODE, parser->tokens[parser->pos].value, INT) : createNode(IDENTIFIER_NODE, parser->tokens[parser->pos++].value, INT);
        addChild(ifNode, testNode);
        addChild(testNode, bin_op_node);
        addChild(bin_op_node, left);
        addChild(bin_op_node, right);

        parser->pos++;
        Node *conditionalBody = createNode(CONDITIONAL_BODY_NODE, "body", NONE);
        if (parser->tokens[parser->pos++].type != OPEN_BRACE_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        while (parser->tokens[parser->pos].type != CLOSE_BRACE_TOKEN)
        {
                addChild(conditionalBody, parseToken(parser));
        }

        if (parser->tokens[parser->pos++].type != CLOSE_BRACE_TOKEN)
        {
                printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                exit(EXIT_FAILURE);
        }
        if (conditionalBody)
        {
                addChild(ifNode, conditionalBody);
        }
        if (parser->tokens[parser->pos].type == ELSE_KEYWORD)
        {
                parser->pos++;
                Node *elseNode = createNode(ELSE_CLAUSE_NODE, "else", NONE);
                Node *elseBody = createNode(CONDITIONAL_BODY_NODE, "body", NONE);

                if (parser->tokens[parser->pos++].type != OPEN_BRACE_TOKEN)
                {
                        printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                        exit(EXIT_FAILURE);
                }
                while (parser->tokens[parser->pos].type != CLOSE_BRACE_TOKEN)
                {
                        addChild(elseBody, parseToken(parser));
                }

                if (parser->tokens[parser->pos].type != CLOSE_BRACE_TOKEN)
                {
                        printf("Syntax error: Unexpected token '%s'\n ", parser->tokens[parser->pos].value);
                        exit(EXIT_FAILURE);
                }
                if (elseBody)
                {
                        addChild(elseNode, elseBody);
                        addChild(ifNode, elseNode);
                        printf("--------parser after else %s", parser->tokens[parser->pos].value);
                }
                parser->pos++;
        }

        return ifNode;
}
Node *parse(TokenList *tokenList)
{
        Parser parser;
        parser.tokens = tokenList->tokens;
        parser.pos = 0;
        parser.size = tokenList->size;

        Node *root = createNode(PROGRAM_NODE, "root", NONE);

        while (parser.pos < parser.size)
        {
                printf("parse function while %ld, %ld \n", parser.pos, parser.size);
                Node *child = parseToken(&parser);
                if (child)
                {
                        printf("\n\n----------node value %s\n\n", child->value);

                        addChild(root, child);
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
