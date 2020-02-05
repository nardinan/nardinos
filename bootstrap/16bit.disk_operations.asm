; The routine reads dh sectors from the drive dl, into es:bx; In case
; of failures, the routine goes in panic mode (usual infinite loop)
load_kernel_from_disk_dl:
  pusha
  push dx
  mov ah, 0x02
  ; OK here we need to give a quick explanation. dh contains the number of sectors we 
  ; need to read but we need dh to index the disk so we're going to store the content
  ; of dh into al.
  ; The next entries are clusters, cylinder and head of the disk we need to read. We
  ; are going to start from the second sector because the first one is the boot sector
  ; and is already taken.
  mov al, dh
  mov cl, 0x02
  mov ch, 0x00
  mov dh, 0x00
  int 0x13
  jc load_disk_dl_error
  pop dx
  cmp al, dh
  jne load_sectors_dl_error
  popa
  ret
load_disk_dl_error:
  mov bx, stored_load_disk_error_message
  call print_bx_string_nl
  jmp $ ; we're going to drop here out efforts
load_sectors_dl_error:
  mov bx, stored_load_sectors_error_message
  call print_bx_string_nl
  jmp $ ; we're going to drop here out efforts
	
stored_load_disk_error_message:
  db "Disk read error", 0

stored_load_sectors_error_message:
  db "Incorrect number of sectors", 0
