#ifndef STANDARD_LIBRARY_MEMORY
#define STANDARD_LIBRARY_MEMORY
typedef unsigned int size_t;
void memory_copy(unsigned char *destination, unsigned char *source, size_t size);
void memory_set(unsigned char *destination, unsigned char data, size_t size);
#endif
