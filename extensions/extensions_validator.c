#include <stdlib.h>

#include "../common/apis.h"
#include "../common/defines.h"
#include "../common/types.h"

static inline s_validate_result_t validate_I_extension() {
  uint64 val;
  s_validate_result_t result = {
      true,
      "I",
      "Extension Integer is Supported",
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

s_validate_result_collection_t *validate_extensions() {
  s_validate_result_collection_t *collection =
      create_result_collection(EXTENSIONS_MAX_CAPACITY);

  append_one_result(collection, validate_I_extension());

  return collection;
}
