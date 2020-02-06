#ifndef STANDARD_LIBRARY_STRING
#define STANDARD_LIBRARY_STRING
#include "memory.h"
extern size_t string_length(const char *buffer);
extern int string_compare(const char *buffer, const char *other_buffer);
extern void string_copy(char *destination, const char *source);
extern size_t string_create(char *destination, const char *format, ...);
#endif
