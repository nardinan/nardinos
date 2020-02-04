; This is our offset; Is here because we're going to be loaded at the address 0x7c00 of the memory
; and, to access the addresses here encoded, we need to add this value.
[org 0x7c00]

; Store the DISK into sotred_local_disk so, we can use this area to keep the disk 
; index (yes, at the beginning the disk number is stored into dl)
mov [stored_local_disk], dl

; We need to configure the stack so, knowing that the BIOS reserved area ends at 0x7c00 
; and that this bootstrap will be loaded at 0x7c00 and will take 0x7c00 + 512bytes (0x7e00), 
; I am going to set it very far enough, in the empty area between 0x9fc00 and 0x7e00 (remember
; that the current address is an offset from 0x7c00)
mov bp, 0x8000
mov sp, bp

mov bx, stored_intro_message
call print_bx_string_nl
mov bx, stored_loading_kernel_message
call print_bx_string_nl
mov bx, KERNEL_OFFSET
mov dh, 2
mov dl, [stored_local_disk]
call load_kernel_from_disk_dl
mov bx, stored_leaving_16bit_message
call print_bx_string_nl
call switch_to_protected_mode_32bit

; We don't want to go ahead with this block so, we're going to push an instruction that loops
; forever (anyway we'll never reach it because the switch of the system in 32bit protect mode 
; will jump in the 32bit block below)
jmp $

; Here we reserve some memory to keep the strings we're going to show at the beginning of the
; bootstrap.
; in-code memory reservation
stored_intro_message:
	db "BOOTING nardinOS", 0

stored_loading_kernel_message:
	db "Loading KERNEL into memory", 0

stored_leaving_16bit_message:
	db "Leaving the 16bit normal mode", 0

stored_entering_32bit_message:
  db "Landing in 32bit protected mode", 0

stored_launching_kernel:
	db "Initializing the KERNEL", 0

stored_local_disk:
	db 0

; constants definition
KERNEL_OFFSET equ 0x1000 ; The offset that we're going to use when we link the kernel

%include "bootstrap/16bit.string_manipulation.asm"
%include "bootstrap/16bit.disk_operations.asm"
%include "bootstrap/32bit.global_description_table.asm"
%include "bootstrap/32bit.string_manipulation.asm"

[bits 32]
enter_protected_mode_32bit:
  mov ebx, stored_entering_32bit_message 
  mov ecx, 15 ; we cannot predict where we'll be, so let's give few lines for the previous messages
  call pm_print_ebx_string 
  mov ebx, stored_launching_kernel
  mov ecx, 16
  call pm_print_ebx_string
  call KERNEL_OFFSET

jmp $

; OK so, this bootstrap has to be 512bytes long and has to be at cylinder 0, head 0, sector 0. This means 
; that the bootstrap code that we wrote just before has to be padded with zeroes in order to make this code 
; exactly 512 bytes long.
; Moreover, to make sure that the system recognizes this disk as bootable, we need to add at the very end
; a magic number (0xAA55)
times 510-($-$$) db 0
dw 0xaa55
