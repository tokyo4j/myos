#pragma once
#include <int.hpp>

#define IDT_ENTRY_COUNT 48
#define IDT_GATE_INT_32 0xe
#define IDT_GATE_TRAP_32 0xf
#define IDT_RING_0 0
#define IDT_RING_3 (3 << 4)
#define IDT_PRESENT (1 << 7)

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xa0
#define PIC2_DATA 0xa1
#define ICW1_ICW4 0x01
#define ICW1_SINGLE 0x02
#define ICW1_INTERVAL4 0x04
#define ICW1_LEVEL 0x08
#define ICW1_INIT 0x10
#define ICW4_8086 0x01
#define ICW4_AUTO 0x02
#define ICW4_BUF_SLAVE 0x08
#define ICW4_BUF_MASTER 0x0c
#define ICW4_SFNM 0x10
#define PIC_EOI 0x20

struct idt_entry_32_t {
    u16 base_lo;
    u16 cs;
    u8 zero;
    u8 flags;
    u16 base_hi;
} __attribute__((packed));

struct idtr_32_t {
    u16 limit;
    u32 base;
} __attribute__((packed));

struct isr_ctx_t {
    u32 ds;
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;
    u32 int_no, err_code;
    u32 eip, cs, eflags, useresp, ss;
} __attribute__((packed));

void set_gate_32(u8 entry_index, void* isr, u16 cs, u8 flags);
void init_gates();
void init_idtr();
void remap_pic();

extern "C" void isr_global_handler(isr_ctx_t ctx);

// implemented in isr_wrapper.asm
extern "C" {
void isr_stub_0();
void isr_stub_1();
void isr_stub_2();
void isr_stub_3();
void isr_stub_4();
void isr_stub_5();
void isr_stub_6();
void isr_stub_7();
void isr_stub_8();
void isr_stub_9();
void isr_stub_10();
void isr_stub_11();
void isr_stub_12();
void isr_stub_13();
void isr_stub_14();
void isr_stub_15();
void isr_stub_16();
void isr_stub_17();
void isr_stub_18();
void isr_stub_19();
void isr_stub_20();
void isr_stub_21();
void isr_stub_22();
void isr_stub_23();
void isr_stub_24();
void isr_stub_25();
void isr_stub_26();
void isr_stub_27();
void isr_stub_28();
void isr_stub_29();
void isr_stub_30();
void isr_stub_31();
void isr_stub_32();
void isr_stub_33();
void isr_stub_34();
void isr_stub_35();
void isr_stub_36();
void isr_stub_37();
void isr_stub_38();
void isr_stub_39();
void isr_stub_40();
void isr_stub_41();
void isr_stub_42();
void isr_stub_43();
void isr_stub_44();
void isr_stub_45();
void isr_stub_46();
void isr_stub_47();
}