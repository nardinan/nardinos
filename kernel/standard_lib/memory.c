#include "memory.h"
void memory_copy(unsigned char *destination, unsigned char *source, size_t size) {
  while(size > 0) {
    *destination = *source;
    ++destination;
    ++source;
    --size;
  }
}
void memory_set(unsigned char *destination, unsigned char data, size_t size) {
  while (size > 0) {
    *destination = data;
    ++destination;
    --size;
  }
}
