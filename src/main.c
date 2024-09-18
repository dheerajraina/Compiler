#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "semantic.h"
#include "codegen.h"
#include "runner.h"

char *readSourceFromFile(const char *filename)
{
        FILE *file = fopen(filename, "r");
        if (!file)
        {
                fprintf(stderr, "Could not open file %s\n", filename);
                exit(EXIT_FAILURE);
        }

        fseek(file, 0, SEEK_END);
        long length = ftell(file);
        fseek(file, 0, SEEK_SET);

        char *buffer = malloc(length + 1);
        if (!buffer)
        {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
        }

        size_t source_code_reading = fread(buffer, 1, length, file);
        buffer[length] = '\0';

        fclose(file);
        printf("File contents %s", buffer);
        return buffer;
}

const char *tokenTypeToString(TokenType type)
{
        switch (type)
        {
        case INT_TOKEN:
                return "INT";
        case IDENTIFIER_TOKEN:
                return "IDENTIFIER";
        case ASSIGN_TOKEN:
                return "ASSIGN";
        case COLON_TOKEN:
                return "COLON";
        case SEMICOLON_TOKEN:
                return "SEMICOLON";
        case PRINT_TOKEN:
                return "PRINT";
        case LPAREN_TOKEN:
                return "LPAREN";
        case RPAREN_TOKEN:
                return "RPAREN";
        case LITERAL_INT_TOKEN:
                return "LITERAL_INT";
        case PLUS_TOKEN:
                return "PLUS";
        case END_OF_FILE_TOKEN:
                return "END_OF_FILE";
        default:
                return "UNKNOWN_TYPE";
        }
}
void printTokens(TokenList tokens)
{
        for (size_t i = 0; i < tokens.size; i++)
        {
                printf("Token Type: %s, Value: %s\n", tokenTypeToString(tokens.tokens[i].type), tokens.tokens[i].value);
        }
}

void printAST(Node *node, int level)
{
        if (node == NULL)
                return;

        // Print current node
        printf("%*sNode Type: %d, Value: %s\n", level * 2, "", node->type, node->value);

        // Recursively print children
        for (int i = 0; i < node->numChildren; i++)
        {
                if (node->children[i] != NULL)
                {
                        printAST(node->children[i], level + 1);
                }
        }
}

int main()
{
        char *sourceCode = readSourceFromFile("test/test_source.txt");
        TokenList tokens = tokenize(sourceCode);
        printf("Tokens:\n");
        printTokens(tokens);

        Node *syntaxTree = parse(&tokens);
        printf("\nAbstract Syntax Tree:\n");
        printAST(syntaxTree, 0);

        analyze(syntaxTree);
        generate(syntaxTree);
        printf("\n\n\n\n");
        printf("Final Source Code Run\n");
        run();

        return EXIT_SUCCESS;
}
