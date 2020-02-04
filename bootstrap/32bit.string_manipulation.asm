[bits 32]
; The routine starts to print each byte from the address stored in
; ebx until a byte equal to zero is reached. All the bytes are written
; starting from the home position of the screen at line in the register
; ecx
VIDEO_MEMORY equ 0Xb8000 ; initial position of the video memory
WHITE_ON_BLACK equ 0x0f  ; character-profile (white character over black
                         ; background)
SCREEN_COLUMNS equ 80
pm_print_ebx_string:
  pusha
  mov edx, VIDEO_MEMORY
  imul ecx, SCREEN_COLUMNS
  imul ecx, 2
  add edx, ecx
pm_continue_print_string:
  mov al, [ebx]
  ; The second half of the al register, takes the byte
  ; that identifies the property of the character.
  ; For the time being the routine works only with white-on-black
  ; (which means white character over black background)
  mov ah, WHITE_ON_BLACK
  cmp al, 0
  jne pm_string_not_yet_over
  popa
  ret
pm_string_not_yet_over:
  ; We write directly into the video memory our character, then we
  ; increase the position of the parameter by 1 byte (to advance to
  ; the next character) and the position of the video memory by
  ; two (due to the fact that the characters are encoded using two
  ; bytes)
  mov [edx], ax
  add ebx, 1
  add edx, 2
  jmp pm_continue_print_string


