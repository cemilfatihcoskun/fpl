#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

FILE *file;
int lookahead;

void next_char() {
    if (file == NULL) {
        return;
    }

    do {
        lookahead = fgetc(file);
    } while (lookahead == ' ' || lookahead == '\n' || lookahead == '\r' || lookahead == '\t');
}

/* F = [0..9] */
int parse_F() {
    int init = 0;
    int sign = 1;
    if (lookahead == '-') {
        sign = -1;
        next_char();
    }
    int val = 0;
    while (lookahead >= '0' && lookahead <= '9') {
        init = 1;
        int num = lookahead - '0';
        val = val * 10 + num;
        next_char();
    }
    if (!init) {
        if (lookahead == EOF) {
            fprintf(stderr, "Error: Unexpected end of file.\n");
        }
        else {
            fprintf(stderr, "Error: %c is not a digit. A digit between 0 and 9 is expected.\n", lookahead);
        }
        exit(EXIT_FAILURE);
    }
    return val * sign;
}

/* T = F * F | F */
int parse_T() {
    int val = parse_F();
    while (lookahead == '*' || lookahead == '/') {
        bool is_mul = true; 
        if (lookahead == '/') {
            is_mul = false;
        }
        next_char();

        int right = parse_F();
        if (is_mul) {
            val *= right;
        }
        else {
            if (right == 0) {
                fprintf(stderr, "Error: Division by zero.\n");
                exit(EXIT_FAILURE);
            }
            val /= right;
        }
    }

    return val;
}

/* E = T + T | T */
int parse_E() {
    int val = parse_T();

    while (lookahead == '+' || lookahead == '-') {
        bool is_sum = true;
        if (lookahead == '-') {
            is_sum = false;
        }
        next_char();
        int right = parse_T();
        if (is_sum) {
            val += right;
        }
        else {
            val -= right;
        }
    }

    return val;
}

int main(int argc, char **argv) {
    bool is_file = false;
    char filename[] = "tests/first.fpl";

    printf("argc=%d\n", argc);
    if (argc > 1) {
        is_file = true;
        file = fopen(argv[1], "r");
    }
    else {
        file = stdin;
    }
    
    if (file == NULL) {
        fprintf(stderr, 
                "Error: File %s could not be opened.\n",
                (argc > 1) ? argv[1] : "stdin");
        return EXIT_FAILURE;
    }

    next_char();

    int result = parse_E();

    printf("Result=%d\n", result);

    if (is_file) {
        fclose(file);
    }

    return EXIT_SUCCESS;
}

