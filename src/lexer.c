#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <setjmp.h>

#include "fpl.h"

static FILE *file;
int lookahead;

bool isDebug = false;

int line = 0;
int col = 0;

void lexer_clean() {
    token.type = TOKEN_EOF;
    token.val = 0;
    token.name = 'T';

    lookahead = EOF;

    line = 0;
    col = 0;
}

void lexer_init(FILE *paramFile) {
    file = paramFile; 
    lexer_clean();
}

void next_char() {
    if (file == NULL) {
        return;
    }

    do {
        if (isDebug) {
            printf("next_char=%c\n", lookahead);
        }

        lookahead = fgetc(file);
        col++;

        if (lookahead == '\n') {
            line++;
            col = 0;
        }
    } while (lookahead == ' ' /* || lookahead == '\n' */ || lookahead == '\r' ||
             lookahead == '\t');
}

void lexer_recover() {
    if (file == NULL) {
        return;
    }

    while (lookahead != '\n' && lookahead != EOF) {
        next_char();
    }
    
    col = 0;
    line += 1;
    ungetc(lookahead, file);
    next_token();
}

void token_print(struct Token tok) {
    switch (tok.type) {
    case TOKEN_LPAREN:
        printf("TOKEN_LPAREN\n");
        break;
    case TOKEN_RPAREN:
        printf("TOKEN_RPAREN\n");
        break;
    case TOKEN_EOF:
        printf("TOKEN_EOF\n");
        break;
    case TOKEN_PLUS:
        printf("TOKEN_PLUS\n");
        break;
    case TOKEN_MINUS:
        printf("TOKEN_MINUS\n");
        break;
    case TOKEN_MUL:
        printf("TOKEN_MUL\n");
        break;
    case TOKEN_DIV:
        printf("TOKEN_DIV\n");
        break;
    case TOKEN_NUM:
        printf("TOKEN_NUM (%d)\n", tok.val);
        break;
    case TOKEN_SYM:
        printf("TOKEN_SYM (%c)\n", tok.name);
        break;
    case TOKEN_ASSIGN:
        printf("TOKEN_ASSIGN\n");
        break;
    case TOKEN_NL:
        printf("TOKEN_NL\n");
        break;
    default:
        printf("TOKEN_LPAREN\n");
        break;
    }
}

void next_token() {
    next_char();
    if (lookahead == '(') {
        token.type = TOKEN_LPAREN;
    }
    else if (lookahead == ')') {
        token.type = TOKEN_RPAREN;
    }
    else if (lookahead >= '0' && lookahead <= '9') {
        int val = 0;
        while (lookahead >= '0' && lookahead <= '9') {
            int num = lookahead - '0';
            val = val * 10 + num;
            next_char();
        }
        
        /* For +, * symbols */
        if (lookahead != EOF) {
            ungetc(lookahead, file);
            col--;
        }

        token.type = TOKEN_NUM;
        token.val = val;
    }
    else if (lookahead >= 'a' && lookahead <= 'z') {
        /*
        Todo: Add a hash table for multi letter symbols
        */

        token.type = TOKEN_SYM;
        token.name = lookahead; 
    }
    else if (lookahead == '+') {
        token.type = TOKEN_PLUS;
    }
    else if (lookahead == '-') {
        token.type = TOKEN_MINUS;
    }
    else if (lookahead == '*') {
        token.type = TOKEN_MUL;
    }
    else if (lookahead == '/') {
        token.type = TOKEN_DIV;
    }
    else if (lookahead == '=') {
        token.type = TOKEN_ASSIGN;
    }
    else if (lookahead == EOF) {
        token.type = TOKEN_EOF;
    }
    else if (lookahead == '\n') {
        token.type = TOKEN_NL;
    }
    else {
        snprintf(fpl_error.msg, sizeof(fpl_error.msg), "Line %d, Column %d, %c.", line, col, lookahead);
        fpl_error.type = ERROR_UNDEFINED_CHARACTER;

        longjmp(jmpbuf, 1);
    }
    
    if (isDebug) {
        printf("Next Token: ");
        token_print(token);
    }
}

