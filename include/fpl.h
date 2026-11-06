#ifndef FPL_H
#define FPL_H

#include <stdbool.h>
#include <setjmp.h>

#include <stdio.h>

/* Lexer Parser */
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
    TOKEN_EQ,
};

struct Token {
    enum TokenType type;
    int val;
    char name;
};

extern struct Token token;

#define SYMBOLS_SIZE 32
extern int symbols[SYMBOLS_SIZE];

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

/* Error */
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

extern jmp_buf jmpbuf;

/* Virtual Machine */
#define STACK_MAX   256
#define CODE_MAX    1024
#define GLOBALS_MAX 256

enum OpCode {
    OP_PUSHINT,
    OP_MUL,
    OP_ADD,
    OP_SUB,
    OP_PRINT,
    OP_JUMP,
    OP_JUMPZERO,
    OP_HALT,
    OP_STORE,
    OP_LOAD,
    OP_DUP,
};
struct Vm {
    int code[CODE_MAX];
    int cp;
    int ip;

    int stack[STACK_MAX];
    int sp;

    int globals[GLOBALS_MAX];
};

void vm_init(struct Vm*);
void vm_code_push(struct Vm *vm, int code);
int vm_code_pop(struct Vm *vm);
void vm_stack_push(struct Vm *vm, int data);
int vm_stack_pop(struct Vm *vm);
int vm_stack_peek(struct Vm *vm);
void vm_run(struct Vm *vm);


#endif

