#pragma once
#include <int.hpp>

static inline u8 inb(u16 port)
{
    u8 result;
    asm volatile("in al, dx"
                 : "=a"(result)
                 : "d"(port));
    return result;
}

static inline void outb(u16 port, u8 data)
{
    asm volatile("out dx, al"
                 :
                 : "a"(data), "d"(port));
}

static inline u16 inw(u16 port)
{
    u16 result;
    asm volatile("in ax, dx"
                 : "=a"(result)
                 : "d"(port));
    return result;
}

static inline void outw(u16 port, u16 data)
{
    asm volatile("out dx, ax"
                 :
                 : "a"(data), "d"(port));
}

static inline void insl(u16 port, void* addr, u32 cnt)
{
    asm volatile("cld; rep insd"
                 :
                 : "d"(port), "D"(addr), "c"(cnt));
}
