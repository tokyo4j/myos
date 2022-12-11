#include <idt.hpp>
#include <int.hpp>
#include <pic.hpp>
#include <screen.hpp>
#include <x86.hpp>

static void keyboard_handler(ISRContext* ctx)
{
    u8 code = inb(0x60);
    kprintf("Keyboard Input: %x\n", code);

    pic_eoi(ctx->int_no);
}

void init_keyboard()
{
    register_interrupt_handler(33, keyboard_handler);
}