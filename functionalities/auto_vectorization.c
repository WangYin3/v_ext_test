#include <stdlib.h>

#include "../common/defines.h"
#include "../common/types.h"

static NO_INLINE uint64 single_loop_int_vv_vectorization() {
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

static NO_INLINE uint64 single_loop_int_vi_vectorization() {
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

static NO_INLINE uint64 single_loop_int_vx_vectorization() {
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

static NO_INLINE uint64 inner_product_int_loop_vectorization() {
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

static NO_INLINE uint64 outer_product_int_loop_vectorization() {
  int loop_size = 1024;
  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);
  uint64 **c = malloc(sizeof(uint64 *) * loop_size);

  for (int i = 0; i < loop_size; i++) {
    c[i] = malloc(sizeof(uint64) * loop_size);
  }

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    for (int j = 0; j < loop_size; j++) {
      c[i][j] = a[i] * b[j];
    }
  }

  FENCE();

  uint64 result = c[rand() % loop_size][rand() % loop_size];

  free(a);
  free(b);
  free(c);

  return result;
}

static NO_INLINE uint64 level_2_loop_int_vectorization() {
  int loop_size = 1024;
  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);
  uint64 *c = malloc(sizeof(uint64) * loop_size);
  uint64 **d = malloc(sizeof(uint64 *) * loop_size);

  for (int i = 0; i < loop_size; i++) {
    d[i] = malloc(sizeof(uint64) * loop_size);
  }

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    uint64 tmp = 0xff;

    for (int k = 0; k < loop_size; k++) {
      tmp &= a[k] * b[k];
    }

    for (int j = 0; j < loop_size; j++) {
      d[i][j] = (a[j] | b[j]) + tmp;
    }
  }

  FENCE();

  uint64 result = d[rand() % loop_size][rand() % loop_size];

  free(a);
  free(b);
  free(c);
  free(d);

  return result;
}

uint64 show_auto_vectorization() {
  uint64 result = 0;

  result += single_loop_int_vv_vectorization();
  result += single_loop_int_vi_vectorization();
  result += single_loop_int_vx_vectorization();

  result += inner_product_int_loop_vectorization();
  result += outer_product_int_loop_vectorization();

  result += level_2_loop_int_vectorization();

  return result;
}
