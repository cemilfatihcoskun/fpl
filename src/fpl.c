#include "fpl.h"

struct FPL_Error fpl_error;

int symbols[SYMBOLS_SIZE];
struct Token token;

jmp_buf jmpbuf;

