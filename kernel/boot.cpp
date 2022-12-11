#include <int.hpp>
#include <linked_values.hpp>

extern "C" void _start() __attribute__((section(".text.boot"))) __attribute__((naked));

u32 early_pgdir[1024] __attribute__((section(".data.boot"))) __attribute__((aligned(4096))) = {
    [0] = 0b1000'0011,
    [0xc0000000 >> 22] = 0b1000'0011,
};

u32 eax __attribute__((section(".data.boot")));
u32 ebx __attribute__((section(".data.boot")));

extern "C" void kmain();

void _start()
{
    asm volatile(""
                 : "=a"(eax));
    asm volatile(""
                 : "=b"(ebx));

    // enable PSE
    asm volatile(
        "movl %%cr4, %%eax\n\t"
        "orl $0x00000010, %%eax\n\t"
        "movl %%eax, %%cr4\n\t" ::
            : "eax");

    // set page directory
    asm volatile(
        "movl %0, %%cr3\n\t"
        :
        : "a"(&early_pgdir));

    // enable paging
    asm volatile(
        "movl %%cr0, %%eax\n\t"
        "orl $0x80000000, %%eax\n\t"
        "movl %%eax, %%cr0\n\t" ::
            : "eax");

    asm volatile(
        "movl %0, %%esp\n\t"
        "addl $0xc0000000, %%esp\n\t"
        "subl $32, %%esp\n\t"
        "movl %%esp, %%ebp\n\t" ::"r"(&__KSTACK_END));

    asm volatile("call kmain");
}