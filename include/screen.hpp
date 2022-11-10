#pragma once
#include <int.hpp>

// VT stands for VGA Text Mode

#define VT_MROWS 25 // row count
#define VT_MCOLS 80 // column count
#define VT_BASE 0xb8000 // base address of text cells
#define VT_CELLS ((u16*)VT_BASE) // for ease of accessing cells as array
#define VT_WHITE_ON_BLACK 0x0f00 // foreground: white, background: black

// reference: http://www.osdever.net/FreeVGA/vga/crtcreg.htm
#define VGA_PORT_CRTC_ADR 0x3d4 // port for CRTC register (address)
#define VGA_IDX_CPOS_HI 0x0e // index of cursor position register (high 8 bits) of CRTC register
#define VGA_IDX_CPOS_LO 0x0f // index of cursor position register (low 8 bits) of CRTC register
#define VGA_PORT_CRTC_DATA 0x3d5 // port for CRTC register (data)

void kputs(const char* src, u16 offset);
void kputs(const char* src, u16 x, u16 y);
void kputs(const char* src);
void kprintf(const char* fmt, ...);
u16 get_cursor_offset();
void set_cursor(u16 offset);
void set_cursor(u16 x, u16 y);
void clear_screen();