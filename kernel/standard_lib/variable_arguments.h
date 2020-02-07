#ifndef STANDARD_LIBRARY_VARIABLE_ARGUMENTS
#define STANDARD_LIBRARY_VARIABLE_ARGUMENTS
/* I have to be honest, I've peeked into the stdarg source because seems that this
 * is the amount of size each argument takes into the stack due to the alignment.
 * I had no idea. Now I know and it makes sense.
 */
#define argument_size_in_stack(type)\
  (((sizeof(type)+sizeof(int)-1)/sizeof(int))*sizeof(int))
/* we need to collect the current element and we need to increase the pointer in
 * order to be able to access the next element at the next call 
 */
#define grab_next_argument(valist,type)\
  (((valist) = (valist)+argument_size_in_stack(type)), \
   *((type *)((valist)-argument_size_in_stack(type))))
/* here the initialization of the pointer that points to the very
 * first element in the stack of the list or variable arguments 
 */
#define grab_pointer_first_variable_argument(last_known_argument)\
  ((unsigned char *)__builtin_next_arg(last_known_argument))
#endif
