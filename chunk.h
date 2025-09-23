#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_RETURN,
} Op_Code;

// dynamic array
typedef struct {
  int count; // how many allocated entries are in use
  int capacity; // num of currently allocated entries
  uint8_t *code;
  ValueArray constants;
  int *lines;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte);
int add_constant(Chunk *chunk, Value value);
void free_chunk(Chunk *chunk);

#endif
