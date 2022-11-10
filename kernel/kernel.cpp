#include <int.hpp>
#include <interrupt.hpp>
#include <kmalloc.hpp>
#include <page_alloc.hpp>
#include <pit.hpp>
#include <screen.hpp>
#include <utils.hpp>

extern "C" void kmain() __attribute__((section(".text.entry")));

static void init_static_objs()
{
    using ctor_func_t = void (*)();
    extern ctor_func_t start_ctors;
    extern ctor_func_t end_ctors;
    for (ctor_func_t* p = &start_ctors; p < &end_ctors; p++)
        (*p)();
}

static void print_e820()
{
    struct smap_entry {
        u32 base_lo;
        u32 base_hi;
        u32 len_lo;
        u32 len_hi;
        u32 type;
    } __attribute__((packed));

    smap_entry* entry = (smap_entry*)0xc0008000; // defined in boot.asm
    for (int i = 0; i < 6; i++) {
        kprintf("%08x - %08x %s\n",
            entry[i].base_lo,
            entry[i].base_lo + entry[i].len_lo,
            entry[i].type == 1 ? "Usable" : "Reserved");
    }
}

static void zero_bss()
{
    extern u8 start_bss;
    extern u8 end_bss;
    zeromem(&start_bss, &end_bss - &start_bss);
}

void kmain()
{
    // call static object constructors
    init_static_objs();
    // fill bss section with zero
    zero_bss();

    // mark memory region mapped by early page directory as free
    extern u8 end_kernel;
    free_pages(PGUP(&end_kernel), P2V(0x400000));

    remap_pic();
    init_pit();
    init_gates();
    init_idtr();

    clear_screen();
    set_cursor(0);
    print_e820();

    kprintf("%08x\n", kmalloc(1));
    kprintf("%08x\n", kmalloc(1));
    kprintf("%08x\n", kmalloc(1));
    kprintf("%08x\n", kmalloc(1));

    asm("sti");

    while (1)
        asm("hlt");

    return;
}