#pragma once
#include <int.hpp>

void kputs(const char* src, u16 offset);
void kputs(const char* src, u16 x, u16 y);
void kputs(const char* src);
void kprintf(const char* fmt, ...);
u16 get_cursor_offset();
void set_cursor(u16 offset);
void set_cursor(u16 x, u16 y);
void clear_screen();