#include <stdio.h>
#include <stdlib.h>

#include "chunk.h"

int test_3_byte_constant() {
  Chunk chunk;
  init_chunk(&chunk);

  for (int i = 0; i < 258; ++i) write_constant(&chunk, (double)i, 123);

  uint8_t byte0 = chunk.code[chunk.count - 3];
  uint8_t byte1 = chunk.code[chunk.count - 2];
  uint8_t byte2 = chunk.code[chunk.count - 1];
  uint32_t constant = byte0 | (byte1 << 8) | (byte2 << 16);

  free_chunk(&chunk);

  printf("constant: %d\n", constant);
  if (constant != 257) return 1;
  return 0;
}

int main(void) {
  int failed = 0;
  failed += test_3_byte_constant();

  if (failed == 0) {
    printf("All tests passed.\n");
    return EXIT_SUCCESS;
  } else {
    printf("%d test(s) failed.\n", failed);
    return EXIT_FAILURE;
  }
}
