#include <pit.hpp>
#include <screen.hpp>
#include <x86.hpp>

void pit_handler()
{
    static u32 tick = 0;
    static u32 secs = 0;
    tick++;
    if (tick < PIT_FREQ)
        return;

    secs++;
    tick = 0;
    // set_cursor(0, 10);
    kprintf("%d\n", secs);
}

void init_pit()
{
    outb(PIT_COM_PORT, PIT_ACCESS_MODE_LOHI | PIT_RATE_GENERATOR);
    outb(PIT_CH0_PORT, PIT_DIVISOR & 0xff);
    outb(PIT_CH0_PORT, (PIT_DIVISOR >> 8) & 0xff);
}