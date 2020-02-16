#ifndef DRIVERS_KEYBOARD_KEYBOARD
#define DRIVERS_KEYBOARD_KEYBOARD
#include "../../interrupts/interrupts_signals.h"
#define KEYBOARD_MAPPED_KEYS 57
#define KEYBOARD_INCOMING_BUFFER_SIZE 256
extern void keyboard_set(void);
extern void keyboard_callback(s_asm_registers registers);
extern void keyboard_push_incoming_key(unsigned char key);
#endif
