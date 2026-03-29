@echo off
set VBOX_PATH="C:\Program Files\Oracle\VirtualBox"

echo [STEP 1] Compiling Bootloader...
nasm -f bin stub.asm -o boot.bin

echo [STEP 2] Compiling IO (Assembly)...
nasm -f elf32 io.asm -o io.o

echo [STEP 3] Compiling C Kernel...
gcc -m32 -ffreestanding -fno-stack-protector -fno-leading-underscore -ffunction-sections -c kernel.c -o kernel.o

echo [STEP 4] Linking...
ld -m i386pe -T linker.ld -o kernel.pe kernel.o io.o

echo [STEP 5] Converting to binary...
objcopy -O binary kernel.pe kernel.bin

echo [STEP 6] Building Image...
if exist kernel.bin (
    copy /b boot.bin + kernel.bin os-image.bin
    echo [SUCCESS] os-image.bin created!
) else (
    echo [ERROR] kernel.bin was not created!
    pause
    exit /b
)

echo [STEP 7] Creating VHD...
if exist os-disk.vhd del os-disk.vhd
fsutil file seteof os-image.bin 65536
%VBOX_PATH%\VBoxManage.exe convertfromraw os-image.bin os-disk.vhd --format VHD

echo [STEP 8] deleting trash...
del /s io.o
del /s kernel.o 
del /s kernel.pe
del /s os-image.bin
del /s boot.bin
del /s kernel.bin

echo [DONE]
pause