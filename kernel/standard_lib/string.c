#include "string.h"
size_t string_length(const char *buffer) {
  const char *entry_point = buffer;
  while (*buffer != 0)
    ++buffer;
  return (size_t)(buffer-entry_point);
}
int string_compare(const char *buffer, const char *other_buffer) {
  while ((*buffer == *other_buffer) &&
      (*buffer != 0)) {
    ++buffer;
    ++other_buffer;
  }
  return (*other_buffer - *buffer);
}
void string_copy(char *destination, const char *source) {
  while (*source != 0) {
    *destination = *source;
    ++destination;
    ++source;
  }
  *destination = 0;
}
/* I have to be honest, I've peeked into the stdarg function because seems that this
 * is the amount of size each argument takes into the stack. I am surprised as well
 * because I had no idea. Now I know.
 */
#define argument_size_in_stack(type)\
  (((sizeof(type)+sizeof(int)-1)/sizeof(int))*sizeof(int))
/* we need to collect the current element and we need to increase the pointer in
 * order to be able to access the next element at the next call */
#define string_next_argument(valist,type)\
  (((valist) = (valist)+argument_size_in_stack(type)), *((type *)((valist)-argument_size_in_stack(type))))
size_t string_create(char *destination, const char *format, ...) {
  size_t result = 0;
  unsigned char *argument_pointer = __builtin_next_arg(format);
  while (*format != 0) {
    if (*format == '%') {
      int move_next_argument = 1, payload_int, entries = 0;
      char payload_character, *payload_string;
      switch (*(format + 1)) {
        case 'c':
          payload_character = (char)string_next_argument(argument_pointer, int);
          if (destination) {
            *destination = payload_character;
            ++destination;
          }
          ++result;
          break;
        case '%':
          if (destination) {
            *destination = *format;
            ++destination;
          }
          ++result;
          move_next_argument = 0;
          break;
        case 's':
          payload_string = string_next_argument(argument_pointer, char *);
          while (*payload_string != 0) {
            if (destination) {
              *destination = *payload_string;
              ++destination;
            }
            ++payload_string;
            ++result;
          }
          break;
        case 'd':
          payload_int = string_next_argument(argument_pointer, int);
          while (payload_int > 0) {
            int operand = payload_int % 10;
            payload_int = (payload_int - operand) / 10;
            if (destination) {
              if (entries > 0)
                memory_move((destination + 1), destination, entries);
              *destination = ('0' + operand);
            }
            ++entries;
            ++result;
          }
          if (destination)
            destination += entries;
          break;
        default:
          break;
      }
      /* double jump to skip % and the code */
      ++format;
      ++format;
    } else {
      if (destination) {
        *destination = *format;
        ++destination;
      }
      ++format;
      ++result;
    }
  }
  if (destination)
    *destination = 0;
  return result;
}

