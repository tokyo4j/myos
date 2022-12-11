#include <gdt.hpp>
#include <idt.hpp>
#include <int.hpp>
#include <keyboard.hpp>
#include <kmalloc.hpp>
#include <linked_values.hpp>
#include <page_alloc.hpp>
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

class C {
public:
    int x, y;
    C(int x, int y)
        : x(x)
        , y(y)
    {
    }
};

C c(123, 234);

extern "C" void kmain()
{
    // call static object constructors
    init_static_objs();

    clear_screen();
    set_cursor(0);
    kprintf("Hello, world!\n");

    kprintf("%d %d\n", c.x, c.y);

    // mark memory region mapped by early page directory as free
    free_pages(P2V(PGUP(&__KERNEL_END)), P2V(0x400000));

    init_gdt();
    init_idt();
    init_pic();
    init_keyboard();
    init_pit();

    asm("sti");

    while (1)
        asm("hlt");

    return;
}