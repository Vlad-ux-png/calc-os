#define SYS_EXIT       1
#define SYS_LIST_FILES 126

static inline int _syscall(int num, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    int ret;
    __asm__ __volatile__ ("int $0x80" : "=a" (ret) : "a" (num), "b" (arg1), "c" (arg2), "d" (arg3) : "memory");
    return ret;
}

__attribute__((section(".text.prologue")))
void _start() {
    _syscall(SYS_LIST_FILES, 0, 0, 0);
    _syscall(SYS_EXIT, 0, 0, 0);
}
