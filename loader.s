MAGIC_NUMBER      equ 0x1BADB002
FLAGS             equ 0x0
CHECKSUM          equ -MAGIC_NUMBER
KERNEL_STACK_SIZE equ 4096

section .text                   
align 4                         
    dd MAGIC_NUMBER             
    dd FLAGS                    
    dd CHECKSUM                 
    dd loader

global loader
extern kernel_main
loader:
    mov esp, kernel_stack + KERNEL_STACK_SIZE
    push ebx

    call kernel_main

.loop:
    jmp .loop

global outb
outb:
    mov al, [esp + 8]
    mov dx, [esp + 4]
    out dx, al
    ret

global idt_flush
idt_flush:
   mov eax, [esp+4]
   lidt [eax]
   ret

section .bss
align 4
kernel_stack:
    resb KERNEL_STACK_SIZE

; Interrupt Service Request Handles
; We define 2 macro's: one for exceptions that do not push an error code
; and one for exceptions that do push an error code
; NASM will convert these to the 32 appropriate code entries

%macro ISR_NOERRORCODE 1
global isr%1
isr%1:
	push byte 0		; push a dummy error code
	push byte %1		; push the interrup number
	jmp alltraps
%endmacro

%macro ISR_WITHERRORCODE 1
global isr%1
isr%1:
	push byte %1		; only push interrupt number, error code has already been pushed
	jmp alltraps
%endmacro

; This macro creates a stub for an IRQ - the first parameter is
; the IRQ number, the second is the ISR number it is remapped to.
%macro IRQ 2
global irq%1
irq%1:
	push byte 0
	push byte %2
	jmp alltraps
%endmacro

; now use the macro's to define the right handlers
ISR_NOERRORCODE 0
ISR_NOERRORCODE 1
ISR_NOERRORCODE 2
ISR_NOERRORCODE 3
ISR_NOERRORCODE 4
ISR_NOERRORCODE 5
ISR_NOERRORCODE 6
ISR_NOERRORCODE 7
ISR_WITHERRORCODE 8
ISR_NOERRORCODE 9
ISR_WITHERRORCODE 10
ISR_WITHERRORCODE 11
ISR_WITHERRORCODE 12
ISR_WITHERRORCODE 13
ISR_WITHERRORCODE 14
ISR_NOERRORCODE 15
ISR_NOERRORCODE 16
ISR_WITHERRORCODE 17
ISR_NOERRORCODE 18
ISR_NOERRORCODE 19
ISR_NOERRORCODE 20
ISR_NOERRORCODE 21
ISR_NOERRORCODE 22
ISR_NOERRORCODE 23
ISR_NOERRORCODE 24
ISR_NOERRORCODE 25
ISR_NOERRORCODE 26
ISR_NOERRORCODE 27
ISR_NOERRORCODE 28
ISR_NOERRORCODE 29
ISR_NOERRORCODE 30
ISR_NOERRORCODE 31
IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47
ISR_NOERRORCODE 64 ;this is T_SYSCALL

; In isr.c
extern trap
;extern irq_handler

; This is our common ISR stub. It saves the processor state, sets
; up for kernel mode segments, calls the C-level fault handler,
; and finally restores the stack frame.
alltraps:
  ; Build trap frame.
  push ds
  push es
  push fs
  push gs
  pusha

  ; Set up data and per-cpu segments.
  mov ax, 0x10
  mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

  ; Call trap(tf), where tf=%esp
  push esp
  call trap
  add esp, 4

  ; Return falls through to trapret...
global trapret
trapret:
  popa
  pop gs
  pop fs
  pop es
  pop ds
  add esp, 0x8  ; trapno and errcode
  iret