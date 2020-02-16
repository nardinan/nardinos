#ifndef INTERRUTPS_DESCRIPTION_TABLE
#define INTERRUPTS_DESCRIPTION_TABLE
#define INTERRUPTS_DESCRIPTION_TABLE_ENTRIES 256
#pragma pack(push, 1)
typedef struct INTERRUTPS_DESCRIPTION_TABLErrupts_description_table_gate {
  unsigned short int lower_offset, kernel_segment_selector;
  /* The flag here stored is the most important source of information; Following the 
   * code and the comments written by @cfenollosa we have:
   * 0-3 bits = 1110, that means that we're using a 32-bit interrupt gate
   * 4 bit    = 0, for interrupt gates
   * 6-5 bits = priviledge level of the caller (0 is kernel, 3 is the user)
   * 7 bit    = 1, that means the interrupt is present
   */
  unsigned char padding_zero_byte, flag;
  unsigned short int higher_offset;
} s_interrupts_description_table_gate;
typedef struct s_interrupts_description_table_register {
  unsigned short int limit;
  unsigned int base;
} s_interrupts_description_table_register;
#pragma pack(pop)
extern s_interrupts_description_table_gate interrupts_description_table_gates[INTERRUPTS_DESCRIPTION_TABLE_ENTRIES];
extern s_interrupts_description_table_register interrupts_description_table_register;
void interrupts_description_table_set_gate(int index, unsigned int handler);
void interrupts_description_table_set(void);
#endif
