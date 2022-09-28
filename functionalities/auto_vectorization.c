#include <math.h>
#include <stdlib.h>

#include "../common/defines.h"
#include "../common/types.h"

static NO_INLINE void single_loop_int_vv_vectorization(uint64 *a, uint64 *b,
                                                       uint64 *c,
                                                       int loop_size) {
  FENCE();

  for (int i = 0; i < loop_size; i++) {
    c[i] = a[i] & b[i];
  }

  FENCE();
}

static NO_INLINE void single_loop_int_vi_vectorization(uint64 *a, uint64 *b,
                                                       int loop_size) {
  FENCE();

  for (int i = 0; i < loop_size; i++) {
    b[i] = a[i] + 1;
  }

  FENCE();
}

static NO_INLINE void single_loop_int_vx_vectorization(uint64 *a, uint64 *b,
                                                       int loop_size) {
  int x = rand() % loop_size;

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    b[i] = a[i] ^ (uint64)x;
  }

  FENCE();
}

static NO_INLINE void inner_product_int_loop_vectorization(uint64 *a, uint64 *b,
                                                           int loop_size) {
  uint64 inner_product = 0;

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    inner_product += a[i] * b[i];
  }

  FENCE();

  a[0] = inner_product;
}

static NO_INLINE void outer_product_int_loop_vectorization(uint64 *a, uint64 *b,
                                                           uint64 **c,
                                                           int loop_size) {
  FENCE();

  for (int i = 0; i < loop_size; i++) {
    for (int j = 0; j < loop_size; j++) {
      c[i][j] = a[i] * b[j];
    }
  }

  FENCE();
}

static NO_INLINE void single_loop_with_unknown_trip_count(double *a, double *b,
                                                          double c, int start,
                                                          int end) {
  FENCE();

  for (int i = start; i <= end; i++) {
    a[i] = a[i] * b[i] + c;
  }

  FENCE();
}

static NO_INLINE void single_loop_with_induction(uint64 *a, int loop_size) {
  FENCE();

  for (int i = 0; i < loop_size; i++) {
    a[i] = i;
  }

  FENCE();
}

static NO_INLINE void single_loop_with_if_condition(uint64 *a, uint64 *b,
                                                    int loop_size) {
  uint64 result = 0;

  FENCE();

  for (int i = 0; i < loop_size; i++) {
    if (a[i] > b[i]) {
      result += a[i] + 0xa;
    }
  }

  FENCE();

  a[0] = result;
}

static NO_INLINE void single_loop_with_function_call(double *a, double *b,
                                                     int loop_size) {
  FENCE();

  for (int i = 0; i < loop_size; i++) {
    b[i] = ceil(a[i]);
  }

  FENCE();
}

static NO_INLINE void single_loop_with_reverse_iterator(uint64 *a, uint64 *b,
                                                        int loop_size) {
  FENCE();

  for (int i = loop_size - 1; i >= 0; i--) {
    b[i] += a[i] & 0xa5;
  }

  FENCE();
}

static NO_INLINE void single_loop_with_scatter_and_gather(uint64 *a, uint64 *b,
                                                          int loop_size) {
  uint64 step = 4;

  FENCE();

  for (int i = 0; i < loop_size / 4; i++) {
    b[i] += a[i * step];
  }

  FENCE();
}

static NO_INLINE void single_loop_with_mixed_types(char *a, uint64 *b,
                                                   int loop_size) {
  FENCE();

  for (int i = 0; i < loop_size; i++) {
    b[i] += a[i] & 0xf1;
  }

  FENCE();
}

static NO_INLINE void single_loop_with_slp_vectorization(int *result, int a1,
                                                         int a2, int b1,
                                                         int b2) {
  int value = (a2 + b2) * (a1 + b1);
  FENCE();

  result[0] = value;
  result[1] = value;
  result[2] = value;
  result[3] = value;

  result[4] = value;
  result[5] = value;
  result[6] = value;
  result[7] = value;

  result[8] = value;
  result[9] = value;
  result[10] = value;
  result[11] = value;

  result[12] = value;
  result[13] = value;
  result[14] = value;
  result[15] = value;

  FENCE();
}

static inline void show_integer_auto_vectorization() {
  int loop_size = 4096;

  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);
  uint64 *c = malloc(sizeof(uint64) * loop_size);

  single_loop_int_vv_vectorization(a, b, c, loop_size);
  single_loop_int_vi_vectorization(a, b, loop_size);
  single_loop_int_vx_vectorization(a, b, loop_size);

  single_loop_with_induction(a, loop_size);
  single_loop_with_if_condition(a, b, loop_size);
  single_loop_with_reverse_iterator(a, b, loop_size);
  single_loop_with_scatter_and_gather(a, b, loop_size);
  single_loop_with_mixed_types((char *)a, b, loop_size);
  single_loop_with_slp_vectorization((int *)a, 1, 2, 4, 8);

  free(a);
  free(b);
  free(c);
}

static inline void show_float_point_auto_vectorization() {
  uint64 loop_size = 4096;
  double *a = malloc(sizeof(double) * loop_size);
  double *b = malloc(sizeof(double) * loop_size);
  double *c = malloc(sizeof(double) * loop_size);

  single_loop_with_unknown_trip_count(a, b, 1.3f, loop_size / 4, loop_size / 2);
  single_loop_with_function_call(a, b, loop_size);

  free(a);
  free(b);
  free(c);
}

static inline void show_int_product_auto_vectorization() {
  int loop_size = 4096;
  uint64 *a = malloc(sizeof(uint64) * loop_size);
  uint64 *b = malloc(sizeof(uint64) * loop_size);
  uint64 **c = malloc(sizeof(uint64 *) * loop_size);

  for (int i = 0; i < loop_size; i++) {
    c[i] = malloc(sizeof(uint64) * loop_size);
  }

  inner_product_int_loop_vectorization(a, b, loop_size);
  outer_product_int_loop_vectorization(a, b, c, loop_size);

  free(a);
  free(b);

  for (int i = 0; i < loop_size; i++) {
    free(c[i]);
  }

  free(c);
}

static inline uint64 **create_matrix(int r, int c) {
  uint64 **matrix = malloc(sizeof(uint64 *) * r);

  for (int i = 0; i < r; i++) {
    matrix[i] = malloc(sizeof(uint64) * c);
  }

  return matrix;
}

static inline void destory_matrix(uint64 **matrix, int r) {
  for (int i = 0; i < r; i++) {
    free(matrix[i]);
  }

  free(matrix);
}

static NO_INLINE void show_simple_int_gemm_auto_vectorization(uint64 **a,
                                                              uint64 **b,
                                                              uint64 **c, int m,
                                                              int k, int n) {
  FENCE();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      c[i][j] = 0;

      for (int l = 0; l < k; l++) {
        c[i][j] += a[i][l] * b[l][j];
      }
    }
  }

  FENCE();
}

static NO_INLINE uint64 **transform_matrix(uint64 **matrix, int k, int n) {
  uint64 **transform = create_matrix(n, k);

  for (int i = 0; i < k; i++) {
    for (int j = 0; j < n; j++) {
      transform[j][i] = matrix[i][j];
    }
  }

  return transform;
}

static NO_INLINE void show_simple_int_gemm_transformed_auto_vectorization(
    uint64 **a, uint64 **b, uint64 **c, int m, int k, int n) {
  uint64 **transform = transform_matrix(b, k, n);

  FENCE();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      c[i][j] = 0;

      for (int l = 0; l < k; l++) {
        c[i][j] += a[i][l] * transform[j][l];
      }
    }
  }

  destory_matrix(transform, n);

  FENCE();
}

static NO_INLINE void show_simple_float_point_gemm_auto_vectorization(
    double **a, double **b, double **c, int m, int k, int n) {
  FENCE();

  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      c[i][j] = 0;

      for (int l = 0; l < k; l++) {
        c[i][j] += a[i][l] * b[l][j];
      }
    }
  }

  FENCE();
}

static inline void show_gemm_auto_vectorization() {
  int m = 1024, k = 512, n = 4096; // mk * kn = mn
  uint64 **a = create_matrix(m, k);
  uint64 **b = create_matrix(k, n);
  uint64 **c = create_matrix(m, n);

  show_simple_int_gemm_auto_vectorization(a, b, c, m, k, n);
  show_simple_float_point_gemm_auto_vectorization((double **)a, (double **)b,
                                                  (double **)c, m, k, n);
  show_simple_int_gemm_transformed_auto_vectorization(a, b, c, m, k, n);

  destory_matrix(a, m);
  destory_matrix(b, k);
  destory_matrix(b, m);
}

static NO_INLINE void
show_simple_int_convolution_auto_vectorization(uint64 **a, int am, int an,
                                               uint64 **k, int km, int kn,
                                               uint64 **f, int bias) {
  for (int i = 0; i < am - km + 1; i++) {
    for (int j = 0; j < an - kn + 1; j++) {
      f[i][j] = bias;

      for (int x = 0; x < km; x++) {
        for (int y = 0; y < kn; y++) {
          f[i][j] += a[i + x][j + y] * k[x][y];
        }
      }
    }
  }
}

static inline void show_convolution_auto_vectorization() {
  int am = 1024, an = 2048, km = 64, kn = 64;
  int fm = am - km + 1, fn = an - kn + 1;
  uint64 **a = create_matrix(am, an);
  uint64 **k = create_matrix(km, kn);
  uint64 **f = create_matrix(fm, fn);

  show_simple_int_convolution_auto_vectorization(a, am, an, k, km, kn, f, 1);

  destory_matrix(a, am);
  destory_matrix(k, km);
  destory_matrix(k, fm);
}

void show_auto_vectorization() {
  show_integer_auto_vectorization();
  show_float_point_auto_vectorization();
  show_int_product_auto_vectorization();
  show_gemm_auto_vectorization();
  show_convolution_auto_vectorization();
}
