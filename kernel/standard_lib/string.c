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
size_t string_from_int(char *destination, int value) {
  size_t result = 0;
  int was_negative = ((value < 0)?1:0);
  if (was_negative) 
    value *= -1.0;  /* we want to use the absolute value
                     * and keep track of the sign of the 
                     * value with the was_negative flag 
                     */
  if (value > 0) {
    while (value > 0) {
      int digit = (value % 10);
      value = (value - digit) / 10;
      if (destination) {
        if (result > 0)
          memory_move((unsigned char *)(destination + 1), 
              (unsigned char *)destination, result);
        *destination = ('0' + digit);
      }
      ++result;
    }
  } else {
    if (destination)
      *destination = '0';
    ++result;
  }
  /* now, in case the sign of the argument was zero, we need to restore it */
  if (was_negative) {
    if (destination) {
      if (result > 0)
        memory_move((unsigned char *)(destination + 1),
            (unsigned char *)destination, result);
      *destination = '-';
    }
    ++result;
  }
  return result;
}
size_t string_from_double(char *destination, double value, int precision) {
  size_t result = 0;
  int decimal_part = value, normalized_fractional_part = 0;
  double fractional_part = ((value - (double)decimal_part) * 
      ((decimal_part < 0)?-1:1)); 
  result = string_from_int(destination, decimal_part);
  if (destination) {
    destination += result;
    *destination = '.';
    ++destination;
  }
  ++result;
  while ((fractional_part > 0) && ((precision < 0) || (precision > 0))) {
    fractional_part *= 10.0;
    if (destination) {
      *destination = ('0' + ((int)fractional_part));
      ++destination;
    }
    --precision;
    ++result;
    fractional_part -= ((int)fractional_part);
  }
  return result;
}
size_t string_hex_from_void(char *destination, pointer_t value, size_t size) {
  size_t result = 0;
  int nibbles = (size * 2);
  while (nibbles > 0) {
    unsigned char nibble = ((value>>((nibbles - 1) * 4))&0xf);
    if (destination) {
      if (nibble > 9)
        *destination = ('A' + (nibble - 10));
      else
        *destination = ('0' + nibble);
      ++destination;
    }
    --nibbles;
    ++result;
  }
  return result;
}
size_t string_create_stack_pointer(char *destination, const char *format, unsigned char *argument_stack_pointer) {
  size_t result = 0;
  unsigned int double_precision = STRING_DOUBLE_DEFAULT_PRECISION;
  while (*format != 0) {
    if (*format == '%') {
      int new_precision_ongoing = 0, continue_loop;
      do {
        int payload_int, entries = 0;
        char payload_character, *payload_string;
        double payload_double;
        pointer_t payload_pointer;
        continue_loop = 0;
        switch (*(format + 1)) {
          case 'c':
          case 'C':
            payload_character = (char)grab_next_argument(argument_stack_pointer, int);
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
            break;
          case 's':
          case 'S':
            payload_string = (char *)grab_next_argument(argument_stack_pointer, void *);
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
          case 'D':
            payload_int = grab_next_argument(argument_stack_pointer, int);
            entries = string_from_int(destination, payload_int);
            if (destination)
              destination += entries;
            result += entries;
            break;
          case 'f':
          case 'F':
            payload_double = grab_next_argument(argument_stack_pointer, double);
            entries = string_from_double(destination, payload_double, double_precision);
            if (destination)
              destination += entries;
            result += entries;
            break;
          case 'p':
          case 'P':
          case 'x':
          case 'X':
            payload_pointer = (pointer_t)grab_next_argument(argument_stack_pointer, void *);
            entries = string_hex_from_void(destination, payload_pointer, sizeof(int));
            if (destination)
              destination += entries;
            result += entries;
            break;
          default:
            /* if the value we got is a digit, means we're going to start 
             * (or to continue) a redefinition of the default precision required 
             * in representing double values. We can have only positive values 
             * so, it doesn't make any sense in checking for a negative sign.
             */
            if ((*(format + 1) >= '0') && (*(format + 1) <= '9')) {
              if (new_precision_ongoing)
                double_precision = (double_precision * 10) + (*(format + 1) - '0');
              else {
                double_precision = (*(format + 1) - '0');
                new_precision_ongoing = 1;
              }
              ++format;
              continue_loop = 1;
            }
            break;
        }
      } while(continue_loop);
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
size_t string_create(char *destination, const char *format, ...) {
  unsigned char *argument_stack_pointer = grab_pointer_first_variable_argument(format);
  return string_create_stack_pointer(destination, format, argument_stack_pointer);
}
void kernel_printf(const char *format, ...) {
  unsigned char *argument_stack_pointer = grab_pointer_first_variable_argument(format);
  char *buffer;
  size_t length;
  if ((length = string_create_stack_pointer(NULL_PTR, format, argument_stack_pointer)) > 0) {
    if ((buffer = (char *)memory_alloc(length + 1))) {
      string_create_stack_pointer(buffer, format, argument_stack_pointer);
      raw_video_print_string(&standard_output, buffer);
      memory_free(buffer);
    }
  }
}
