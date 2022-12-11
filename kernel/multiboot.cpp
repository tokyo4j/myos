#include <int.hpp>

struct MB2HeaderFixed {
    u32 magic;
    u32 architecture;
    u32 header_length;
    u32 checksum;
};

struct MB2HeaderHead {
    u16 type;
    u16 flags;
    u32 size;
};

struct MB2Header {
    MB2HeaderFixed fixed __attribute__((aligned(8)));
    MB2HeaderHead end __attribute__((aligned(8)));
};

MB2Header mb2_header __attribute__((section(".multiboot"))) = {
    .fixed = {
        .magic = 0xe85250d6,
        .architecture = 0,
        .header_length = sizeof(MB2HeaderFixed),
        .checksum = 0x1'00000000 - (0xe85250d6 + 0 + sizeof(MB2HeaderFixed)),
    },
    .end = {
        .type = 0,
        .flags = 0,
        .size = sizeof(MB2HeaderHead),
    },
};