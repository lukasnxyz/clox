#include <stdio.h>

#include "debug.h"
#include "value.h"

void disassemble_chunk(Chunk *chunk, const char* name) {
  printf("== %s ==\n", name);

  for (int offset = 0; offset < chunk->count;)
    offset = disassemble_instruction(chunk, offset);
}

// (static = basically private func)
static int simple_instruction(const char *name, int offset) {
  printf("%s\n", name);
  return offset + 1;
}

static int constant_instruction(const char *name, Chunk *chunk, int offset) {
  uint8_t constant = chunk->code[offset + 1];
  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");
  return offset + 2;
}

static int constant_long_instruction(const char *name, Chunk *chunk, int offset) {
  uint8_t byte0 = chunk->code[offset + 1];
  uint8_t byte1 = chunk->code[offset + 2];
  uint8_t byte2 = chunk->code[offset + 3];
  uint32_t constant = byte0 | (byte1 << 8) | (byte2 << 16);

  printf("%-16s %4d '", name, constant);
  print_value(chunk->constants.values[constant]);
  printf("'\n");

  return offset + 4;
}

int disassemble_instruction(Chunk *chunk, int offset) {
  printf("%04d ", offset);
  if (offset > 0 && chunk->lines[offset].line == chunk->lines[offset - 1].line)
    printf("   | ");
  else
    printf("%4d ", chunk->lines[offset].line);

  uint8_t instruction = chunk->code[offset];
  switch (instruction) {
    case OP_CONSTANT:
      return constant_instruction("OP_CONSTANT", chunk, offset);
    case OP_CONSTANT_LONG:
      return constant_long_instruction("OP_CONSTANT_LONG", chunk, offset);
    case OP_RETURN:
      return simple_instruction("OP_RETURN", offset);
    default:
      printf("Unknown opcode %d\n", instruction);
      return offset + 1;
  }
}
