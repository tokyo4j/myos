BITS 32

extern isr_global_handler

section .text

isr_common:
    pusha
    mov ax, ds
    push eax
    mov eax, 0x10
    mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

    call isr_global_handler

	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	popa

    add esp, 8
    sti
    iret

%macro isr_stub_err 1
global isr_stub_%1
isr_stub_%1:
    cli
    push %1
    jmp isr_common
%endmacro

%macro isr_stub_noerr 1
global isr_stub_%1
isr_stub_%1:
    cli
    push 0
    push %1
    jmp isr_common
%endmacro

;these defines isr_stub_[0~47] globally

;exceptions by CPU
isr_stub_noerr 0
isr_stub_noerr 1
isr_stub_noerr 2
isr_stub_noerr 3
isr_stub_noerr 4
isr_stub_noerr 5
isr_stub_noerr 6
isr_stub_noerr 7
isr_stub_err 8
isr_stub_noerr 9
isr_stub_err 10
isr_stub_err 11
isr_stub_err 12
isr_stub_err 13
isr_stub_err 14
isr_stub_noerr 15
isr_stub_noerr 16
isr_stub_err 17
isr_stub_noerr 18
isr_stub_noerr 19
isr_stub_noerr 20
isr_stub_noerr 21
isr_stub_noerr 22
isr_stub_noerr 23
isr_stub_noerr 24
isr_stub_noerr 25
isr_stub_noerr 26
isr_stub_noerr 27
isr_stub_noerr 28
isr_stub_noerr 29
isr_stub_err 30
isr_stub_noerr 31

;PIC interrupt handlers
isr_stub_noerr 32 ;irq 0
isr_stub_noerr 33 ;irq 1
isr_stub_noerr 34 ;irq 2
isr_stub_noerr 35 ;irq 3
isr_stub_noerr 36 ;irq 4
isr_stub_noerr 37 ;irq 5
isr_stub_noerr 38 ;irq 6
isr_stub_noerr 39 ;irq 7
isr_stub_noerr 40 ;irq 8
isr_stub_noerr 41 ;irq 9
isr_stub_noerr 42 ;irq 10
isr_stub_noerr 43 ;irq 11
isr_stub_noerr 44 ;irq 12
isr_stub_noerr 45 ;irq 13
isr_stub_noerr 46 ;irq 14
isr_stub_noerr 47 ;irq 15
