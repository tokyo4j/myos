#pragma once
#include <int.hpp>

#define IDT_ENTRY_COUNT 48
#define IDT_GATE_INT_32 0xe
#define IDT_GATE_TRAP_32 0xf
#define IDT_RING_0 0
#define IDT_RING_3 (3 << 4)
#define IDT_PRESENT (1 << 7)

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