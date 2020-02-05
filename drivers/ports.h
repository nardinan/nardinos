#ifndef DRIVERS_PORTS
#define DRIVERS_PORTS
extern unsigned char port_byte_read(unsigned short port);
extern void port_byte_write(unsigned short port, unsigned char data);
extern unsigned short port_word_read(unsigned short port);
extern void port_word_write(unsigned short port, unsigned short data);
#endif
