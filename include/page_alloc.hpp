#pragma once
#include <utils.hpp>

#define PGSIZE 0x1000

void free_page(void* va);
void free_pages(void* va_start, void* va_end);
void* alloc_page();