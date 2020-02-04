; The routine starts to print each byte from the address stored in bx 
; until a byte equal to zero is reached
print_bx_string:
  pusha ; We're going to push all the values into the registeres into the stack
  mov ah, 0x0e
continue_print_string:
	mov al, [bx]
	cmp al, 0
	jne string_not_yet_over
  popa ; We're going to recover all the values from the stack and put back into the register.
       ; In this way, bx will take back its original value
	ret
string_not_yet_over:
	int 0x10
	add bx, 1
	jmp continue_print_string

; The routine prints the string in the buffer bx but, appends
; to the end a new-line character and a carriage return. This
; means that the cursor will move back to the home at the
; line after
print_bx_string_nl:
  call print_bx_string
  pusha
  mov ah, 0x0e
  mov al, 0x0a
  int 0x10
  mov al, 0x0d
  int 0x10
  popa
  ret

; The routine prints out the content of the register dx as a string
; using its hexadecimal code. (remember, the register is 16 bits so
; the function will print everything)
print_dx_hex:
  pusha
  ; in bx we have now the address of the string; This means that
  ; we should follow the approach:
  ; [stored_hex_format + 0] = current value + (bl and 0xF)
  ; [stored_hex_format + 1] = current value + (bl shr 4)
  ; [stored_hex_format + 2] = current value + (bh and 0xF)
  ; [stored_hex_format + 3] = current value + (bl shr 4)
  ; We need however to take into account the fact that if we're 
  ; going to get a value bigger than 9, we need to add there 8 which is
  ; the jump in the ASCII table from the lates number to the first 
  ; letter of the alphabet
  mov ax, 4
continue_shifting_hex:
  cmp ax, 0
  je stop_shifting_hex
  mov cl, dl
  and cl, 0x0F
  cmp cl, 0x09
  jle continue_without_increasing_hex
  add cl, 0x07
continue_without_increasing_hex:
  add cl, 0x30
  ; OK now in the lower side of the cx register we have the right byte
  ; (already converted into ASCII) and then we're going to store it
  ; into the space reserved for the string
  mov bx, stored_hex_format
  add bx, ax
  add bx, 1
  mov [bx], cl
  shr dx, 4
  sub ax, 1 
  jmp continue_shifting_hex
stop_shifting_hex:
  popa
  mov bx, stored_hex_format
  call print_bx_string
  ret

stored_hex_format:
  db "0x0000", 0
