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
                        addToken(&list, LITERAL_INT_TOKEN, strndup(start, ptr - start));
                }
                else if (isalpha(*ptr))
                {
                        const char *start = ptr;
                        while (isalnum(*ptr))
                                ptr++;
                        char *word = strndup(start, ptr - start);
                        if (strcmp(word, "int") == 0 || strcmp(word, "integer") == 0)
                        {
                                addToken(&list, INT_TOKEN, word);
                        }
                        else if (strcmp(word, "print") == 0)
                        {
                                addToken(&list, PRINT_TOKEN, word);
                        }
                        else if (strcmp(word, "for") == 0)
                        {
                                addToken(&list, FOR_KEYWORD, word);
                        }
                        else if (strcmp(word, "from") == 0)
                        {
                                addToken(&list, FROM_KEYWORD, word);
                        }
                        else if (strcmp(word, "to") == 0)
                        {
                                addToken(&list, TO_KEYWORD, word);
                        }
                        else
                        {
                                addToken(&list, IDENTIFIER_TOKEN, word);
                        }
                        free(word);
                }
                else
                {
                        // printf("----------current token %s", ptr);
                        switch (*ptr)
                        {
                        case ':':
                                if (*(ptr + 1) == '=')
                                {
                                        addToken(&list, ASSIGN_TOKEN, ":=");
                                        ptr++;
                                }
                                else
                                {
                                        addToken(&list, COLON_TOKEN, ":");
                                }
                                break;
                        case ';':
                                addToken(&list, SEMICOLON_TOKEN, ";");
                                break;
                        case '(':
                                addToken(&list, LPAREN_TOKEN, "(");
                                break;
                        case ')':
                                addToken(&list, RPAREN_TOKEN, ")");
                                break;
                        case '{':
                                addToken(&list, OPEN_BRACE_TOKEN, "{");
                                break;
                        case '}':
                                addToken(&list, CLOSE_BRACE_TOKEN, "}");
                                break;
                        case '+':
                                addToken(&list, PLUS_TOKEN, "+");
                                break;
                        case '>':
                                addToken(&list, GREATER_THAN_TOKEN, ">");
                                break;
                        case '<':
                                addToken(&list, LESS_THAN_TOKEN, "<");
                                break;
                        case '=':
                                if (*(ptr + 1) == '=')
                                {
                                        addToken(&list, EQUAL_TO_TOKEN, "==");
                                        ptr++;
                                }
                                break;
                        default:
                                ptr++;
                                break;
                        }
                        ptr++;
                }
        }
        addToken(&list, END_OF_FILE_TOKEN, "");
        return list;
}
