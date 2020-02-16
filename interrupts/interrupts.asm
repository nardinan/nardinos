[extern interrupts_signals_handler]

; This is the routine that every callback triggered by an 
; interrupt will call back in order to trigger the 
; C handler.
interrupts_signalc_callback:
  pusha
  mov ax, ds
  push eax
  mov ax, 0x10
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  call interrupts_signals_handler
  pop eax
  mov ds, ax,
  mov es, ax
  mov fs, ax
  mov gs, ax
  popa
  ; We need now to clean the error code and 
  ; the interrupt code we've just pushed to
  ; be sure we're not going to inject it to
  ; then next interrupt.
  add esp, 8
  sti
  iret

; This is a very important piece of information that arrives
; from the cfenollosa tutorial and that is the reason behind
; a so fragmented way to handle interrupts: we don't have any
; detail about the interrupt that has been called, so we need
; to create a routine for each one of them so we can 
; intercept the callback.
;
; We need to make the sure we can link the callbacks to the
; C extern rotuines.
global callback_interrupt_signal0
global callback_interrupt_signal1
global callback_interrupt_signal2
global callback_interrupt_signal3
global callback_interrupt_signal4
global callback_interrupt_signal5
global callback_interrupt_signal6
global callback_interrupt_signal7
global callback_interrupt_signal8
global callback_interrupt_signal9
global callback_interrupt_signal10
global callback_interrupt_signal11
global callback_interrupt_signal12
global callback_interrupt_signal13
global callback_interrupt_signal14
global callback_interrupt_signal15
global callback_interrupt_signal16
global callback_interrupt_signal17
global callback_interrupt_signal18
global callback_interrupt_signal19
global callback_interrupt_signal20
global callback_interrupt_signal21
global callback_interrupt_signal22
global callback_interrupt_signal23
global callback_interrupt_signal24
global callback_interrupt_signal25
global callback_interrupt_signal26
global callback_interrupt_signal27
global callback_interrupt_signal28
global callback_interrupt_signal29
global callback_interrupt_signal30
global callback_interrupt_signal31

callback_interrupt_signal0:
  cli
  push byte 0
  push byte 0
  jmp interrupts_signalc_callback

callback_interrupt_signal1:
  cli
  push byte 0
  push byte 1
  jmp interrupts_signalc_callback

callback_interrupt_signal2:
  cli
  push byte 0
  push byte 2
  jmp interrupts_signalc_callback

callback_interrupt_signal3:
  cli
  push byte 0
  push byte 3
  jmp interrupts_signalc_callback

callback_interrupt_signal4:
  cli
  push byte 0
  push byte 4
  jmp interrupts_signalc_callback

callback_interrupt_signal5:
  cli
  push byte 0
  push byte 5
  jmp interrupts_signalc_callback

callback_interrupt_signal6:
  cli
  push byte 0
  push byte 6
  jmp interrupts_signalc_callback

callback_interrupt_signal7:
  cli
  push byte 0
  push byte 7
  jmp interrupts_signalc_callback

callback_interrupt_signal8:
  cli
  ; this one has an error code, so we shouldn't 
  ; push an empty one
  push byte 8
  jmp interrupts_signalc_callback

callback_interrupt_signal9:
  cli
  push byte 0
  push byte 9
  jmp interrupts_signalc_callback

callback_interrupt_signal10:
  cli
  push byte 0
  ; this one has an error code, so we shouldn't 
  ; push an empty one
  push byte 10
  jmp interrupts_signalc_callback

callback_interrupt_signal11:
  cli
  push byte 0
  ; this one has an error code, so we shouldn't 
  ; push an empty one
  push byte 11
  jmp interrupts_signalc_callback

callback_interrupt_signal12:
  cli
  push byte 0
  ; this one has an error code, so we shouldn't 
  ; push an empty one
  push byte 12
  jmp interrupts_signalc_callback

callback_interrupt_signal13:
  cli
  push byte 0
  ; this one has an error code, so we shouldn't 
  ; push an empty one
  push byte 13
  jmp interrupts_signalc_callback

callback_interrupt_signal14:
  cli
  push byte 0
  ; this one has an error code, so we shouldn't 
  ; push an empty one
  push byte 14
  jmp interrupts_signalc_callback

callback_interrupt_signal15:
  cli
  push byte 0
  push byte 15
  jmp interrupts_signalc_callback

callback_interrupt_signal16:
  cli
  push byte 0
  push byte 16
  jmp interrupts_signalc_callback

callback_interrupt_signal17:
  cli
  push byte 0
  push byte 17
  jmp interrupts_signalc_callback

callback_interrupt_signal18:
  cli
  push byte 0
  push byte 18
  jmp interrupts_signalc_callback

callback_interrupt_signal19:
  cli
  push byte 0
  push byte 19
  jmp interrupts_signalc_callback

callback_interrupt_signal20:
  cli
  push byte 0
  push byte 20
  jmp interrupts_signalc_callback

callback_interrupt_signal21:
  cli
  push byte 0
  push byte 21
  jmp interrupts_signalc_callback

callback_interrupt_signal22:
  cli
  push byte 0
  push byte 22
  jmp interrupts_signalc_callback

callback_interrupt_signal23:
  cli
  push byte 0
  push byte 23
  jmp interrupts_signalc_callback

callback_interrupt_signal24:
  cli
  push byte 0
  push byte 24
  jmp interrupts_signalc_callback

callback_interrupt_signal25:
  cli
  push byte 0
  push byte 25
  jmp interrupts_signalc_callback

callback_interrupt_signal26:
  cli
  push byte 0
  push byte 26
  jmp interrupts_signalc_callback

callback_interrupt_signal27:
  cli
  push byte 0
  push byte 27
  jmp interrupts_signalc_callback

callback_interrupt_signal28:
  cli
  push byte 0
  push byte 28
  jmp interrupts_signalc_callback

callback_interrupt_signal29:
  cli
  push byte 0
  push byte 29
  jmp interrupts_signalc_callback

callback_interrupt_signal30:
  cli
  push byte 0
  push byte 30
  jmp interrupts_signalc_callback

callback_interrupt_signal31:
  cli
  push byte 0
  push byte 31
  jmp interrupts_signalc_callback



