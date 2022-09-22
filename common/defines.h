#ifndef DEFINES_H
#define DEFINES_H

#define true 1
#define false 0

#define EXTENSIONS_MAX_CAPACITY 512
#define TO_BOOL_STRING(b) ((b) ? "true" : "false")
#define FENCE() asm volatile("fence" : :);
#define NO_INLINE __attribute__((noinline))

#endif
