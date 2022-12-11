#pragma once

#include <int.hpp>
#include <utils.hpp>

struct GDTEntry {
    u16 limit_lo;
    u16 base_lo;
    u8 base_mid;
    u8 access;
    u8 flags_limit_hi;
    u8 base_hi;
};

struct GDTR {
    u16 size;
    u32 addr;
} __attribute((packed));

void init_gdt();