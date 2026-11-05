#ifndef FPL_H
#define FPL_H

#include <stdbool.h>
#include <setjmp.h>

#include <stdio.h>

enum TokenType {
    TOKEN_NUM,
    TOKEN_SYM,
    TOKEN_ASSIGN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_MUL,
    TOKEN_DIV,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_EOF,
    TOKEN_NL,
};

struct Token {
    enum TokenType type;
    int val;
    char name;
};

enum ErrorType {
    /* Lexer Errors */
    ERROR_UNDEFINED_CHARACTER,
    
    /* Parser Errors */
    ERROR_INCORRECT_GRAMMAR,
    ERROR_PARANTHESIS_IS_NOT_CLOSED,
    
    
    ERROR_DIVISION_BY_ZERO,
};

struct FPL_Error {
    enum ErrorType type;
    char msg[64];
};

extern struct FPL_Error fpl_error;

#define SYMBOLS_SIZE 32
extern int symbols[SYMBOLS_SIZE];
extern struct Token token;

extern jmp_buf jmpbuf;

void lexer_init(FILE *file);
void next_char();
void token_print(struct Token tok);
void next_token();
void lexer_recover();

int parse_E();
int parse_F();
int parse_T();
int parse_E();
void parse_S();
int parse_L();

#endif

