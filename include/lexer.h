#ifndef LEXER_H
#define LEXER_H

typedef enum
{
        INT_TOKEN,
        IDENTIFIER_TOKEN,
        ASSIGN_TOKEN,
        COLON_TOKEN,
        SEMICOLON_TOKEN,
        PRINT_TOKEN,
        LPAREN_TOKEN,
        RPAREN_TOKEN,
        LITERAL_INT_TOKEN,
        PLUS_TOKEN,
        END_OF_FILE_TOKEN,
} TokenType;

typedef struct
{
        TokenType type;
        char value[100];
} Token;

typedef struct
{
        Token *tokens;
        size_t size;
        size_t capacity;
} TokenList;

TokenList tokenize(const char *source);

#endif
