C_SOURCES = $(wildcard kernel/*.c)
CPP_SOURCES = $(wildcard kernel/*.cpp)
ASM_SOURCES = kernel/isr_wrapper.asm
OBJS = $(C_SOURCES:.c=.o) $(CPP_SOURCES:.cpp=.o) $(ASM_SOURCES:.asm=.o)
HEADERS = $(wildcard include/*.h) $(wildcard include/*.hpp)
# QEMUOPTS = \
# 	-serial mon:stdio \
# 	-drive filos.iso,index=0,if=ide,format=raw \
# 	-drive file=os.iso,index=0,if=floppy,format=raw

CXXFLAGS = \
	-g \
	-O0 \
	-I./include \
	-nostdlib \
	-ffreestanding \
	-Wall \
	-Wextra \
	-fno-exceptions \
	-fno-rtti \
	-std=gnu++20

# gcc
# CXX = i386-elf-g++
# LD = i386-elf-ld
# CXXFLAGS += -Wno-sized-deallocation

#clang
CXX = clang++
CXXFLAGS += -target i386 -fno-use-init-array -Wno-c99-designator
LD = ld.lld

.PHONY: all run debug bdebug clean bochs

all: os.iso kernel.elf

qemu: os.iso
	qemu-system-i386 -drive format=raw,file=$^

qemud: os.iso
	qemu-system-i386 -drive format=raw,file=$^ -s -S
#	lldb kernel.elf --one-liner "gdb-remote localhost:1234"
#	gdb kernel.elf -ex "target remote localhost:1234"

VM_FOLDER = ./vm
VM_NAME = MYOS
IDE_CONTROLLER_NAME = "SATA Controller"
vm: os.iso
	mkdir -p $(VM_FOLDER)
	VBoxManage createvm --name $(VM_NAME) --ostype Ubuntu20_LTS_64 --basefolder $(VM_FOLDER) --register
	VBoxManage modifyvm $(VM_NAME) --cpus 2 --memory 2048 --vram 12 --graphicscontroller vmsvga
	VBoxManage storagectl $(VM_NAME) --name $(IDE_CONTROLLER_NAME) --add ide
	VBoxManage storageattach $(VM_NAME) --storagectl $(IDE_CONTROLLER_NAME) --port 0  --device 0 --type dvddrive --medium os.iso
vbox:
	VBoxManage startvm $(VM_NAME)

os.iso: kernel.elf grubroot/boot/grub/grub.cfg
	cp kernel.elf grubroot/boot/
	grub-mkrescue -o $@ grubroot/

kernel.elf: $(OBJS) kernel/kernel.ld
	$(LD) -T kernel/kernel.ld $(OBJS) -o $@

%.bin: %.asm
	nasm -fbin $< -o $@

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.asm
	nasm -f elf32 $< -o $@

run: os.iso
	$(QEMU) $(QEMUOPTS)

debug: os.iso kernel/kernel.elf
#	$(QEMU) -s -S $(QEMUOPTS) & $(DBG) kernel/kernel.elf $(DBGOPTS)
	$(QEMU) -s -S $(QEMUOPTS)

clean:
	rm -f */*.bin */*.o */*.elf os.iso *.elf

	-VBoxManage unregistervm --delete $(VM_NAME)
	rm -rf $(VM_FOLDER)

hoge:
	objdump -b binary -m i386 -M intel -D kernel/kernel.binos.iso:
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