#define SYS_EXIT        1
#define SYS_WRITE       4
#define SYS_DRAW_RECT   127
#define SYS_WAIT_STRING 129
#define SYS_GETUID      24

static inline int _syscall(int num, unsigned int arg1, unsigned int arg2, unsigned int arg3) {
    int ret;
    __asm__ __volatile__ ("int $0x80" : "=a" (ret) : "a" (num), "b" (arg1), "c" (arg2), "d" (arg3) : "memory");
    return ret;
}

int atoi(const char* str) {
    int res = 0;
    for (int i = 0; str[i] != '\0'; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            res = res * 10 + str[i] - '0';
        }
    }
    return res;
}

__attribute__((section(".text.prologue")))
void _start() {
    int current_uid = _syscall(SYS_GETUID, 0, 0, 0);
    if (current_uid != 0) {
        _syscall(SYS_WRITE, 1, (unsigned int)"draw: Permission denied\n", 15);
        _syscall(SYS_EXIT, 0, 0, 0);
        return; 
    } 
    
    char val[256];
    int r_w, r_h, r_x, r_y;

    _syscall(SYS_WRITE, 1, (unsigned int)"Enter width: ", 15);
    _syscall(SYS_WAIT_STRING, (unsigned int)val, 0, 0);
    _syscall(SYS_WRITE, 1, (unsigned int)"\n", 15);
    r_w = atoi(val);

    _syscall(SYS_WRITE, 1, (unsigned int)"Enter height: ", 15);
    _syscall(SYS_WAIT_STRING, (unsigned int)val, 0, 0);
    _syscall(SYS_WRITE, 1, (unsigned int)"\n", 15);
    r_h = atoi(val);

    _syscall(SYS_WRITE, 1, (unsigned int)"Enter x: ", 15);
    _syscall(SYS_WAIT_STRING, (unsigned int)val, 0, 0);
    _syscall(SYS_WRITE, 1, (unsigned int)"\n", 15);
    r_x = atoi(val);

    _syscall(SYS_WRITE, 1, (unsigned int)"Enter y: ", 15);
    _syscall(SYS_WAIT_STRING, (unsigned int)val, 0, 0);
    _syscall(SYS_WRITE, 1, (unsigned int)"\n", 15);
    r_y = atoi(val);

    unsigned int packed_xy = ((r_x & 0xFFFF) << 16) | (r_y & 0xFFFF);
    unsigned int packed_wh = ((r_w & 0xFFFF) << 16) | (r_h & 0xFFFF);
    _syscall(SYS_DRAW_RECT, packed_xy, packed_wh, 15);

    _syscall(SYS_EXIT, 0, 0, 0);
}
