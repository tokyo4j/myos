#pragma once
#include <int.hpp>

struct slob_header {
    slob_header* ptr;
    u32 size;
};

void kfree(void* ap);
void* kmalloc(u32 nbytes);

static_assert(sizeof(slob_header) == 8, "");