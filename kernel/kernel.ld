OUTPUT_ARCH(i386)
__KERNEL_OFFSET = 0xc0000000;

SECTIONS
{
    . = 1M;

    .multiboot : {
        KEEP(*(.multiboot))
    }

    .text.boot : {
        *(.text.boot)
    }

    .data.boot : {
        *(.data.boot)
    }

    . += __KERNEL_OFFSET;

    .text ALIGN(4K) : AT (ADDR(.text) - __KERNEL_OFFSET) {
		*(.text*)
	}

	.rodata ALIGN(4K) : AT (ADDR(.rodata) - __KERNEL_OFFSET) {
        __START_CTORS = .;
        *(.ctors*)
        __END_CTORS = .;

		*(.rodata*)
        /* __debug_info_start = .;
        KEEP(*(.debug_info))
        __debug_info_end = .;
        __debug_abbrev_start = .;
        KEEP(*(.debug_abbrev))
        __debug_abbrev_end = .;
        __debug_str_start = .;
        KEEP(*(.debug_str))
        __debug_str_end = .;
        __debug_line_start = .;
        KEEP(*(.debug_line))
        __debug_line_end = .;
        __debug_ranges_start = .;
        KEEP(*(.debug_ranges))
        __debug_ranges_end = .; */
	}

	.data ALIGN(4K) : AT (ADDR(.data) - __KERNEL_OFFSET) {
		*(.data*)
	}

	.bss ALIGN(4K) : AT (ADDR(.bss) - __KERNEL_OFFSET) {
		*(.bss*)
	}

    .bss.stack ALIGN(4K) : AT (ADDR(.bss.stack) - __KERNEL_OFFSET) {
		__KSTACK_START = . - __KERNEL_OFFSET;
        . += 4K;
		__KSTACK_END = . - __KERNEL_OFFSET;
	}

    __KERNEL_END = . - __KERNEL_OFFSET;
}