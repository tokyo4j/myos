#pragma once
#include <int.hpp>

#define IDT_ENTRY_COUNT 48

struct IDTEntry {
    u16 base_lo;
    u16 cs;
    u8 zero;
    u8 flags;
    u16 base_hi;
} __attribute__((packed));

struct IDTR {
    u16 limit;
    u32 base;
} __attribute__((packed));

struct ISRContext {
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 int_no, err_code;
    u32 eip, cs, eflags, useresp, ss;
} __attribute__((packed));

using InterruptHandler = void (*)(ISRContext*);

void init_idt();
void register_interrupt_handler(int no, InterruptHandler handler);