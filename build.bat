@echo off

nasm -f bin src/boot/stub.asm -o boot.bin

nasm -f elf32 src/arch/io.asm -o io.o
echo done
nasm -f bin src/arch/POWER.asm -o POWER.bin
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
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -mgeneral-regs-only -mno-red-zone -I./include -c src/c/font.c -o font.o

ld -m i386pe -T linker.ld -o kernel.pe kernel.o disk.o stdio.o mouse2.o utils.o keyboard.o font.o io.o inout.o mouse.o

objcopy -O binary kernel.pe kernel.bin

if exist kernel.bin (
    copy /b boot.bin + kernel.bin temp_image.bin
    fsutil file createnew os-image.img 1474560
    powershell -Command "$boot = [System.IO.File]::ReadAllBytes('temp_image.bin'); $image = [System.IO.File]::ReadAllBytes('os-image.img'); [System.Array]::Copy($boot, $image, $boot.Length); [System.IO.File]::WriteAllBytes('os-image.img', $image)"
    del temp_image.bin
) else (
    goto cleanup_error
)

if exist POWER.bin (
    powershell -Command " $image = [System.IO.File]::ReadAllBytes('os-image.img'); $prog = [System.IO.File]::ReadAllBytes('POWER.bin'); $appStruct = New-Object byte[] 512; $nameBytes = [System.Text.Encoding]::ASCII.GetBytes('POWER'); [System.Array]::Copy($nameBytes, $appStruct, $nameBytes.Length); $lba = [System.BitConverter]::GetBytes([int]200); [System.Array]::Copy($lba, 0, $appStruct, 32, 4); $size = [System.BitConverter]::GetBytes([int]1); [System.Array]::Copy($size, 0, $appStruct, 36, 4); $appStruct[40] = [byte]1; [System.Array]::Copy($appStruct, 0, $image, (150 * 512), 512); [System.Array]::Copy($prog, 0, $image, (200 * 512), $prog.Length); [System.IO.File]::WriteAllBytes('os-image.img', $image) "
) else (
    echo Error: POWER.bin not found!
	goto cleanup
)

qemu-system-i386 -drive format=raw,file=os-image.img

:cleanup
del *.o
del kernel.pe
del kernel.bin
del boot.bin
del power.bin
exit /b

:cleanup_error
del *.o
del kernel.pe
del boot.bin
del power.bin
pause
exit /b
