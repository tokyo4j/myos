#include <constants.hpp>
#include <gdt.hpp>
#include <idt.hpp>
#include <int.hpp>
#include <keyboard.hpp>
#include <kmalloc.hpp>
#include <page_alloc.hpp>
#include <pci.hpp>
#include <pic.hpp>
#include <pit.hpp>
#include <screen.hpp>
#include <utils.hpp>

static void init_static_objs()
{
    using ctor_func_t = void (*)();

    for (ctor_func_t* p = (ctor_func_t*)&__START_CTORS; p < (ctor_func_t*)&__END_CTORS; p++)
        (*p)();
}

const char* str = "hello, world";

struct C {
    const char *x, *y;
    C(const char* x, const char* y)
        : x(x)
        , y(y)
    {
    }
};
C c("hello", "world");

extern "C" void kmain(void* mb2_structure_ptr)
{
    // call static object constructors
    init_static_objs();

    clear_screen();
    set_cursor(0);
    kprintf("%s, %s\n", c.x, c.y);

    free_pages(P2V(PGUP(&__KERNEL_END)), P2V(0x4000000));

    init_gdt();
    init_idt();
    init_pic();
    init_keyboard();
    init_pit();
    init_pci();

    kprintf("%08x\n", u32(new int));
    kprintf("%08x\n", u32(new int));
    kprintf("%08x\n", u32(new int));
    kprintf("%08x\n", u32(new int));

    // asm("hlt");
    asm("sti");

    while (1)
        asm("hlt");

    return;
}