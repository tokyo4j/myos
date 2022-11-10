BITS 16
ORG 0x7c00

;A20 checking is not implemented, assuming that it is enabled by default in every modern hardware.

jmp boot_real
BPB:
    times 3-($-$$) db 0x90   ; Support 2 or 3 byte encoded JMPs before BPB.

    ; Dos 4.0 EBPB 1.44MB floppy
    OEMname:           db    "mkfs.fat"  ; mkfs.fat is what OEMname mkdosfs uses
    bytesPerSector:    dw    512
    sectPerCluster:    db    1
    reservedSectors:   dw    1
    numFAT:            db    2
    numRootDirEntries: dw    224
    numSectors:        dw    2880
    mediaType:         db    0xf0
    numFATsectors:     dw    9
    sectorsPerTrack:   dw    18
    numHeads:          dw    2
    numHiddenSectors:  dd    0
    numSectorsHuge:    dd    0
    driveNum:          db    0
    reserved:          db    0
    signature:         db    0x29
    volumeID:          dd    0x2d7e5a1a
    volumeLabel:       db    "NO NAME    "
    fileSysType:       db    "FAT12   "

gdt_start:
    dq 0
gdt_code:
    dw 0xffff    ; limit 15-0
    dw 0x0       ; base 15-0
    db 0x0       ; base 23-16
    db 10011010b ; access byte
    db 11001111b ; flags 3-0 + limit 19-16
    db 0x0       ; base 31-24
gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0
gdt_end:
gdt_descriptor:
    dw gdt_end - gdt_start - 1; size
    dd gdt_start ; address
CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

disk_number:
    db 0
H:
    db 0
S:
    db 0

; 7c00-7dff bootloader (512B)
; 7e00-7fff 2nd bootloader (512B), which stores some functions called from 1st bootloader
; 8000-80ff memory map info (512B)
; 8100-8fff stack for bootloader & early kernel (4KB - 512B)
; 9000-9fff early pgdir (4KB)
; a000-a1ff buffer (512B)
; 100000-1fffff kernel (1MB)
; kernel_end (supplied by linker script)-3fffff early kernel heap

SECOND_BOOTLOADER equ 0x7e00
KERNEL_OFFSET equ 0x100000
BUFFER_OFFSET equ 0xa000
; SECTOR_COPY_BATCH_SIZE equ ((KERNEL_OFFSET - BUFFER_OFFSET) >> 9) & 
EARLY_STACK_BASE equ 0x9000
EARLY_PGDIR equ 0x9000
MEMORY_MAP_OFFSET equ 0x8000
V_OFFSET equ 0xc0000000
PDXSHIFT equ 22
KERNEL_SECTOR_COUNT equ 6144

boot_real:
    mov [disk_number], dl
    mov eax, 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, EARLY_STACK_BASE
    mov ss, ax
    mov esp, ebp

    ;load 2nd bootloader
    mov ah, 2; read function
    mov al, 1; 1 sector
    mov ch, 0; cylinder 0
    mov cl, 2; sector 2
    mov dh, 0; head 0
    mov dl, [disk_number]; disk number
    mov bx, SECOND_BOOTLOADER
    int 0x13

    ; get disk geometry and save to H,S
    mov ah, 8
    mov dl, [disk_number]
    int 0x13
    inc dh
    mov [H], dh
    and cl, 0x3f
    mov [S], cl
    ;int 0x13 ah=08 might modify es
    mov ax, 0
    mov es, ax

    ;get memory map to MEMORY_MAP_OFFSET
    mov ebx, 0
    mov di, MEMORY_MAP_OFFSET
e820_loop:
    mov eax, 0xe820
    mov ecx, 20
    mov edx, 0x0534D4150
    int 0x15
    add di, 20
    cmp ebx, 0
    jne e820_loop

    cli
    ;unreal mode
    push ds
    push es
    lgdt [gdt_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    mov bx, DATA_SEG
    mov ds, bx
    mov es, bx
    and al, 0xfe
    mov cr0, eax
    pop es
    pop ds
    sti

    mov ecx, 0
.load_sector_loop:
    call copy_sector
    inc ecx
    cmp ecx, KERNEL_SECTOR_COUNT
    jne .load_sector_loop

    cli
    ;protected mode
    lgdt [gdt_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE_SEG:boot32

BITS 32
boot32:
    sti
    mov eax, DATA_SEG
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    mov ebp, EARLY_STACK_BASE
    mov esp, ebp

; set up early page directory
    mov edi, EARLY_PGDIR
.loop_fillzero:
    mov [edi], dword 0
    add edi, 4
    cmp edi, EARLY_PGDIR + 0x1000
    jne .loop_fillzero

    mov edi, EARLY_PGDIR
    mov [edi], dword 0b1000_0011

    add edi, (V_OFFSET >> PDXSHIFT) * 4
    mov [edi], dword 0b1000_0011

    add edi, 4
    mov [edi], dword ((1 << 22) | 0b1000_0011)

    cli
    ;enable PSE
    mov eax, cr4
    or eax, 0x00000010
    mov cr4, eax
    ;set early paging directory
    mov eax, EARLY_PGDIR
    mov cr3, eax
    ;enable paging
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax

    cli
    mov ebp, V_OFFSET + EARLY_STACK_BASE
    mov esp, ebp
    call V_OFFSET + KERNEL_OFFSET
times 510-($-$$) db 0
dw 0xaa55

;2nd bootloader
BITS 16
;for debugging
;input: edx=value
print_hex:
    pushad
    mov bh, 0x00
    mov bl, 0x07
    mov cx, 8
.loop:
    mov eax, edx
    shr eax, 28
    cmp al, 10
    jge .alphabet
.num:
    add al, "0"
    jmp .print
.alphabet:
    add al, "a" - 10
    jmp .print
.print:
    mov ah, 0x0e
    int 0x10
    shl edx, 4
    loop .loop
.done:
    mov al, " "
    int 0x10
    popad
    ret

;for debugging
print_nl:
    pusha
    mov bx, 0x07
    mov ah, 0x0e
    mov al, 0x0d
    int 0x10
    mov al, 0x0a
    int 0x10
    popa
    ret

;input: edx=LBA
;return: ch=Cylinder, cl=Sector, dh=Head
lba2chs:
    ;perform LBA / S
    mov eax, edx
    mov edx, 0
    mov ecx, 0
    mov cl, [S]
    div ecx
    ;TI(Track Index) = (LBA/S) = eax
    ;Sector-1 = (LBA%S) = edx
    inc edx
    mov bx, dx ; store Sector in bx

    ;perform TI/H
    mov edx, 0
    mov ecx, 0
    mov cl, [H]
    div ecx
    ;Cylinder = (TI/H) = ax
    ;Head = (TI%H) = dx
    mov ch, al
    mov cl, bl
    mov dh, dl
    mov dl, 0
    ret

;input: ecx=Sector offset (ecx=0 means sector 3 (LBA=2))
copy_sector:
    pushad
    push ecx
    mov edx, ecx
    add edx, 2
    call lba2chs

    mov dl, [disk_number]
    mov bx, BUFFER_OFFSET
    mov al, 1 ;1 sector
    mov ah, 2 ;disk read BIOS interrupt
    int 0x13
    pop ecx

    ;copy buffer to kernel area (1MB~)
    mov esi, BUFFER_OFFSET
    mov edi, KERNEL_OFFSET
    shl ecx, 9
    add edi, ecx
    mov ecx, 0x80
    a32 rep movsd

    popad
    ret

times 1024-($-$$) db 0