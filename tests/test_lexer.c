#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "fpl.h"

FILE *strtofile(const char *str) {
    FILE *tmp = tmpfile();
    assert(tmp != NULL);

    int i = 0;

    while (str[i] != '\0') {
        fputc(str[i++], tmp);
    }

    rewind(tmp);

    return tmp;
}

void test1() {
    int res = setjmp(jmpbuf);
    if (res != 0) {
        printf("Error: %s\n", fpl_error.msg);    
        return;
    }

    char str[] = "41 + 59";
    FILE *file = strtofile(str);
    lexer_init(file);

    assert(token.type == TOKEN_EOF);

    next_token();
    assert(token.type == TOKEN_NUM);
    assert(token.val == 41);

    next_token();
    assert(token.type == TOKEN_PLUS);

    next_token();
    assert(token.type == TOKEN_NUM);
    assert(token.val == 59);

    next_token();
    assert(token.type == TOKEN_EOF);

    fclose(file);
}

void test2() {
    int res = setjmp(jmpbuf);

    if (res != 0) {
        printf("Error: %s\n", fpl_error.msg);    
        return;
    }

    char str[] = "3 +)";
    FILE *file = strtofile(str);
    lexer_init(file);
    
    next_token();
    assert(token.type == TOKEN_NUM);
    assert(token.val == 3);

    next_token();
    assert(token.type == TOKEN_PLUS);

    next_token();
    assert(token.type == TOKEN_RPAREN);

    next_token();
    assert(token.type == TOKEN_EOF);

    fclose(file);
}

void test3() {
    int res = setjmp(jmpbuf);
    if (res != 0) {
        printf("Error: %s\n", fpl_error.msg);    
        return;
    }

    char str[] = "1+2*3";
    FILE *file = strtofile(str);
    lexer_init(file);

    assert(token.type == TOKEN_EOF);

    next_token();
    assert(token.type == TOKEN_NUM);
    assert(token.val == 1);

    next_token();
    assert(token.type == TOKEN_PLUS);

    next_token();
    assert(token.type == TOKEN_NUM);
    assert(token.val == 2);

    next_token();
    assert(token.type == TOKEN_MUL);

    next_token();
    assert(token.type == TOKEN_NUM);
    assert(token.val == 3);

    next_token();
    assert(token.type == TOKEN_EOF);

    fclose(file);
}

void test4() {
    int res = setjmp(jmpbuf);
    if (res != 0) {
        printf("Error: %s\n", fpl_error.msg);    
        return;
    }

    char str[] = "@";
    FILE *file = strtofile(str);
    lexer_init(file);

    next_token();

    fclose(file);
}

int main(int argc, char **argv) {
    test1();
    test2();
    test3();
    test4();

    return EXIT_SUCCESS;
}

