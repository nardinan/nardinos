#ifndef STANDARD_LIBRARY_MEMORY
#define STANDARD_LIBRARY_MEMORY
/* this value defines the maximum amount of fragments we may 
 * keep in memory at the same time; Every time an allocation is
 * requested, we pass through the memory index searching for 
 * the first empty block big enough to keep the space required
 * Every time we find two contigous blocks of memory with the 
 * same flags, we merge them together into a single block. 
 * If we reach the end without detecting any space, we will 
 * create a new empty entry and we're going to use that one. */
#define MEMORY_MAXIMUM_FRAGMENTS 512
/* this is where the usable memory starts. The compiler has to
 * calculate it and re-define it accordingly with the space 
 * taken by the kernel. We know the kernel starts at 0x1000 so,
 * we need to take the space of the kernel + 0x1000 to fetch
 * new memory areas. */
#define MEMORY_STARTING_LOCATION ((void *)0x10000)
/* null pointer, returned if no memory can be allocated 
 * (memory is heavily fragmented and no more space can be
 * allocated). Usually, if an operation like a malloc 
 * returns null pointer, we might want to call kernel panic */
#define NULL_PTR ((void *)0x0)
typedef unsigned int size_t;
typedef long int pointer_t;
typedef enum e_memory_flags {
  e_memory_flag_memory_initialized  = 0x01,
  e_memory_flag_memory_taken        = 0x02
} e_memory_flags;
typedef struct s_memory_fragment {
  unsigned char *start, *end, flags;
} s_memory_fragment;
typedef struct s_memory_area {
  s_memory_fragment fragments[MEMORY_MAXIMUM_FRAGMENTS];
  size_t elements;
} s_memory_area;
extern s_memory_area memory;
void memory_copy(unsigned char *destination, unsigned char *source, size_t size);
void memory_move(unsigned char *destination, unsigned char *source, size_t size);
void memory_set(unsigned char *destination, unsigned char data, size_t size);
void memory_defrag(void);
void *memory_alloc(size_t bytes) __attribute__((warn_unused_result));
void memory_free(void *pointer);
#endif
