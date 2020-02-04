#include "ports.h"
unsigned char port_byte_read(unsigned short int port) {
  unsigned char result;
  __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
  return result;
}
void port_byte_write(unsigned short int port, unsigned char data) {
  __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}
unsigned short int port_word_read(unsigned short int port) {
  unsigned short int result;
  __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
  return result;
}
void port_word_write(unsigned short int port, unsigned short data) {
  __asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}

