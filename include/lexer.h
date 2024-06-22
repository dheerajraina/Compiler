#ifndef LEXER_H
#define LEXER_H

typedef enum
{
        INT,
        IDENTIFIER,
        ASSIGN,
        COLON,
        SEMICOLON,
        PRINT,
        LPAREN,
        RPAREN,
        LITERAL_INT,
        END_OF_FILE
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
