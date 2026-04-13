@echo off

nasm -f bin src/boot/stub.asm -o boot.bin

nasm -f elf32 src/arch/io.asm -o io.o
echo done
nasm -f elf32 src/arch/power.asm -o power.o
echo done
nasm -f elf32 src/arch/inout.asm -o inout.o
echo done
nasm -f elf32 src/arch/mouse.asm -o mouse.o
echo done

gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c src/kernel/kernel.c -o kernel.o
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c src/c/disk.c -o disk.o
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c src/c/stdio.c -o stdio.o
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c src/c/mouse.c -o mouse2.o
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c src/c/utils.c -o utils.o
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c src/c/keyboard.c -o keyboard.o

ld -m i386pe -T linker.ld -o kernel.pe kernel.o disk.o stdio.o mouse2.o utils.o keyboard.o io.o power.o inout.o mouse.o

objcopy -O binary kernel.pe kernel.bin

if exist kernel.bin (
    copy /b boot.bin + kernel.bin temp_image.bin
    fsutil file createnew os-image.img 1474560
    powershell -Command "$boot = [System.IO.File]::ReadAllBytes('temp_image.bin'); $image = [System.IO.File]::ReadAllBytes('os-image.img'); [System.Array]::Copy($boot, $image, $boot.Length); [System.IO.File]::WriteAllBytes('os-image.img', $image)"
    del temp_image.bin
) else (
    goto cleanup_error
)

qemu-system-i386 -drive format=raw,file=os-image.img

:cleanup
del *.o
del kernel.pe
del kernel.bin
del boot.bin
exit /b

:cleanup_error
del *.o
del kernel.pe
del boot.bin
pause
exit /b
