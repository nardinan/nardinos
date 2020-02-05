SOURCES = $(wildcard kernel/*.c kernel/*/*.c drivers/*.c drivers/*/*.c)
HEADERS = $(wildcard kernel/*.h kernel/*/*.c drivers/*.h drivers/*/*.h)
OBJ = ${SOURCES:.c=.o}
CC = /usr/local/i386_elf_gcc/bin/i386-elf-gcc
GDB = /usr/local/i386_elf_gcc/bin/i386-elf-gdb
CFLAGS = -g

nardinos.bin: bootstrap.bin kernel.bin
	cat $^ > nardinos.bin

kernel.bin: kernel/kernel_entry_point.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ --oformat binary

kernel.elf: kernel/kernel_entry_point.o ${OBJ}
	i386-elf-ld -o $@ -Ttext 0x1000 $^ 

run: nardinos.bin
	qemu-system-i386 -fda nardinos.bin

# Open the connection to qemu and load our kernel-object file with symbols
debug: nardinos.bin kernel.elf
	qemu-system-i386 -s -fda nardinos.bin &
	${GDB} -ex "target remote localhost:1234" -ex "symbol-file kernel.elf"

# Generic rules for wildcards
# To make an object, always compile from its .c
%.o: %.c ${HEADERS}
	${CC} ${CFLAGS} -ffreestanding -c $< -o $@

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

clean:
	rm -f *.bin *.dis *.o nardinos.bin *.elf
	rm -f `find . -name *.o`
	rm -rf kernel/*.o drivers/*.o

