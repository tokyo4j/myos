#include <int.hpp>
#include <screen.hpp>
#include <string.hpp>
#include <va_list.hpp>
#include <x86.hpp>

void kputs(const char* src, u16 offset)
{
    int i; // index of string
    int w_offset; // offset of currently writing cell
    for (i = 0, w_offset = offset;; i++, w_offset++) {
        if (w_offset >= VT_MCOLS * VT_MROWS) {
            int j = 0;
            // move rows by 1
            for (j = 0; j < VT_MCOLS * (VT_MROWS - 1); j++)
                VT_CELLS[j] = VT_CELLS[j + VT_MCOLS];
            // clear last row
            for (; j < VT_MCOLS * VT_MROWS; j++)
                VT_CELLS[j] = VT_WHITE_ON_BLACK | ' ';
            w_offset = VT_MCOLS * (VT_MROWS - 1);
        }

        char c = src[i];

        if (c == 0)
            break;

        if (c == '\n') {
            w_offset += VT_MCOLS;
            w_offset = VT_MCOLS * (w_offset / VT_MCOLS) - 1;
            continue;
        }

        VT_CELLS[w_offset]
            = VT_WHITE_ON_BLACK | c;
    }
    set_cursor(w_offset);
}

void kputs(const char* src, u16 x, u16 y)
{
    kputs(src, x + VT_MCOLS * y);
}

void kputs(const char* src)
{
    kputs(src, get_cursor_offset());
}

void vkprintf(const char* fmt, va_list arg)
{
    char buf[256];
    vsprintf(buf, fmt, arg);
    kputs(buf);
}

void kprintf(const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vkprintf(fmt, ap);
    va_end(ap);
}

u16 get_cursor_offset()
{
    outb(VGA_PORT_CRTC_ADR, VGA_IDX_CPOS_HI);
    u16 offset = inb(VGA_PORT_CRTC_DATA) << 8;
    outb(VGA_PORT_CRTC_ADR, VGA_IDX_CPOS_LO);
    offset |= inb(VGA_PORT_CRTC_DATA);
    return offset;
}

void set_cursor(u16 offset)
{
    outb(VGA_PORT_CRTC_ADR, VGA_IDX_CPOS_HI);
    outb(VGA_PORT_CRTC_DATA, offset >> 8);
    outb(VGA_PORT_CRTC_ADR, VGA_IDX_CPOS_LO);
    outb(VGA_PORT_CRTC_DATA, offset & 0xff);
}

void set_cursor(u16 x, u16 y)
{
    set_cursor(x + VT_MCOLS * y);
}

void clear_screen()
{
    for (int i = 0; i < VT_MCOLS * VT_MROWS; i++)
        VT_CELLS[i] = VT_WHITE_ON_BLACK | ' ';
}
