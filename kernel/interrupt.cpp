#include <interrupt.hpp>
#include <pit.hpp>
#include <screen.hpp>
#include <x86.hpp>

static idtr_32_t idtd;
static idt_entry_32_t idt[IDT_ENTRY_COUNT];

#define NUM_ISR 48
static void (*isr_stubs[NUM_ISR])() = {
    isr_stub_0,
    isr_stub_1,
    isr_stub_2,
    isr_stub_3,
    isr_stub_4,
    isr_stub_5,
    isr_stub_6,
    isr_stub_7,
    isr_stub_8,
    isr_stub_9,
    isr_stub_10,
    isr_stub_11,
    isr_stub_12,
    isr_stub_13,
    isr_stub_14,
    isr_stub_15,
    isr_stub_16,
    isr_stub_17,
    isr_stub_18,
    isr_stub_19,
    isr_stub_20,
    isr_stub_21,
    isr_stub_22,
    isr_stub_23,
    isr_stub_24,
    isr_stub_25,
    isr_stub_26,
    isr_stub_27,
    isr_stub_28,
    isr_stub_29,
    isr_stub_30,
    isr_stub_31,
    isr_stub_32,
    isr_stub_33,
    isr_stub_34,
    isr_stub_35,
    isr_stub_36,
    isr_stub_37,
    isr_stub_38,
    isr_stub_39,
    isr_stub_40,
    isr_stub_41,
    isr_stub_42,
    isr_stub_43,
    isr_stub_44,
    isr_stub_45,
    isr_stub_46,
    isr_stub_47
};

void set_gate(u8 entry_index, void (*isr)(), u16 cs, u8 flags)
{
    idt_entry_32_t* entry_ptr = &idt[entry_index];

    entry_ptr->base_lo = (u32)isr & 0xffff;
    entry_ptr->cs = cs;
    entry_ptr->zero = 0;
    entry_ptr->flags = flags;
    entry_ptr->base_hi = ((u32)isr >> 16) & 0xffff;
}

void isr_global_handler(isr_ctx_t ctx)
{
    // kprintf("no: %d error code: %d\n", ctx.int_no, ctx.err_code);

    switch (ctx.int_no) {
    case 32: {
        pit_handler();
        break;
    }
    case 33: {
        u8 code = inb(0x60);
        kprintf("Keyboard Input: %x\n", code);
        break;
    }
    }

    if (ctx.int_no >= 32) {
        outb(PIC1_COMMAND, PIC_EOI);
        if (ctx.int_no >= 40)
            outb(PIC2_COMMAND, PIC_EOI);
    }
}

void init_gates()
{
    set_gate(0, isr_stub_0, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(1, isr_stub_1, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(2, isr_stub_2, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(3, isr_stub_3, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(4, isr_stub_4, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(5, isr_stub_5, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(6, isr_stub_6, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(7, isr_stub_7, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(8, isr_stub_8, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(9, isr_stub_9, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(10, isr_stub_10, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(11, isr_stub_11, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(12, isr_stub_12, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(13, isr_stub_13, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(14, isr_stub_14, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);
    set_gate(15, isr_stub_15, 0x08, IDT_PRESENT | IDT_GATE_TRAP_32);

    set_gate(16, isr_stub_16, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(17, isr_stub_17, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(18, isr_stub_18, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(19, isr_stub_19, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(20, isr_stub_20, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(21, isr_stub_21, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(22, isr_stub_22, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(23, isr_stub_23, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(24, isr_stub_24, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(25, isr_stub_25, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(26, isr_stub_26, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(27, isr_stub_27, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(28, isr_stub_28, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(29, isr_stub_29, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(30, isr_stub_30, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(31, isr_stub_31, 0x08, IDT_PRESENT | IDT_GATE_INT_32);

    set_gate(32, isr_stub_32, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(33, isr_stub_33, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(34, isr_stub_34, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(35, isr_stub_35, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(36, isr_stub_36, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(37, isr_stub_37, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(38, isr_stub_38, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(39, isr_stub_39, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(40, isr_stub_40, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(41, isr_stub_41, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(42, isr_stub_42, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(43, isr_stub_43, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(44, isr_stub_44, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(45, isr_stub_45, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(46, isr_stub_46, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
    set_gate(47, isr_stub_47, 0x08, IDT_PRESENT | IDT_GATE_INT_32);
}

void remap_pic()
{
    // ICW1
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);

    // ICW2
    // set PIC vector offset
    outb(PIC1_DATA, 32);
    outb(PIC2_DATA, 40);

    // ICW3
    // IR 2 is connected to slave
    outb(PIC1_DATA, 0b00000100); // bit 2 is on
    outb(PIC2_DATA, 2);

    // ICW4
    // 8086/88 mode
    outb(PIC1_DATA, ICW4_8086);
    outb(PIC2_DATA, ICW4_8086);

    // unmask all
    outb(PIC1_DATA, 0b00000000);
    outb(PIC2_DATA, 0);
}

void init_idtr()
{
    idtd.limit = (u16)sizeof(idt) - 1;
    idtd.base = (u32)&idt;

    asm("lidt [%0]" ::"r"(&idtd));
}
