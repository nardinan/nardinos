#include "interrupts_signals.h"
#include "../drivers/video/raw_video.h"
#include "../kernel/standard_lib/standard_lib.h"
t_callback_interrupt_request interrupts_handler[INTERRUPTS_DESCRIPTION_TABLE_ENTRIES];
char *interrupt_description[] = {
  "division by zero",
  "debug",
  "non maskable interrupt",
  "breakpoint",
  "into detected overflow",
  "out of bounds",
  "invalid opcode",
  "no copocessor",
  "double fault",
  "coprocessor segment overrun",
  "bad TSS",
  "segment not present",
  "stack fault",
  "general protection fault",
  "page fault",
  "unknown interrupt",
  "coprocessor fault",
  "alignment check",
  "machine check",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved",
  "reserved"
};
#define INTERRUPT_SIGNAL_REGISTER_CALLBACK(number) \
  interrupts_description_table_set_gate(number, (unsigned int)INTERRUPT_DEFINE_CALLBACK_SIGNAL_NAME(number))
#define INTERRUPT_REQUEST_REGISTER_CALLBACK(number) \
  interrupts_description_table_set_gate((number + INTERRUPT_REQUEST0), \
      (unsigned int)INTERRUPT_DEFINE_CALLBACK_REQUEST_NAME(number))
void interrupts_signals_register(void) {
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(0);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(1);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(2);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(3);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(4);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(5);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(6);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(7);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(8);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(9);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(10);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(11);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(12);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(13);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(14);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(15);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(16);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(17);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(18);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(19);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(20);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(21);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(22);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(23);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(24);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(25);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(26);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(27);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(28);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(29);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(30);
  INTERRUPT_SIGNAL_REGISTER_CALLBACK(31);

  /* we're going to remap the PIC but, unfortunately, still not clear why we're 
   * going to do it.
   */
  port_byte_write(0x20, 0x11);
  port_byte_write(0xA0, 0x11);
  port_byte_write(0x21, 0x20);
  port_byte_write(0xA1, 0x28);
  port_byte_write(0x21, 0x04);
  port_byte_write(0xA1, 0x02);
  port_byte_write(0x21, 0x01);
  port_byte_write(0xA1, 0x01);
  port_byte_write(0x21, 0x00);
  port_byte_write(0xA1, 0x00);

  /* registration of the callbacks for the interrupt requests coming from the 
   * hardware (e.g. from the keyboard). We want to give the the system the
   * full control and ability to re-map in real time these callbacks.
   */
  INTERRUPT_REQUEST_REGISTER_CALLBACK(0);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(1);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(2);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(3);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(4);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(5);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(6);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(7);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(8);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(9);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(10);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(11);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(12);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(13);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(14);
  INTERRUPT_REQUEST_REGISTER_CALLBACK(15);
  interrupts_description_table_set();
}
void interrupts_request_register(unsigned char number, t_callback_interrupt_request callback) {
  interrupts_handler[number] = callback;
}
void interrupts_signals_handler(s_asm_registers registers) {
  kernel_printf("INT SIG: %d (%s)\n", registers.interrupt_number, interrupt_description[registers.interrupt_number]);
}
void interrupts_request_handler(s_asm_registers registers) {
  /* when we receive a interrupt request we need to reset some statuses 
   * in order to avoid to receive the very same request, once again, 
   * just after.
   */
  kernel_printf("INT REQ: %d\n", registers.interrupt_number);
  if (registers.interrupt_number >= 40) 
    port_byte_write(0xa0, 0x20);
  port_byte_write(0x20, 0x20);
  if (interrupts_handler[registers.interrupt_number])
    interrupts_handler[registers.interrupt_number](registers);
}
