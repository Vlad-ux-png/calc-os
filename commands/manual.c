#include <coms.h>
#include <riscv.h>
#include <video.h>

void cln() {
    #if defined(__riscv)
        screen_clear();
        return;
    #else
        screen_clear();
        ncount = 1;
        return;
    #endif
}

void help() {
    #if defined(__riscv)
    print("Available commands:\n", 15);
    print("  help - show this message\n", 15);
    print("  status - print CPU status\n", 15);
    print("  reboot - reboot the system\n", 15);
    print("  devices - print PCI devices\n", 15);
    print("  lifetime - system timer ticks\n", 15);
    #else
    print("Available commands:\n", 15);
    print("  help - show this message\n", 15);
    print("  cln  - clear the screen\n", 15);
    print("  ls  - list all files\n", 15);
    print("  touch  - create a new file\n", 15);
    print("  draw - draw a rectangle\n", 15);
    print("  status - check system status\n", 15);
    print("  cat - print file content\n", 15);
    print("  devices - print PCI devices\n", 15);
    print("  send - send a byte to the network\n", 15);
    print("  behave - receive a byte from the network\n", 15);
    print("  reboot - reboot the system\n", 15);
    print("  forth - Forth interpreter\n", 15);
    print("  empire - play imperial march\n", 15);
    print("  umount - umount FS\n", 15);
    print("  mount - mount FS\n", 15);
    print("  casm - Calc Assembly Compiler\n", 15);
    print("  su - change to root\n", 15);
    print("  exit - change to user\n", 15);
    print("  whoami - print current user\n", 15);
    #endif
}
