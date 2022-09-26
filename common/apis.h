#ifndef API_H
#define API_H

#include "defines.h"
#include "types.h"

extern s_validate_result_collection_t *
create_result_collection(uint32 capacity);
extern void
dispose_result_collection(s_validate_result_collection_t **collection);
extern void append_one_result(s_validate_result_collection_t *collection,
                              s_validate_result_t result);
extern void print_result_collection(s_validate_result_collection_t *collection);

extern s_validate_result_collection_t *validate_extensions();

extern void show_auto_vectorization();

#endif
