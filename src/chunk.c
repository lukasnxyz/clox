#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void init_chunk(Chunk *chunk) {
  chunk->count = 0;
  chunk->capacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  chunk->line_count = 0;
  chunk->line_capacity = 0;
  init_value_array(&chunk->constants);
}

void write_chunk(Chunk *chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) {
    int old_capacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(old_capacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, old_capacity, chunk->capacity);
  }

  chunk->code[chunk->count] = byte;

  if (chunk->line_count > 0 && chunk->lines[chunk->line_count - 1].line == line) {
    // same line as previous instruction
    chunk->lines[chunk->line_count - 1].count++;
  } else {
    if (chunk->line_capacity < chunk->line_count + 1) {
      int old_line_capacity = chunk->line_capacity;
      chunk->line_capacity = GROW_CAPACITY(old_line_capacity);
      chunk->lines = GROW_ARRAY(LineRLE, chunk->lines, old_line_capacity, chunk->line_capacity);
    }

    chunk->lines[chunk->line_count].line = line;
    chunk->lines[chunk->line_count].count = 1;
    chunk->line_count++;
  }

  chunk->count++;
}

/*
bit masking tutorial:
  Original number: 0x123456
  Binary:          00010010 00110100 01010110
                      ^        ^        ^
                    byte2    byte1    byte0

  Mask 0xFF:       00000000 00000000 11111111
  Result:          00000000 00000000 01010110 = 0x56
*/
void write_constant(Chunk *chunk, Value value, int line) {
  int c_index = add_constant(chunk, value);
  if (c_index < 256) {
    write_chunk(chunk, OP_CONSTANT, line);
    write_chunk(chunk, (uint8_t)c_index, line);
  } else {
    write_chunk(chunk, OP_CONSTANT_LONG, line);

    write_chunk(chunk, (uint8_t)(c_index & 0xFF), line); // LSB (right most)
    write_chunk(chunk, (uint8_t)((c_index >> 8) & 0xFF), line); // middle byte
    write_chunk(chunk, (uint8_t)((c_index >> 16) & 0xFF), line); // MSB
  }
}

int add_constant(Chunk *chunk, Value value) {
  write_value_array(&chunk->constants, value);
  return chunk->constants.count - 1;
}

int get_line(Chunk *chunk, int instruction) {
  int curr_instruction = 0;

  for (int i = 0; i < chunk->line_count; i++) {
    curr_instruction += chunk->lines[i].count;
    if (instruction < curr_instruction)
      return chunk->lines[i].line;
  }

  return -1;
}

void free_chunk(Chunk *chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(LineRLE, chunk->lines, chunk->line_capacity);
  free_value_array(&chunk->constants);
  init_chunk(chunk);
}
