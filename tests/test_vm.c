#include <stdbool.h>

#include <stdio.h>
#include <stdlib.h>

#include <assert.h>
#include <setjmp.h>

#include "fpl.h"

// Pushes 2 and 3 onto the stack,
// adds them and checks whether the result is 5 or not.
void test_sum() {
    struct Vm vm;
    vm_init(&vm);
    
    vm_code_push(&vm, OP_PUSHINT);
    vm_code_push(&vm, 2);
    vm_code_push(&vm, OP_PUSHINT);
    vm_code_push(&vm, 3);
    vm_code_push(&vm, OP_ADD);
    vm_code_push(&vm, OP_HALT);
    vm_run(&vm);

    int val = vm_stack_peek(&vm);
    assert(val == 5);
}

// Ensures the jump is taken and the instruction is skipped,
// so 10 was not pushed into the stack.
void test_jmp() {
    struct Vm vm;
    vm_init(&vm);
    
    vm_code_push(&vm, OP_PUSHINT);
    vm_code_push(&vm, 0);

    vm_code_push(&vm, OP_JUMPZERO);
    vm_code_push(&vm, 6);

    vm_code_push(&vm, OP_PUSHINT);
    vm_code_push(&vm, 10);

    vm_code_push(&vm, OP_HALT);
    vm_run(&vm);

    assert(vm.sp == 0);
}

// Ensures the store instruction pops the value from the stack.
void test_storepops() {
    struct Vm vm;
    vm_init(&vm);

    vm_code_push(&vm, OP_PUSHINT);
    vm_code_push(&vm, 10);
    vm_code_push(&vm, OP_STORE);
    vm_code_push(&vm, 0);
    
    vm_run(&vm);

    assert(vm.sp == 0);
}

// Pushes 10 into the globals[0].
// Pushes 20 into the stack.
// Pushes globals[0] into the stack.
// Multiply them (10 and 20).
void test_load() {
    struct Vm vm;
    vm_init(&vm);

    vm_code_push(&vm, OP_PUSHINT);
    vm_code_push(&vm, 10);
    vm_code_push(&vm, OP_STORE);
    vm_code_push(&vm, 0);

    vm_code_push(&vm, OP_PUSHINT);
    vm_code_push(&vm, 20);
    vm_code_push(&vm, OP_LOAD);
    vm_code_push(&vm, 0);
    vm_code_push(&vm, OP_MUL);
    
    vm_run(&vm);

    int val = vm_stack_peek(&vm);
    assert(val == 200);
}

int main(int argc, char **argv) {
    test_sum();
    test_jmp();
    test_storepops();
    test_load();

    return EXIT_SUCCESS;
}

