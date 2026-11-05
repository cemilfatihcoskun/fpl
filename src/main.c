#include <stdio.h>
#include <stdlib.h>

#include "fpl.h"

int main(int argc, char** argv) {
    for (int i = 0; i < SYMBOLS_SIZE; i++) {
        symbols[i] = -1;
    }

    bool is_file = false;
    char filename[] = "tests/first.fpl";

    FILE *file;
    if (argc > 1) {
        is_file = true;
        file = fopen(argv[1], "r");
    } else {
        file = stdin;
    }

    if (file == NULL) {
        fprintf(stderr, "Error: File %s could not be opened.\n",
                (argc > 1) ? argv[1] : "stdin");
        return EXIT_FAILURE;
    }

    lexer_init(file);
    do {
        printf("usr> ");
        fflush(stdout);

        int err = setjmp(jmpbuf);
        if (err) {
            fprintf(stderr, "Error: %s.\n", fpl_error.msg);
            lexer_recover();
            continue;
        }

        next_token();

        if (token.type == TOKEN_NL) {
            continue;
        }

        int res = parse_L();
        if (token.type != TOKEN_NL && token.type != TOKEN_EOF) {
            fpl_error.type = ERROR_INCORRECT_GRAMMAR;
            snprintf(fpl_error.msg, sizeof(fpl_error.msg), "Unexpected Token");
            fprintf(stderr, "Error: %s.\n", fpl_error.msg);
            lexer_recover();
        }
        else {
            printf("%d\n", res);
        }

    } while (token.type != TOKEN_EOF);

clean:
    if (is_file) {
        fclose(file);
    }

    return EXIT_SUCCESS;
}

