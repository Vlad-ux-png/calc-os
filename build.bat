@echo off
set VBOX_PATH="C:\Program Files\Oracle\VirtualBox"

echo [STEP 1] Compiling Bootloader...
nasm -f bin stub.asm -o boot.bin

echo [STEP 2] Compiling IO and Power (Assembly)...
nasm -f elf32 io.asm -o io.o
nasm -f elf32 power.asm -o power.o

echo [STEP 3] Compiling C Code...
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -c kernel.c -o kernel.o
:: gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -c lib/system.c -I./include -o system.o
:: gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -c drivers/keyboard.c -I./include -o keyboard.o
:: gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -c lib/programs.c -I./include -o programs.o

echo [STEP 4] Linking...
ld -m i386pe -T linker.ld -o kernel.pe kernel.o io.o power.o

echo [STEP 5] Converting to binary...
objcopy -O binary kernel.pe kernel.bin

echo [STEP 6] Building Image...
if exist kernel.bin (
    copy /b boot.bin + kernel.bin os-image.bin
    echo [SUCCESS] os-image.bin created!
) else (
    echo [ERROR] kernel.bin was not created!
    del /s kernel.bin
    del /s boot.bin
    del /s kernel.pe
    del /s power.o
    del /s io.o
    del /s kernel.o
    del /s system.o
    del /s os-image.bin
    del /s programs.o
    del /s keyboard.o
    pause
    exit /b
)

echo [STEP 7] Creating VHD...
if exist os-disk.vhd del os-disk.vhd
fsutil file seteof os-image.bin 65536
%VBOX_PATH%\VBoxManage.exe convertfromraw os-image.bin os-disk.vhd --format VHD

echo [STEP 8] Cleaning up...
del /s kernel.bin
del /s keyboard.o
del /s boot.bin
del /s kernel.pe
del /s power.o
del /s io.o
del /s kernel.o
del /s system.o
del /s programs.o
del /s os-image.bin
echo [DONE]
pause