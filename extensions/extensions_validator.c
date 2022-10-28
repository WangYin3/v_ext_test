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

static inline s_validate_result_t validate_A_extension() {
  uint64 a = 123, b = 455, c;
  s_validate_result_t result = {
      true,
      "A",
      "Extension A is Supported",
  };

  asm volatile("addi       t0, %[src1], 0x0\n\t"
               "addi       t1, %[src2], 0x0\n\t"
               "lr.d       t2, (t0)\n\t"
               "lr.d       t3, (t1)\n\t"
               "amoadd.d   t3, t2, (t0)\n\t"
               "amoxor.d   t4, t3, (t1)\n\t"
               "amoand.d   t5, t3, (t0)\n\t"
               "sc.d       t4, t3, (t1)\n\t"
               "sc.d       t5, t4, (t0)\n\t"
               "sd         t5, %[dest]\n\t"
               "fence"
               : [dest] "=m"(c)
               : [src1] "r"(&a), [src2] "r"(&b)
               :);

  return result;
}

static inline s_validate_result_t validate_C_extension() {
  uint64 val;
  s_validate_result_t result = {
      true,
      "C",
      "Extension C is Supported",
  };

  asm volatile("c.lui      a0, 0xf\n\t"
               "auipc      a1, 0x11\n\t"
               "c.mv       a2, t0\n\t"
               "c.andi     a3, 0xa\n\t"
               "c.xor      a3, a1\n\t"
               "c.add      a2, a0\n\t"
               "c.sub      a3, a2\n\t"
               "c.slli     a3, 0x3\n\t"
               "c.sd       a3, %[dest]\n\t"
               "fence"
               : [dest] "=m"(val)
               :
               : "a0", "a1", "a2", "a3");

  return result;
}

static inline s_validate_result_t validate_V_extension() {
  uint64 a[4], b[4], c[4];
  s_validate_result_t result = {
      true,
      "V",
      "Extension V is Supported",
  };

  asm volatile("vsetvli    t1, t0, e64, m1, ta, ma\n\t"
               "vle64.v    v1, (%[src1])\n\t"
               "vle64.v    v2, (%[src2])\n\t"
               "vadd.vv    v3, v2, v1\n\t"
               "vsub.vv    v4, v2, v1\n\t"
               "vxor.vv    v5, v3, v4\n\t"
               "vsll.vv    v6, v3, v4\n\t"
               "vand.vv    v7, v6, v5\n\t"
               "vse64.v    v7, (%[src3])\n\t"
               "fence"
               :
               : [src1] "r"(a), [src2] "r"(b), [src3] "r"(c)
               :);

  return result;
}

static inline s_validate_result_t validate_Zifencei_extension() {
  s_validate_result_t result = {
      true,
      "Zifencei",
      "Extension Zifencei is Supported",
  };

  asm volatile("fence.i\n\t"
               "fence"
               :
               :
               :);

  return result;
}

static inline s_validate_result_t validate_Zicsr_extension() {
  uint64 csr;
  s_validate_result_t result = {
      true,
      "Zicsr",
      "Extension Zicsr is Supported",
  };

  asm volatile("csrrs t0,  fcsr, x0\n\t"
               "csrrw t1,  fcsr, t0\n\t"
               "sd    t1,  %[dest]\n\t"
               "fence"
               : [dest] "=m"(csr)
               :
               :);

  return result;
}

/*
static inline s_validate_result_t validate_P_extension() {
  uint64 a = 123, b = 455, c;
  s_validate_result_t result = {
      true,
      "P",
      "Extension P is Supported",
  };

  asm volatile("ld    t0,  %[src1]\n\t"
               "ld    t1,  %[src2]\n\t"
               "add16 t3,  t0, t1\n\t"
               // "sub16 t4,  t0, t1\n\t"
               // "sra16 t5,  t3, t4\n\t"
               "sd    t5,  %[dest]\n\t"
               "fence"
               : [dest] "=m"(c)
               : [src1] "m"(a), [src2] "m"(b)
               :);

  return result;
}
*/

static inline s_validate_result_t validate_B_extension() {
  uint64 val;
  s_validate_result_t result = {
      true,
      "B",
      "Extension B is Supported",
  };

  asm volatile("lui        t0, 0xff1e\n\t"
               "lui        t1, 0x774\n\t"
               "clz        t2, t0\n\t"
               "clz        t3, t1\n\t"
               "min        t4, t2, t3\n\t"
               "max        t5, t2, t3\n\t"
               "xnor       t6, t5, t5\n\t"
               "sd         t6, %[dest]\n\t"
               "fence"
               : [dest] "=m"(val)
               :
               :);

  return result;
}

s_validate_result_collection_t *validate_extensions() {
  s_validate_result_collection_t *collection =
      create_result_collection(EXTENSIONS_MAX_CAPACITY);

  // append_one_result(collection, validate_I_extension());
  // append_one_result(collection, validate_M_extension());
  // append_one_result(collection, validate_F_extension());
  // append_one_result(collection, validate_D_extension());
  // append_one_result(collection, validate_C_extension());
  // append_one_result(collection, validate_A_extension());
  append_one_result(collection, validate_V_extension());
  // append_one_result(collection, validate_Zifencei_extension());
  // append_one_result(collection, validate_Zicsr_extension());
  // append_one_result(collection, validate_P_extension());
  // append_one_result(collection, validate_B_extension());

  return collection;
}
