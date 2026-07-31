#define SYS_EXIT        1
#define SYS_WRITE       4
#define SYS_WAIT_STRING 129
#define SYS_UMOUNT2     166
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
        _syscall(SYS_WRITE, 1, (unsigned int)"umount: Permission denied\n", 15);
        _syscall(SYS_EXIT, 0, 0, 0);
        return; 
    } 
    
    char path[256];

    _syscall(SYS_WRITE, 1, (unsigned int)"Enter path: ", 15);
    _syscall(SYS_WAIT_STRING, (unsigned int)path, 0, 0);
    _syscall(SYS_WRITE, 1, (unsigned int)"\n", 15);

    int res = _syscall(SYS_UMOUNT2, (unsigned int)path, 0, 0);

    _syscall(SYS_EXIT, 0, 0, 0);
}
