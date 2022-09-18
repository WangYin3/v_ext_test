#include "../common/apis.h"
#include "../common/types.h"

static inline void validate_riscv_extensions() {
  s_validate_result_collection_t *collection = validate_extensions();

  print_result_collection(collection);

  dispose_result_collection(&collection);
}

int main() {
  validate_riscv_extensions();

  return 0;
}
