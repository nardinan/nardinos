#ifndef INTERRUPTS_SIGNALS
#define INTERRUPTS_SIGNALS
#include "../drivers/ports.h"
#include "interrupts_description_table.h"

/* codes used by the interrupts request in order to identify them. This part of the code goes 
 * directly to the user for the registration so, we need to give him enough informations to
 * map the incoming interrupts requests
 */
#define INTERRUPT_REQUEST0 32
#define INTERRUPT_REQUEST1 33
#define INTERRUPT_REQUEST2 34
#define INTERRUPT_REQUEST3 35
#define INTERRUPT_REQUEST4 36
#define INTERRUPT_REQUEST5 37
#define INTERRUPT_REQUEST6 38
#define INTERRUPT_REQUEST7 39
#define INTERRUPT_REQUEST8 40
#define INTERRUPT_REQUEST9 41
#define INTERRUPT_REQUEST10 42
#define INTERRUPT_REQUEST11 43
#define INTERRUPT_REQUEST12 44
#define INTERRUPT_REQUEST13 45
#define INTERRUPT_REQUEST14 46
#define INTERRUPT_REQUEST15 47

/* These are the callbacks that will be mapped and associated to the interrupt signals and
 * interrupt requests. Each interrupt, once registered and once thrown from the CPU, will call 
 * the routine registered in the assembly code that will, as expected, record the information 
 * about the interrupt and call the routine interrupt_signal_hander hereby defined.
 */
#define INTERRUPT_DEFINE_CALLBACK_SIGNAL_NAME(number) \
  callback_interrupt_signal##number
#define INTERRUPT_DEFINE_CALLBACK_REQUEST_NAME(number) \
  callback_interrupt_request##number
#define INTERRUPT_DEFINE_CALLBACK_SIGNAL(number) \
  extern void INTERRUPT_DEFINE_CALLBACK_SIGNAL_NAME(number)(void)
#define INTERRUPT_DEFINE_CALLBACK_REQUEST(number) \
  extern void INTERRUPT_DEFINE_CALLBACK_REQUEST_NAME(number)(void)

INTERRUPT_DEFINE_CALLBACK_SIGNAL(0);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(1);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(2);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(3);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(4);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(5);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(6);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(7);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(8);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(9);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(10);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(11);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(12);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(13);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(14);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(15);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(16);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(17);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(18);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(19);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(20);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(21);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(22);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(23);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(24);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(25);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(26);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(27);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(28);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(29);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(30);
INTERRUPT_DEFINE_CALLBACK_SIGNAL(31);
INTERRUPT_DEFINE_CALLBACK_REQUEST(0);
INTERRUPT_DEFINE_CALLBACK_REQUEST(1);
INTERRUPT_DEFINE_CALLBACK_REQUEST(2);
INTERRUPT_DEFINE_CALLBACK_REQUEST(3);
INTERRUPT_DEFINE_CALLBACK_REQUEST(4);
INTERRUPT_DEFINE_CALLBACK_REQUEST(5);
INTERRUPT_DEFINE_CALLBACK_REQUEST(6);
INTERRUPT_DEFINE_CALLBACK_REQUEST(7);
INTERRUPT_DEFINE_CALLBACK_REQUEST(8);
INTERRUPT_DEFINE_CALLBACK_REQUEST(9);
INTERRUPT_DEFINE_CALLBACK_REQUEST(10);
INTERRUPT_DEFINE_CALLBACK_REQUEST(11);
INTERRUPT_DEFINE_CALLBACK_REQUEST(12);
INTERRUPT_DEFINE_CALLBACK_REQUEST(13);
INTERRUPT_DEFINE_CALLBACK_REQUEST(14);
INTERRUPT_DEFINE_CALLBACK_REQUEST(15);

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
typedef void (*t_callback_interrupt_request)(s_asm_registers);
extern const char *interrupt_description[];
void interrupts_signals_register(void);
void interrupts_request_register(unsigned char number, t_callback_interrupt_request callback);
void interrupts_signals_handler(s_asm_registers registers);
void interrupts_request_handler(s_asm_registers registers);
#endif
