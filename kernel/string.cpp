#include <int.hpp>
#include <string.hpp>

enum format_type {
    CHAR = 0,
    DECIMAL = 1,
    HEX = 2,
    STRING = 3,
    PERCENT = 4
};

struct spec {
    int zero_padding;
    int width;
    format_type type;
};

static int copy_str(char* dst, const char* src)
{
    int read = 0;
    while ((*dst++ = *src++) != 0)
        if (++read > 256)
            break;
    return read;
}

static int int2str(char* dst, int src, int width, int zero_padding)
{
    char buf[11];
    buf[10] = 0;
    int i;
    for (i = 9; src; i--) {
        buf[i] = '0' + src % 10;
        src /= 10;
    }
    if (i == 9)
        buf[i--] = '0';
    if (width) {
        char c = zero_padding ? '0' : ' ';
        for (; 9 - i < width; i--) {
            buf[i] = c;
        }
    }
    return copy_str(dst, buf + i + 1);
}

static int uint2hexstr(char* dst, u32 src, int width, int zero_padding)
{
    char buf[9];
    buf[8] = 0;
    int i;
    for (i = 7; src; i--) {
        char nibble = src & 0xf;
        buf[i] = nibble < 10 ? nibble + '0' : nibble - 10 + 'a';
        src >>= 4;
    }
    if (i == 7)
        buf[i--] = '0';
    if (width) {
        char c = zero_padding ? '0' : ' ';
        for (; 7 - i < width; i--) {
            buf[i] = c;
        }
    }
    return copy_str(dst, buf + i + 1);
}

static int str2int(int* dst, const char* src)
{
    int w = 0;
    int read = 0;
    char c;
    while (1) {
        c = *src++;
        if (!('0' <= c && c <= '9'))
            break;
        read++;
        w *= 10;
        w += c - '0';
    }
    *dst = w;
    return read;
}

static int read_spec(struct spec* spec, const char* src)
{
    int read = 0;

    spec->zero_padding = 0;
    spec->width = 0;
    if (*src == '0') {
        spec->zero_padding = 1;
        src++;
        read++;
    }
    if ('1' <= *src && *src <= '9') {
        int r = str2int(&spec->width, src);
        src += r;
        read += r;
    }
    switch (*src) {
    case '%':
        spec->type = PERCENT;
        break;
    case 'c':
        spec->type = CHAR;
        break;
    case 's':
        spec->type = STRING;
        break;
    case 'd':
        spec->type = DECIMAL;
        break;
    case 'x':
        spec->type = HEX;
        break;
    }

    return ++read;
}

int vsprintf(char* dst, const char* fmt, va_list arg)
{
    char c;
    int length = 0;

    while ((c = *fmt++)) {
        if (c == '%') {
            struct spec spec;
            fmt += read_spec(&spec, fmt);
            switch (spec.type) {
            case PERCENT:
                *dst++ = '%';
                length++;
                break;
            case CHAR:
                *dst++ = va_arg(arg, int);
                length++;
                break;
            case STRING: {
                int written = copy_str(dst, va_arg(arg, char*));
                dst += written;
                length += written;
                break;
            }
            case DECIMAL: {
                int written = int2str(dst, va_arg(arg, int), spec.width, spec.zero_padding);
                dst += written;
                length += written;
                break;
            }
            case HEX: {
                int written = uint2hexstr(dst, va_arg(arg, int), spec.width, spec.zero_padding);
                dst += written;
                length += written;
                break;
            }
            }
        } else {
            *dst++ = c;
            length++;
        }
    }
    *dst = 0;
    return ++length;
}

void sprintf(char* dst, const char* fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf(dst, fmt, ap);
    va_end(ap);
}