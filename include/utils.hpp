#pragma once

void zeromem(void* dst, int n);

#define PANIC(msg)                                    \
    {                                                 \
        kputs("PANIC!: " #msg "\nat: " __FILE__ ":"); \
        kputd(__LINE__);                              \
        asm("hlt");                                   \
    }

#define ASSERT(v)                                                               \
    {                                                                           \
        if (!(v)) {                                                             \
            kprintf("PANIC! %s is false\nat: %s:%d\n", #v, __FILE__, __LINE__); \
            asm("hlt");                                                         \
        }                                                                       \
    }

#define KERNEL_BASE 0xc0000000

#define P2V(pa) ((u8*)(pa) + KERNEL_BASE)
#define PGDOWN(a) ((void*)((u32)a & ~(u32)0xfff))
#define PGUP(a) ((u8*)PGDOWN(a) + PGSIZE)

struct run_node {
    run_node* next;
};

class run {
private:
    run_node* head;

public:
    void push(run_node* new_node);
    run_node* pop();
};