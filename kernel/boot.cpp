#include <constants.hpp>
#include <int.hpp>

static u32 early_pgdir[1024] __attribute__((section(".data.boot"), aligned(4096)));

static u32 eax __attribute__((section(".data.boot")));
static u32 ebx __attribute__((section(".data.boot")));

extern "C" void kmain(void* mb2_structure_ptr);

static void jmp_to_kmain()
{
    early_pgdir[0] = 0;
    asm volatile("invlpg (%0)" ::"r"(0));

    // u32 x = ebx;

    kmain(*(void**)(P2V(&ebx)));
}

extern "C" void __attribute__((section(".text.boot"))) _start()
{
    asm volatile(""
                 : "=a"(eax));
    asm volatile(""
                 : "=b"(ebx));
    if (eax != 0x36d76289)
        asm volatile("hlt");

    if (KERNEL_OFFSET != (u32)&__KERNEL_OFFSET)
        asm volatile("hlt");

    early_pgdir[0] = 0b1000'0011;
    for (u32 i = KERNEL_OFFSET >> PGDIR_SHIFT; i < 0x1'0000'0000 >> PGDIR_SHIFT; i++) {
        u32 frame_index = i - (KERNEL_OFFSET >> PGDIR_SHIFT);
        early_pgdir[i] = (frame_index << PGDIR_SHIFT) | 0b1000'0011;
    }

    // enable PSE
    asm volatile(
        "movl %%cr4, %%eax\n\t"
        "orl $0x00000010, %%eax\n\t"
        "movl %%eax, %%cr4\n\t"
        :
        :
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
        "movl %%eax, %%cr0\n\t"
        :
        :
        : "eax");

    // initialize esp & ebp
    asm volatile(
        "movl %0, %%esp\n\t"
        "subl $32, %%esp\n\t"
        "movl %%esp, %%ebp\n\t"
        :
        : "r"(P2V(&__KSTACK_END)));

    jmp_to_kmain();
}
