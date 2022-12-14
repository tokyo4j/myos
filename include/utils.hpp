#pragma once
#include <int.hpp>
#include <kmalloc.hpp>

#define PANIC(msg)                                                     \
    {                                                                  \
        kprintf("PANIC!: %s at: %s :%d\n", (msg), __FILE__, __LINE__); \
        asm("hlt");                                                    \
    }

#define ASSERT(v)                                                               \
    {                                                                           \
        if (!(v)) {                                                             \
            kprintf("PANIC! %s is false\nat: %s:%d\n", #v, __FILE__, __LINE__); \
            asm("hlt");                                                         \
        }                                                                       \
    }
