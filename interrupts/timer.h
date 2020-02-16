#ifndef TIMER
#define TIMER
#include "interrupts_signals.h"
extern unsigned int cpu_tick;
extern void timer_set(unsigned int frequency);
extern void timer_callback(s_asm_registers registers);
#endif
