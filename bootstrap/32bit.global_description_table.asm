[bits 16]
; The definition of the GDT is here, and will be 
; used to enter into the so-called 32bit protected
; mode.
gdt_start:
  ; We need to start with a 8 byte NULL pointer
  ; (oh yes, this will be our NULL pointer)
  dd 0x0
  dd 0x0

; GDT for code segment (base is 0, length is 0xfffff)
gdt_code:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10011010b
  db 11001111b
  db 0x0

; GDT for data segment (base is 0, length is 0xfffff)
gdt_data:
  dw 0xffff
  dw 0x0
  db 0x0
  db 10010010b
  db 11001111b
  db 0x0

gdt_end:
gdt_descriptor:
  dw gdt_end - gdt_start - 1 ; 16 bits to mark the side of the GDT table
  dd gdt_start               ; 32 bits to mark the start of the GDT table

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; The function switches from 16bit mode to 32bit protected
; mode and returns the control of the system to the label
; 'enter_protected_mode_32bit'
switch_to_protected_mode_32bit:
  ; OK here the situation is kind of critical and we need 
  ; to follow the process exactly. First, we need to disable
  ; the interrupts, then we need to load the gdt descriptor
  ; and then, we can enable the 32bit protected mode.
  cli
  lgdt [gdt_descriptor]
  ; we cannot operate on cr0 directly, so we need to store
  ; the content first on eax and then, from there, moving
  ; it back into cr0
  mov eax, cr0
  or eax, 0x1
  mov cr0, eax
  jmp CODE_SEG:switch_to_protected_mode_32bit_entry

[bits 32]
switch_to_protected_mode_32bit_entry:
  mov ax, DATA_SEG
  mov ds, ax
  mov ss, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  ; we need to re-update the stack to be at the top of the 
  ; empty space (so, we'll pick once again 0x90000)
  mov ebp, 0x90000
  mov esp, ebp
  call enter_protected_mode_32bit


