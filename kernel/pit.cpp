#include <idt.hpp>
#include <pic.hpp>
#include <pit.hpp>
#include <screen.hpp>
#include <x86.hpp>

#define PIT_ACCESS_MODE_LOHI 0b00110000
#define PIT_RATE_GENERATOR 0b00000100
#define PIT_BASE_FREQ 1193182
#define PIT_FREQ 1000
#define PIT_DIVISOR ((u16)(PIT_BASE_FREQ / PIT_FREQ))
#define PIT_CH0_PORT 0x40
#define PIT_CH1_PORT 0x41
#define PIT_CH2_PORT 0x42
#define PIT_COM_PORT 0x43

static void pit_handler(ISRContext* ctx)
{
    static u32 tick = 0;
    static u32 secs = 0;
    tick++;
    if (tick >= PIT_FREQ) {
        secs++;
        tick = 0;
        // set_cursor(0, 10);
        kprintf("%d\n", secs);
    }
    pic_eoi(ctx->int_no);
}

void init_pit()
{
    outb(PIT_COM_PORT, PIT_ACCESS_MODE_LOHI | PIT_RATE_GENERATOR);
    outb(PIT_CH0_PORT, PIT_DIVISOR & 0xff);
    outb(PIT_CH0_PORT, (PIT_DIVISOR >> 8) & 0xff);

    register_interrupt_handler(32, pit_handler);
}