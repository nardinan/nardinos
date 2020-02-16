#include "keyboard.h"
#include "../video/raw_video.h"
void keyboard_set(void) {
  interrupts_request_register(INTERRUPT_REQUEST1, keyboard_callback);
}

void keyboard_callback(s_asm_registers registers) {
  static char keyboard_ascii_map[] = {'?', '?', '1', '2', '3', '4', '5', '6',     
    '7', '8', '9', '0', '-', '=', '\r', '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 
    'U', 'I', 'O', 'P', '[', ']', '?', '?', 'A', 'S', 'D', 'F', 'G', 
    'H', 'J', 'K', 'L', ';', '\'', '`', '?', '\\', 'Z', 'X', 'C', 'V', 
    'B', 'N', 'M', ',', '.', '/', '?', '?', '?', ' '};
  unsigned char incoming_character = port_byte_read(0x60);
  (void)(registers); // is unused in this specific routine
  if (incoming_character <= KEYBOARD_MAPPED_KEYS)
    keyboard_push_incoming_key(keyboard_ascii_map[incoming_character]);
  else
    keyboard_push_incoming_key('X');
}
void keyboard_push_incoming_key(unsigned char key) {
  char buffer[] = {key, 0};
  raw_video_print_string(&standard_output, buffer);
}
