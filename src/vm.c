#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "value.h"
#include "vm.h"

VM vm;

static void reset_stack() {
  vm.stack_top = vm.stack;
}

void init_vm() {
  reset_stack();
}

void free_vm() {
}

static InterpretResult run() {
#define READ_BYTE() (*vm.ip++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

  for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
    disassemble_instruction(vm.chunk, (int)(vm.ip - vm.chunk->code));
    for (Value *slot = vm.stack; slot < vm.stack_top; ++slot) {
      printf("[ ");
      print_value(*slot);
      printf(" ]");
    }
    printf("\n");
#endif
    uint8_t instruction;
    switch (instruction = READ_BYTE()) {
      case OP_CONSTANT: {
                          Value constant = READ_CONSTANT();
                          push(constant);
                          break;
                        }
      case OP_RETURN: {
                        print_value(pop());
                        printf("\n");
                        return INTERPRET_OK;
                      }
    }
  }
#undef READ_BYTE
#undef READ_CONSTANT
}

InterpretResult interpret(Chunk *chunk) {
  vm.chunk = chunk;
  vm.ip = vm.chunk->code;
  return run();
}

void push(Value value) {
  *vm.stack_top = value;
  vm.stack_top++;
}

Value pop() {
  // don't need to explicitly "remove" the element from the array
  vm.stack_top--;
  return *vm.stack_top;
}
