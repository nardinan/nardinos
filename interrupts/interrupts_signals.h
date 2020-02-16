#ifndef INTERRUPTS_SIGNALS
#define INTERRUPTS_SIGNALS
#include "interrupts_description_table.h"
/* These are the callback that will be mapped and associated to the interrupt signals. Each 
 * interrupt, once registered and once thrown from the CPU, will call the routine registered 
 * in the assembly code that will, as expected, record the information about the interrupt 
 * and call the routine interrupt_signal_hander hereby defined.
 */
#define INTERRUPT_DEFINE_CALLBACK_NAME(number) \
  callback_interrupt_signal##number
#define INTERRUPT_DEFINE_CALLBACK(number) \
  extern void INTERRUPT_DEFINE_CALLBACK_NAME(number)(void)
INTERRUPT_DEFINE_CALLBACK(0);
INTERRUPT_DEFINE_CALLBACK(1);
INTERRUPT_DEFINE_CALLBACK(2);
INTERRUPT_DEFINE_CALLBACK(3);
INTERRUPT_DEFINE_CALLBACK(4);
INTERRUPT_DEFINE_CALLBACK(5);
INTERRUPT_DEFINE_CALLBACK(6);
INTERRUPT_DEFINE_CALLBACK(7);
INTERRUPT_DEFINE_CALLBACK(8);
INTERRUPT_DEFINE_CALLBACK(9);
INTERRUPT_DEFINE_CALLBACK(10);
INTERRUPT_DEFINE_CALLBACK(11);
INTERRUPT_DEFINE_CALLBACK(12);
INTERRUPT_DEFINE_CALLBACK(13);
INTERRUPT_DEFINE_CALLBACK(14);
INTERRUPT_DEFINE_CALLBACK(15);
INTERRUPT_DEFINE_CALLBACK(16);
INTERRUPT_DEFINE_CALLBACK(17);
INTERRUPT_DEFINE_CALLBACK(18);
INTERRUPT_DEFINE_CALLBACK(19);
INTERRUPT_DEFINE_CALLBACK(20);
INTERRUPT_DEFINE_CALLBACK(21);
INTERRUPT_DEFINE_CALLBACK(22);
INTERRUPT_DEFINE_CALLBACK(23);
INTERRUPT_DEFINE_CALLBACK(24);
INTERRUPT_DEFINE_CALLBACK(25);
INTERRUPT_DEFINE_CALLBACK(26);
INTERRUPT_DEFINE_CALLBACK(27);
INTERRUPT_DEFINE_CALLBACK(28);
INTERRUPT_DEFINE_CALLBACK(29);
INTERRUPT_DEFINE_CALLBACK(30);
INTERRUPT_DEFINE_CALLBACK(31);
/* we need to have a structure that aggregates all the parameters we're going to
 * receive from the assembly callback. The values are pushed into the stack (in
 * reverse order) directly by the assembly callback implemented.
 * - data_segment
 * - edi
 * - esi
 * - epb
 * - ebx
 * - edx
 * - ecx
 * - eax: these are pushed directly from the common code, called by each interrupt
 *   callback once the callback is terminated. 
 * - interrupt_number: this is the value filled by the callback in the interrupt
 *   in order to help us distinguish among the different callbacks.
 * - everything else is going to be automatically pushed by the processor
 *
 * We don't obviously need to align the structure because contains only 32bits 
 * types.
 */
typedef struct s_asm_registers {
  unsigned int data_segment, edi, esi, epb, esp, ebx, edx, ecx, eax, interrupt_number, error_code, eip, cs, eflags, usersp, ss; 
} s_asm_registers;
void interrupts_signals_register(void);
void interrupts_signals_handler(s_asm_registers registers);
#endif
