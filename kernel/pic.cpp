#include "idt.hpp"
#include <int.hpp>
#include <screen.hpp>
#include <x86.hpp>

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

void init_pic()
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

void pic_eoi(u32 int_no)
{
    if (int_no >= 32) {
        outb(PIC1_COMMAND, PIC_EOI);
        if (int_no >= 40)
            outb(PIC2_COMMAND, PIC_EOI);
    }
}