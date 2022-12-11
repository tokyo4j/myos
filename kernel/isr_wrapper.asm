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
isr_stub_%1:
    cli
    push %1
    jmp isr_common
%endmacro

%macro isr_stub_noerr 1
isr_stub_%1:
    cli
    push 0
    push %1
    jmp isr_common
%endmacro

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

section .data
global isr_stubs
isr_stubs:
    dd isr_stub_0
    dd isr_stub_1
    dd isr_stub_2
    dd isr_stub_3
    dd isr_stub_4
    dd isr_stub_5
    dd isr_stub_6
    dd isr_stub_7
    dd isr_stub_8
    dd isr_stub_9
    dd isr_stub_10
    dd isr_stub_11
    dd isr_stub_12
    dd isr_stub_13
    dd isr_stub_14
    dd isr_stub_15
    dd isr_stub_16
    dd isr_stub_17
    dd isr_stub_18
    dd isr_stub_19
    dd isr_stub_20
    dd isr_stub_21
    dd isr_stub_22
    dd isr_stub_23
    dd isr_stub_24
    dd isr_stub_25
    dd isr_stub_26
    dd isr_stub_27
    dd isr_stub_28
    dd isr_stub_29
    dd isr_stub_30
    dd isr_stub_31
    dd isr_stub_32
    dd isr_stub_33
    dd isr_stub_34
    dd isr_stub_35
    dd isr_stub_36
    dd isr_stub_37
    dd isr_stub_38
    dd isr_stub_39
    dd isr_stub_40
    dd isr_stub_41
    dd isr_stub_42
    dd isr_stub_43
    dd isr_stub_44
    dd isr_stub_45
    dd isr_stub_46
    dd isr_stub_47