#include <int.hpp>
#include <x86.hpp>

void read_sector_busy(void* buf, u32 offset)
{
    outb(0x1F2, 1);
    outb(0x1F3, offset);
    outb(0x1F4, offset >> 8);
    outb(0x1F5, offset >> 16);
    outb(0x1F6, (offset >> 24) | 0xE0);
    outb(0x1F7, 0x20);
    while ((inb(0x1F7) & 0xC0) != 0x40)
        ;
    insl(0x1F0, buf, 512 / 4);
}