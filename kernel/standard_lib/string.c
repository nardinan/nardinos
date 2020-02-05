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
int is_printable(char character) {
  return ((character >= 0x20) && (character <= 0x7f));
}
