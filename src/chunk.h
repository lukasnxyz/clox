#ifndef clox_chunk_h
#define clox_chunk_h

#include "common.h"
#include "value.h"

typedef enum {
  OP_CONSTANT,
  OP_CONSTANT_LONG, // 24-bit

  OP_ADD,
  OP_SUBTRACT,
  OP_MULTIPLY,
  OP_DIVIDE,
  OP_NEGATE,

  OP_RETURN,
} Op_Code;

typedef struct {
  int line;
  int count;
} LineRLE;

// dynamic array
typedef struct {
  int count; // how many allocated entries are in use
  int capacity; // num of currently allocated entries
  uint8_t *code;
  ValueArray constants;

  LineRLE *lines;
  int line_count;
  int line_capacity;
} Chunk;

void init_chunk(Chunk *chunk);
void write_chunk(Chunk *chunk, uint8_t byte, int line);
void write_constant(Chunk *chunk, Value value, int line);
int add_constant(Chunk *chunk, Value value);
int get_line(Chunk *chunk, int instruction);
void free_chunk(Chunk *chunk);

#endif
