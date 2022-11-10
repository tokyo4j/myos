C_SOURCES = $(wildcard kernel/*.c)
CPP_SOURCES = $(wildcard kernel/*.cpp)
ASM_SOURCES = kernel/isr_wrapper.asm
OBJS = $(C_SOURCES:.c=.o) $(CPP_SOURCES:.cpp=.o) $(ASM_SOURCES:.asm=.o)
HEADERS = $(wildcard include/*.h) $(wildcard include/*.hpp)
QEMU = qemu-system-i386
QEMUOPTS = \
	-serial mon:stdio \
	-drive file=fs.img,index=0,if=ide,format=raw \
	-drive file=disk.img,index=0,if=floppy,format=raw

CXXFLAGS = -g -O0 -I ./include -masm=intel -nostdlib -ffreestanding -Wall -Wextra -fno-exceptions -fno-rtti

# gcc
# CXX = i386-elf-g++
# LD = i386-elf-ld
# DBG = i386-elf-gdb
# DBGOPTS = \
# 	-ex "target remote localhost:1234"

#clang
CXX = clang++ -target i386
LD = ld.lld
DBG = lldb
DBGOPTS = \
	-o "gdb-remote localhost:1234"

.PHONY: all run debug bdebug clean bochs

all: disk.img kernel/kernel.elf

disk.img: boot/boot.bin kernel/kernel.bin
#	dd if=/dev/zero of=$@ count=2880
	dd if=/dev/zero count=6144 | tr '\0' 'x' > $@
	dd if=boot/boot.bin of=$@ conv=notrunc
	dd if=kernel/kernel.bin of=$@ conv=notrunc seek=2

kernel/kernel.bin: $(OBJS) kernel/kernel.ld fs.img
	$(LD) -T kernel/kernel.ld $(OBJS) -b binary fs.img -o $@ --oformat binary

kernel/kernel.elf: $(OBJS) kernel/kernel.ld
	$(LD) -T kernel/kernel.ld $(OBJS) -b binary fs.img -o $@ --oformat elf32-i386

%.bin: %.asm
	nasm -fbin $< -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.asm
	nasm -f elf32 $< -o $@

run: disk.img
	$(QEMU) $(QEMUOPTS)

debug: disk.img kernel/kernel.elf
#	$(QEMU) -s -S $(QEMUOPTS) & $(DBG) kernel/kernel.elf $(DBGOPTS)
	$(QEMU) -s -S $(QEMUOPTS)

# debug: disk.img kernel/kernel.elf
# 	$(QEMU) -s -S -fda $< &
# 	$(DBG) -ex "target remote localhost:1234" -ex "set disassembly-flavor intel" -ex "b *0x7c00" -ex "layout asm"

clean:
	rm -f */*.bin */*.o */*.elf disk.img fs.img

hoge:
	objdump -b binary -m i386 -M intel -D kernel/kernel.bin

fs.img:
	dd if=/dev/zero of=$@ bs=1M count=1
	sudo losetup /dev/loop0 $@
	sudo mkfs.ext2 -I 128 /dev/loop0
	mkdir mnt
	sudo mount /dev/loop0 mnt
	sudo chmod 777 mnt

	echo "Hello, world" > mnt/hello.txt

	sudo umount mnt
	rm -r mnt
	sudo losetup -d /dev/loop0