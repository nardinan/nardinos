; This block, placed on top of the KERNEL will help us to start the execution
; even if the main() routine is not the first routine of the code.
; Pretty cool, uh?
[bits 32]
[extern main]
call main
jmp $
