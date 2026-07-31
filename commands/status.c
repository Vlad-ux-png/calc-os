#define SYS_EXIT        1
#define SYS_WRITE       4
#define SYS_GET_BATTERY 123

static inline int _syscall(int num, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    int ret;
    __asm__ __volatile__ ("int $0x80" : "=a" (ret) : "a" (num), "b" (arg1), "c" (arg2), "d" (arg3) : "memory");
    return ret;
}
__attribute__((section(".text.prologue")))
void _start() {
    int battery_ok = _syscall(SYS_GET_BATTERY, 0, 0, 0);
    if (battery_ok) {
        _syscall(SYS_WRITE, 1, (unsigned int)"Battery: OK\n", 15);
    } else {
        _syscall(SYS_WRITE, 1, (unsigned int)"Battery: BAD. Please insert a new CMOS battery\n", 15);
    }
    _syscall(SYS_EXIT, 0, 0, 0);
}
