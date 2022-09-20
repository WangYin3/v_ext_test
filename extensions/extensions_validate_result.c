#include <stdio.h>
#include <stdlib.h>

#include "../common/defines.h"
#include "../common/types.h"

s_validate_result_collection_t *create_result_collection(uint32 capacity) {
  if (capacity == 0) {
    capacity = EXTENSIONS_MAX_CAPACITY;
  }

  s_validate_result_collection_t *collection = malloc(sizeof(*collection));

  collection->capacity = capacity;
  collection->count = 0;
  collection->results = malloc(sizeof(*collection->results) * capacity);

  return collection;
}

void dispose_result_collection(s_validate_result_collection_t **collection) {
  if (collection == NULL || *collection == NULL) {
    return;
  }

  free((*collection)->results);
  free(*collection);

  *collection = NULL;
}

void append_one_result(s_validate_result_collection_t *collection,
                       s_validate_result_t result) {
  if (collection->count >= collection->capacity) {
    return;
  }

  collection->results[collection->count++] = result;
}

void print_result_collection(s_validate_result_collection_t *collection) {
  if (collection == NULL) {
    return;
  }

  printf("+-----------+-----------+-------------------------------------+\n");
  printf("| Extension | Supported | Description                         |\n");
  printf("+-----------+-----------+-------------------------------------+\n");

  for (uint32 i = 0; i < collection->count; i++) {
    s_validate_result_t result = collection->results[i];
    printf("| %-9s | %-9s | %-35s |\n", result.extension_name,
           TO_BOOL_STRING(result.is_supported), result.description);
    printf("+-----------+-----------+-------------------------------------+\n");
  }
}
