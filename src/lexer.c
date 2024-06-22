#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

#define INITIAL_CAPACITY 10

void addToken(TokenList *list, TokenType type, const char *value)
{
        if (list->size >= list->capacity)
        {
                list->capacity *= 2;
                list->tokens = realloc(list->tokens, list->capacity * sizeof(Token));
        }
        list->tokens[list->size].type = type;
        strcpy(list->tokens[list->size].value, value);
        list->size++;
}

TokenList tokenize(const char *source)
{
        TokenList list;
        list.size = 0;
        list.capacity = INITIAL_CAPACITY;
        list.tokens = malloc(list.capacity * sizeof(Token));

        const char *keywords[] = {"int", "integer", "print"};
        const char *ptr = source;

        while (*ptr)
        {
                while (isspace(*ptr))
                        ptr++;
                if (isdigit(*ptr))
                {
                        const char *start = ptr;
                        while (isdigit(*ptr))
                                ptr++;
                        addToken(&list, LITERAL_INT, strndup(start, ptr - start));
                }
                else if (isalpha(*ptr))
                {
                        const char *start = ptr;
                        while (isalnum(*ptr))
                                ptr++;
                        char *word = strndup(start, ptr - start);
                        if (strcmp(word, "int") == 0 || strcmp(word, "integer") == 0)
                        {
                                addToken(&list, INT, word);
                        }
                        else if (strcmp(word, "print") == 0)
                        {
                                addToken(&list, PRINT, word);
                        }
                        else
                        {
                                addToken(&list, IDENTIFIER, word);
                        }
                        free(word);
                }
                else
                {
                        switch (*ptr)
                        {
                        case ':':
                                if (*(ptr + 1) == '=')
                                {
                                        addToken(&list, ASSIGN, ":=");
                                        ptr++;
                                }
                                else
                                {
                                        addToken(&list, COLON, ":");
                                }
                                break;
                        case ';':
                                addToken(&list, SEMICOLON, ";");
                                break;
                        case '(':
                                addToken(&list, LPAREN, "(");
                                break;
                        case ')':
                                addToken(&list, RPAREN, ")");
                                break;
                        default:
                                ptr++;
                                break;
                        }
                        ptr++;
                }
        }
        addToken(&list, END_OF_FILE, "");
        return list;
}
