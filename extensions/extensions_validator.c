#include <stdlib.h>

#include "../common/apis.h"
#include "../common/defines.h"
#include "../common/types.h"

static inline s_validate_result_t validate_I_extension() {
  uint64 val;
  s_validate_result_t result = {
      true,
      "I",
      "Extension I is Supported",
  };

  asm volatile("lui        t0, 0xff\n\t"
               "auipc      t1, 0x11\n\t"
               "andi       t2, t0, 0xaf\n\t"
               "andi       t2, t0, 0xaf\n\t"
               "xori       t3, t1, 0x7d\n\t"
               "add        t4, t2, t0\n\t"
               "sub        t5, t3, t1\n\t"
               "sll        t6, t4, t5\n\t"
               "sd         t6, %[dest]\n\t"
               "fence"
               : [dest] "=m"(val)
               :
               :);

  return result;
}

static inline s_validate_result_t validate_M_extension() {
  uint64 val;
  s_validate_result_t result = {
      true,
      "M",
      "Extension M is Supported",
  };

  asm volatile("lui        t0, 0xff\n\t"
               "lui        t1, 0x11\n\t"
               "mul        t2, t1, t0\n\t"
               "lui        t3, 0x2e\n\t"
               "rem        t4, t3, t0\n\t"
               "div        t5, t4, t1\n\t"
               "sd         t5, %[dest]\n\t"
               "fence"
               : [dest] "=m"(val)
               :
               :);

  return result;
}

static inline s_validate_result_t validate_F_extension() {
  float a = 41.2f, b = 92.24f, c;
  s_validate_result_t result = {
      true,
      "F",
      "Extension F is Supported",
  };

  asm volatile("flw        f0, %[src1]\n\t"
               "flw        f1, %[src2]\n\t"
               "fsub.s     f2, f1, f0\n\t"
               "fadd.s     f3, f1, f0\n\t"
               "fsqrt.s    f4, f3\n\t"
               "fmax.s     f5, f4, f2\n\t"
               "fsw        f5, %[dest]\n\t"
               "fence"
               : [dest] "=m"(c)
               : [src1] "m"(a), [src2] "m"(b)
               :);

  return result;
}

static inline s_validate_result_t validate_D_extension() {
  double a = 41.2f, b = 92.24f, c;
  s_validate_result_t result = {
      true,
      "D",
      "Extension D is Supported",
  };

  asm volatile("fld        f0, %[src1]\n\t"
               "fld        f1, %[src2]\n\t"
               "fsub.d     f2, f1, f0\n\t"
               "fadd.d     f3, f1, f0\n\t"
               "fsqrt.d    f4, f3\n\t"
               "fmax.d     f5, f4, f2\n\t"
               "fsd        f5, %[dest]\n\t"
               "fence"
               : [dest] "=m"(c)
               : [src1] "m"(a), [src2] "m"(b)
               :);

  return result;
}

s_validate_result_collection_t *validate_extensions() {
  s_validate_result_collection_t *collection =
      create_result_collection(EXTENSIONS_MAX_CAPACITY);

  append_one_result(collection, validate_I_extension());
  append_one_result(collection, validate_M_extension());
  append_one_result(collection, validate_F_extension());
  append_one_result(collection, validate_D_extension());

  return collection;
}
