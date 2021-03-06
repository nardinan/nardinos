#ifndef STANDARD_LIBRARY_STRING
#define STANDARD_LIBRARY_STRING
#include "memory.h"
#include "variable_arguments.h"
#include "../../drivers/video/raw_video.h"
/* This identifies the default precision, used to represent a decimal number 
 * when no precision is specified in the parameter of the %f placeholder */
#define STRING_DOUBLE_DEFAULT_PRECISION 10
extern size_t string_length(const char *buffer);
extern int string_compare(const char *buffer, const char *other_buffer);
extern void string_copy(char *destination, const char *source);
extern size_t string_from_int(char *destination, int value);
extern size_t string_from_double(char *destination, double value, int precision);
extern size_t string_hex_from_void(char *destination, pointer_t value, size_t size);
extern size_t string_create_stack_pointer(char *destination, const char *format, unsigned char *argument_stack_pointer);
extern size_t string_create(char *destination, const char *format, ...)
  __attribute__ ((format (printf, 2, 3)));
extern void kernel_printf(const char *format, ...)
  __attribute__ ((format (printf, 1, 2)));
#endif
