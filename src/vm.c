#include <stdlib.h>

#include "fpl.h"

void vm_init(struct Vm *vm) {
    vm->ip = 0;
    vm->cp = 0;
    vm->sp = 0;

    for (int i = 0; i < CODE_MAX; i++) {
        vm->code[i] = 0;
    }

    for (int i = 0; i < STACK_MAX; i++) {
        vm->stack[i] = 0;
    }
}

void vm_code_push(struct Vm *vm, int code) {
    if (vm->cp >= CODE_MAX) {
        fprintf(stderr, "Error: Code overflow!");
        exit(1);
    }
    vm->code[vm->cp++] = code;
}

int vm_code_pop(struct Vm *vm) {
    if (vm->cp <= 0) {
        fprintf(stderr, "Error: Code underflow!");
        exit(1);
    }
    return vm->code[--vm->cp];
}

void vm_stack_push(struct Vm *vm, int data) {
    if (vm->sp >= STACK_MAX) {
        fprintf(stderr, "Error: Stack overflow!");
        exit(1);
    }
    vm->stack[vm->sp++] = data;
}

int vm_stack_pop(struct Vm *vm) {
    if (vm->sp <= 0) {
        fprintf(stderr, "Error: Stack underflow!");
        exit(1);
    } 
    return vm->stack[--vm->sp];
}

int vm_stack_peek(struct Vm *vm) {
    if (vm->sp <= 0 || vm->sp >= STACK_MAX) {
        fprintf(stderr, "Error: Stack pointer exceeds its limits.");
        exit(1);
    }
    return vm->stack[vm->sp - 1];
}

static inline void vm_step(struct Vm *vm) {
    if (vm->ip < 0 || vm->ip >= vm->cp) {
        return;
    }

    int inst = vm->code[vm->ip++];

    switch (inst) {
    case OP_PUSHINT: {
        int val = vm->code[vm->ip++];
        vm_stack_push(vm, val);
        break;
    }
    case OP_MUL: {
        int b = vm_stack_pop(vm);
        int a = vm_stack_pop(vm);
        vm_stack_push(vm, a * b);
        break;
    }
    case OP_ADD: {
        int b = vm_stack_pop(vm);
        int a = vm_stack_pop(vm);
        vm_stack_push(vm, a + b);
        break;
    }
    case OP_SUB: {
        int b = vm_stack_pop(vm);
        int a = vm_stack_pop(vm);
        vm_stack_push(vm, a - b);
        break;
    }
    case OP_PRINT: {
        int val = vm_stack_pop(vm);
        printf("VM=>%d\n", val);
        break;
    }
    case OP_JUMP: {
        int target = vm->code[vm->ip++];
        vm->ip = target;
        break;
    }
    case OP_JUMPZERO: {
        int target = vm->code[vm->ip++];
        int flag = vm_stack_pop(vm); 
        if (!flag) {
            vm->ip = target;
        }
        break;
    }
    case OP_HALT: {
        return;
        break;
    }
    case OP_STORE: {
        int index = vm->code[vm->ip++];
        int val = vm_stack_pop(vm);

        if (index < 0 || index >= GLOBALS_MAX) {
            fprintf(stderr, "Error: Global variable index out of bounds.\n");
            exit(1);
        }

        vm->globals[index] = val;

        break;
    }
    case OP_LOAD: {
        int index = vm->code[vm->ip++];
        
        if (index < 0 || index >= GLOBALS_MAX) {
            fprintf(stderr, "Error: Global variable index out of bounds.\n");
            exit(1);
        }
        
        int val = vm->globals[index];
        vm_stack_push(vm, val);

        break;
    }
    case OP_DUP: {
        int curr = vm_stack_peek(vm);
        vm_stack_push(vm, curr);
        break;
    }
    default: {
        fprintf(stderr, "Unrecognized instruction: %d\n", inst);
        exit(1);
        break;
    }
    }
}

void vm_run(struct Vm *vm) {
    while ((vm->ip >= 0) && (vm->ip < vm->cp)) {
        vm_step(vm);
    }
}


