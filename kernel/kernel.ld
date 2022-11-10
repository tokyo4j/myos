OUTPUT_ARCH(i386)
ENTRY(kmain)

SECTIONS
{
    . = 0xc0100000;
    .text ALIGN(0x1000) : {
        *(.text.entry)
		*(.text*)
        *(.gnu.linkonce.t*)
	}

	.rodata ALIGN(0x1000) : {
        start_ctors = .;
        /* *(SORT(.ctors*)) */
        end_ctors = .;

		start_dtors = .;
        /* *(SORT(.dtors*)) */
        end_dtors = .;

		*(.rodata*)
		*(.gnu.linkonce.r.*)
	}

	.data ALIGN(0x1000) : {
		*(.data)
        *(.gnu.linkonce.d*)
	}

	.bss ALIGN(0x1000) : {
        start_bss = .;
		*(.bss)
        *(.gnu.linkonce.b*)
        end_bss = .;
	}

    /DISCARD/ :
    {
        *(.comment)
        *(.eh_frame)
    }

    end_kernel = .;
}