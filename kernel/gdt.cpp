#include "x86.hpp"
#include <constants.hpp>
#include <gdt.hpp>

static GDTEntry gdt[3] = {
    // null
    {},
    // code
    {
        .limit_lo = 0xffff,
        .base_lo = 0,
        .base_mid = 0,
        .access = 0b1001'1010,
        .flags_limit_hi = 0b1100'1111,
        .base_hi = 0,
    },
    // data
    {
        .limit_lo = 0xffff,
        .base_lo = 0,
        .base_mid = 0,
        .access = 0b1001'0010,
        .flags_limit_hi = 0b1100'1111,
        .base_hi = 0,
    }
};

static GDTR gdtr = {
    .size = sizeof(gdt) - 1,
    .addr = (u32)(&gdt),
};

void init_gdt()
{
    asm volatile("lgdt (%0)" ::"r"(&gdtr));
    asm volatile("movw $0x10, %%ax\n\t"
                 "movw %%ax, %%ds\n\t"
                 "movw %%ax, %%es\n\t"
                 "movw %%ax, %%fs\n\t"
                 "movw %%ax, %%gs\n\t"
                 "movw %%ax, %%ss\n\t"
                 "jmp $0x08, $done\n\t"
                 "done: nop\n\t" ::
                     : "ax");
}