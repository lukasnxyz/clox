#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"

int main(int argc, const char *argv[] ) {
  printf("clox\n");

  Chunk chunk;
  init_chunk(&chunk);

  for (int i = 0; i < 280; ++i)
    write_constant(&chunk, (double)i, 123);

  write_chunk(&chunk, OP_RETURN, 123);

  disassemble_chunk(&chunk, "test chunk");
  free_chunk(&chunk);

  return 0;
}
