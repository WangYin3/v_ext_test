#include <stdlib.h>

#include "../common/defines.h"
#include "../common/types.h"

static NO_INLINE uint64 single_loop_vv_vectorization() {
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

static NO_INLINE uint64 single_loop_vi_vectorization() {
  int loop_size = 4096;
  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    b[i] = a[i] + 1;
  }

  FENCE();

  uint64 result = b[rand() % loop_size];

  free(a);
  free(b);

  return result;
}

static NO_INLINE uint64 single_loop_vx_vectorization() {
  int loop_size = 4096;
  int x = rand() % loop_size;
  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    b[i] = a[i] ^ (uint64)x;
  }

  FENCE();

  uint64 result = b[x];

  free(a);
  free(b);

  return result;
}

static NO_INLINE uint64 inner_product_loop_vectorization() {
  int loop_size = 1024;
  uint64 inner_product = 0;
  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    inner_product += a[i] * b[i];
  }

  FENCE();

  free(a);
  free(b);

  return inner_product;
}

uint64 show_auto_vectorization() {
  uint64 result = 0;

  result += single_loop_vv_vectorization();
  result += single_loop_vi_vectorization();
  result += single_loop_vx_vectorization();

  result += inner_product_loop_vectorization();

  return result;
}
