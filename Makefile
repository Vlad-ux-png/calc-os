AS = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy

ASFLAGS_BIN = -f bin
ASFLAGS_ELF = -f elf32
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore \
         -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c
LDFLAGS = -m elf_i386 -T linker.ld

OBJ = kernel.o disk.o stdio.o mouse2.o utils.o keyboard.o font.o io.o inout.o mouse.o

all: os-image.img

os-image.img: boot.bin kernel.bin power.bin
	dd if=/dev/zero of=os-image.img bs=512 count=2880

	cat boot.bin kernel.bin > temp_image.bin
	dd if=temp_image.bin of=os-image.img conv=notrunc
	dd if=/dev/zero of=app_struct.bin bs=512 count=1
	
	echo -n "power" | dd of=app_struct.bin bs=1 conv=notrunc
	
	printf '\310\0\0\0' | dd of=app_struct.bin bs=1 seek=32 conv=notrunc
	
	printf '\001\0\0\0' | dd of=app_struct.bin bs=1 seek=36 conv=notrunc
	
	printf '\001' | dd of=app_struct.bin bs=1 seek=40 conv=notrunc
	
	dd if=app_struct.bin of=os-image.img bs=512 seek=150 conv=notrunc
	
	dd if=power.bin of=os-image.img bs=512 seek=200 conv=notrunc
	
	@rm temp_image.bin app_struct.bin

boot.bin: src/boot/stub.asm
	$(AS) $(ASFLAGS_BIN) $< -o $@

power.bin: src/arch/power.asm
	$(AS) $(ASFLAGS_BIN) $< -o $@

io.o: src/arch/io.asm
	$(AS) $(ASFLAGS_ELF) $< -o $@

inout.o: src/arch/inout.asm
	$(AS) $(ASFLAGS_ELF) $< -o $@

mouse.o: src/arch/mouse.asm
	$(AS) $(ASFLAGS_ELF) $< -o $@

kernel.o: src/kernel/kernel.c
	$(CC) $(CFLAGS) $< -o $@

disk.o: src/c/disk.c
	$(CC) $(CFLAGS) $< -o $@

stdio.o: src/c/stdio.c
	$(CC) $(CFLAGS) $< -o $@

mouse2.o: src/c/mouse.c
	$(CC) $(CFLAGS) $< -o $@

utils.o: src/c/utils.c
	$(CC) $(CFLAGS) $< -o $@

keyboard.o: src/c/keyboard.c
	$(CC) $(CFLAGS) $< -o $@

font.o: src/c/font.c
	$(CC) $(CFLAGS) $< -o $@

kernel.bin: $(OBJ)
	$(LD) $(LDFLAGS) -o kernel.elf $(OBJ)
	$(OBJCOPY) -O binary kernel.elf kernel.bin


clean:
	rm -f *.o *.bin *.elf 

cleane:
	rm -f *.o *.bin *.elf *.img

run: os-image.img
	qemu-system-i386 -drive file=os-image.img,format=raw
