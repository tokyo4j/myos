#pragma once
#include <int.hpp>

#define KERNEL_OFFSET 0xc0000000
#define PGDIR_SHIFT 22
#define PGTBL_SHIFT 12

extern u32 __KERNEL_OFFSET; // exact value is unknown when compiling. this is verified in _start().
extern char __KSTACK_END;
extern char __KSTACK_START;
extern char __START_CTORS;
extern char __END_CTORS;
extern char __KERNEL_END;

#define P2V(pa) (void*)((u8*)(pa) + KERNEL_OFFSET)
#define V2P(pa) (void*)((u8*)(pa)-KERNEL_OFFSET)
#define PGDOWN(a) ((void*)((u32)a & ~(u32)0xfff))
#define PGUP(a) ((u8*)PGDOWN(a) + PGSIZE)