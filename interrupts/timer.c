#include "timer.h"
unsigned int cpu_tick = 0;
void timer_set(unsigned int frequency) {
  unsigned int divisor = (1193182 / frequency); /* The PIT (Programmable Interval Timer) frequency */
  interrupts_request_register(INTERRUPT_REQUEST0, timer_callback);
  port_byte_write(0x43, 0x36); /* Command port */
  port_byte_write(0x40, (divisor&0xff));
  port_byte_write(0x40, ((divisor >> 8)&0xff));
}
void timer_callback(s_asm_registers registers) {
  (void)(registers);
  ++cpu_tick;
}
