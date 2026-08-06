#define SYS_EXIT        1
#define SYS_WRITE       4
#define SYS_GETUID      24

static inline int _syscall(int num, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    int ret;
    __asm__ __volatile__ ("int $0x80" : "=a" (ret) : "a" (num), "b" (arg1), "c" (arg2), "d" (arg3) : "memory");
    return ret;
}

__attribute__((section(".text.prologue")))
void _start() {
    int current_uid = _syscall(SYS_GETUID, 0, 0, 0);
    if (current_uid != 0) {
        _syscall(SYS_WRITE, 1, (unsigned int)"user\n", 15);
        _syscall(SYS_EXIT, 0, 0, 0);
        return;
    } else {
        _syscall(SYS_WRITE, 1, (unsigned int)"root\n", 15);
        _syscall(SYS_EXIT, 0, 0, 0);
        return;
    }
}

/*
    if (current_uid != 0) {
        printk("user\n", 15);
        return;
    } else {
        printk("root\n", 15);
        return;
    }
*/