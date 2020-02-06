#include "memory.h"
s_memory_area memory;
void memory_copy(unsigned char *destination, unsigned char *source, size_t size) {
  memory_move(destination, source, size);
}
void memory_move(unsigned char *destination, unsigned char *source, size_t size) {
  if (source > destination) {
    while (size > 0) {
      *destination = *source;
      ++destination;
      ++source;
      --size;
    }
  } else if (source < destination) {
    destination += size;
    source += size;
    while (size > 0) {
      *destination = *source;
      --destination;
      --source;
      --size;
    }
  } 
  /* if the source and the destination are the same, 
   * we don't need to copy anything */
}
void memory_set(unsigned char *destination, unsigned char data, size_t size) {
  while (size > 0) {
    *destination = data;
    ++destination;
    --size;
  }
}
void memory_defrag(void) {
  /* we're going to pass through the fragments of memory and we're going to merge 
   * the blocks with the same flag together in order to defragment the area 
   * and to make room for more contigus available memory 
   */
  for (size_t index = 1;
      (index < memory.elements) &&
      (memory.fragments[index].flags & e_memory_flag_memory_initialized);)
    if (memory.fragments[index].flags == memory.fragments[index - 1].flags) {
      memory.fragments[index - 1].end = memory.fragments[index].end;
      if (index < (memory.elements - 1))
        memory_copy((unsigned char *)&(memory.fragments[index]), 
            (unsigned char *)&(memory.fragments[index + 1]),
            (sizeof(s_memory_fragment) * (memory.elements - index - 1)));
      memory.fragments[memory.elements - 1].flags = 0x0; /* last entry goes uninitialized */
      --(memory.elements);
    } else
      ++index;
}
void *memory_alloc(size_t bytes) {
  void *result = NULL_PTR;
  int defrag_required = 0;
  /* if the amounts of segments in the memory is bigger or equal to the maximum 
   * fragments, we're going to return a NULL pointer: our memory is too fragmented 
   * to be used unfortunately and we need to wait for other routines to release 
   * more memory.
   */
  if (memory.elements < MEMORY_MAXIMUM_FRAGMENTS) {
    size_t index = 0, required_size = bytes + sizeof(size_t);
    memory_defrag();
    for (;(index < memory.elements) &&
        (memory.fragments[index].flags & e_memory_flag_memory_initialized); ++index)
      if ((!(memory.fragments[index].flags & e_memory_flag_memory_taken)) &&
          ((memory.fragments[index].end - memory.fragments[index].start) >= required_size))
        break;
    if (memory.fragments[index].flags & e_memory_flag_memory_initialized) {
      size_t residual_memory = (memory.fragments[index].end - 
          memory.fragments[index].start) - required_size;
      result = memory.fragments[index].start;
      memory.fragments[index].flags |= e_memory_flag_memory_taken;
      if (residual_memory > 0) {
        if (index < (memory.elements - 1))
          memory_move((unsigned char *)&(memory.fragments[index + 2]),
              (unsigned char *)&(memory.fragments[index + 1]),
              (sizeof(s_memory_fragment) * (memory.elements - index - 1)));
        memory.fragments[index + 1].end = memory.fragments[index].end;
        memory.fragments[index + 1].flags = e_memory_flag_memory_initialized;
        memory.fragments[index].end -= residual_memory;
        ++(memory.elements);
        defrag_required = 1;
      }
    } else {
      if (index > 0) {
        memory.fragments[index].start = (memory.fragments[index - 1].end + 1);
        defrag_required = 1;
      } else
        memory.fragments[index].start = MEMORY_STARTING_LOCATION;
      result = memory.fragments[index].start;
      memory.fragments[index].end = memory.fragments[index].start + required_size;
      memory.fragments[index].flags = 
        (e_memory_flag_memory_taken | e_memory_flag_memory_initialized);
      ++(memory.elements);
    }
    memory_copy((unsigned char *)result, (unsigned char *)&(bytes), sizeof(size_t));
    if (defrag_required)
      memory_defrag();
  } 
  return (result)?(result + sizeof(size_t)):result;
}
void memory_free(void *pointer) {
  if (pointer) {
    size_t bytes;
    unsigned char *casted_pointer = ((unsigned char *)pointer - sizeof(size_t));
    memory_copy(casted_pointer, (unsigned char *)&bytes, sizeof(size_t));
    for (size_t index = 0;
        (index < memory.elements) &&
        (memory.fragments[index].flags & e_memory_flag_memory_initialized);
        ++index) {
      if ((casted_pointer >= memory.fragments[index].start) &&
          (casted_pointer <= memory.fragments[index].end)) {
        if (memory.fragments[index].flags & e_memory_flag_memory_taken) {
          /* worst case scenario:
           * segment[X] = [0x05] -- [0x10-0x20] --- [0x30] 
           * and we need to free the block of memory [0x10-0x20]. In this scenario, we need 
           * to fragment the memory even more because, from a single block, we're going to 
           * generate two additional segments or memory taken with, in the middle, a free block.
           * This means that we should have enough space available
           */
          size_t head_residual_memory = (casted_pointer - memory.fragments[index].start),
                 /* we can relay on the fact that the tail of the pointer is in the same memory block */
                 tail_residual_memory = (memory.fragments[index].end - (casted_pointer + sizeof(size_t) + bytes));
          int required_slots = 0;
          /* we check now if we have enough space in the queue to host these fragments of memory and,
           * in the worst case scenario, we're not going to release that part of memory (sigh) 
           */
          if ((head_residual_memory) && 
              (tail_residual_memory))
            required_slots = 2;
          else if ((head_residual_memory) || 
              (tail_residual_memory))
            required_slots = 1;
          if (memory.elements < (MEMORY_MAXIMUM_FRAGMENTS - required_slots)) {
            /* we need to check if we have enough space to create the required entries, otherwise
             * we have to handle the things in a different way and (sigh) leave the pointer there
             * allocated into the memory
             */
            if (head_residual_memory) {
              memory_move((unsigned char *)&(memory.fragments[index + 1]), 
                  (unsigned char *)&(memory.fragments[index]),
                  (sizeof(s_memory_fragment) * (memory.elements - index)));
              memory.fragments[index].end = memory.fragments[index].start + head_residual_memory;
              memory.fragments[index + 1].start = casted_pointer;
              ++(memory.elements);
              ++index;
            }
            if (tail_residual_memory) {
              memory_move((unsigned char *)&(memory.fragments[index + 2]),
                  (unsigned char *)&(memory.fragments[index + 1]),
                  (sizeof(s_memory_fragment) * (memory.elements - index - 1)));
              memory.fragments[index].end = (casted_pointer + sizeof(size_t) + bytes);
              memory.fragments[index + 1].start = (memory.fragments[index].end + 1);
              ++(memory.elements);
            }
            memory.fragments[index].flags &= ~e_memory_flag_memory_taken;
          }
        }
      }
    }
  }
}
