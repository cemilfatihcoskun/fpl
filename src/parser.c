#include <stdio.h>
#include <stdlib.h>

#include "fpl.h"

struct Token saved_token;
struct Token saved_token2;
bool has_saved_token = false;
bool use_saved_token = false;

void peek() {
    saved_token = token;
    has_saved_token = true;
    use_saved_token = true;
    next_token();
}

void get_token() {
    if (has_saved_token) {
        if (use_saved_token) {
            saved_token2 = token;
            token = saved_token;
            use_saved_token = false;
        }
        else {
            has_saved_token = false;
            token = saved_token2;
        }
    }
    else {
        next_token();
    }
    //token_print(token);
}

/* F = [0..9] */
int parse_F() {
    // printf("fact\n");
    int val = 0;

    if (token.type == TOKEN_LPAREN) {
        get_token();
        printf("\ta\t");
        token_print(token);

        val = parse_E();

        // ) error
        if (token.type != TOKEN_RPAREN) {
            fpl_error.type = ERROR_PARANTHESIS_IS_NOT_CLOSED;
            snprintf(fpl_error.msg, sizeof(fpl_error.msg), "Parenthesis is not closed");
            longjmp(jmpbuf, 1);
        }
        get_token();
    }
    else if (token.type == TOKEN_SYM) {
        val = symbols[token.name - 'a'];
        get_token();
    }
    else if (token.type == TOKEN_NUM) {
        val = token.val;
        get_token();
    }
    else if (token.type == TOKEN_MINUS) {
        get_token();
        if (token.type != TOKEN_NUM) {
            snprintf(fpl_error.msg, sizeof(fpl_error.msg), "A number should come after");
            longjmp(jmpbuf, 1);
        }
        val = -parse_F();
    }
    else if (token.type == TOKEN_PLUS) {
        get_token();
        if (token.type != TOKEN_NUM) {
            snprintf(fpl_error.msg, sizeof(fpl_error.msg), "A number should come after");
            longjmp(jmpbuf, 1);
        }
        val = parse_F();
    }
    else if (token.type == TOKEN_EOF) {
    }
    else {
        fpl_error.type = ERROR_INCORRECT_GRAMMAR;
        snprintf(fpl_error.msg, sizeof(fpl_error.msg), "Incorrect token type %d", token.type);
        longjmp(jmpbuf, 1);
    }

    return val;
}

/* T = F ('*' F)* */
int parse_T() {
    // printf("term\n");
    int val = parse_F();
    while (token.type == TOKEN_MUL || token.type == TOKEN_DIV) {
        bool is_mul = true;
        if (token.type == TOKEN_DIV) {
            is_mul = false;
        }
        get_token();

        int right = parse_F();
        if (is_mul) {
            val *= right;
        } else {
            if (right == 0) {
                fpl_error.type = ERROR_DIVISION_BY_ZERO;
                snprintf(fpl_error.msg, sizeof(fpl_error.msg), "Division by zero");
                longjmp(jmpbuf, 1);
                fprintf(stderr, "Division by zero");
                exit(EXIT_FAILURE);
            }
            val /= right;
        }
    }

    return val;
}

/* E = T ('+' T) */
int parse_E() {
    // printf("expr\n");
    int val = parse_T();

    while (token.type == TOKEN_PLUS || token.type == TOKEN_MINUS) {
        bool is_sum = true;
        if (token.type == TOKEN_MINUS) {
            is_sum = false;
        }
        get_token();

        int right = parse_T();
        if (is_sum) {
            val += right;
        } else {
            val -= right;
        }
    }

    return val;
}

void parse_S() {
}

/* L = S | E */
int parse_L() {
    int val = 0;

    if (token.type == TOKEN_SYM) {
        peek();
        if (token.type == TOKEN_ASSIGN) {
            int index = token.name - 'a';

            get_token();
            get_token();
            get_token();
            int right = parse_E();
            symbols[index] = right;
            val = right;
        }
        else {
            get_token();
            val = parse_E();
        }
    }
    else {
        val = parse_E();
    }

    return val;
}

