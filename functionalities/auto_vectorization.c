#include <stdlib.h>

#include "../common/defines.h"
#include "../common/types.h"

static inline uint64 inner_loop_nn_vectorization() {
  int loop_size = 4096;
  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);
  uint64 *c = malloc(sizeof(uint64) * loop_size);

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    c[i] = a[i] & b[i];
  }

  FENCE();

  uint64 result = c[rand() % loop_size];

  free(a);
  free(b);
  free(c);

  return result;
}

uint64 show_auto_vectorization() {
  uint64 result = 0;

  result += inner_loop_nn_vectorization();

  return result;
}
