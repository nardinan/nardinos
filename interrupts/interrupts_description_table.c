#include "interrupts_description_table.h"
s_interrupts_description_table_gate interrupts_description_table_gate[INTERRUPTS_DESCRIPTION_TABLE_ENTRIES];
s_interrupts_description_table_register interrupts_description_table_register;
void interrupts_description_table_set_gate(int index, unsigned int handler) {
  interrupts_description_table_gate[index].lower_offset = (handler&0xffff);
  interrupts_description_table_gate[index].higher_offset = ((handler>>16)&0xffff);
  interrupts_description_table_gate[index].kernel_segment_selector = 0x08;
  /* the flag has to be set to 10001110 that stands for:
   * 1 - interrupt present
   * 0
   * 0 - kernel caller
   * 0 - interrupt gates
   * 1
   * 1
   * 1
   * 0 - 32 bit interrupt gate */
  interrupts_description_table_gate[index].flag = 0x8e;
  interrupts_description_table_gate[index].padding_zero_byte = 0;
}
void interrupts_description_table_set(void) {
  interrupts_description_table_register.base = (unsigned int)&(interrupts_description_table_register);
  interrupts_description_table_register.limit = (INTERRUPTS_DESCRIPTION_TABLE_ENTRIES * sizeof(s_interrupts_description_table_gate)) - 1;
  __asm__ __volatile__("lidtl (%0)" : : "r"(&interrupts_description_table_register));
}

