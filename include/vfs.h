#ifndef VFS_H
#define VFS_H
#include <stdint.h>

struct fs_driver {
    int (*open)(const char* path);
    int (*read)(int fd, void* buf, size_t count);
    int (*close)(int fd);
    int (*list)(void);
    int (*create)(const char* path, const uint8_t* data, size_t size); 
    int (*list_basic)(void);
    int (*umount)(void);
};

typedef struct {
    const char* path;          
    struct fs_driver* driver;  
} mount_t;

typedef struct {
    struct fs_driver* drv;
    int real_fd;
    int used;
} vfs_fd_t;

int vfs_open(const char* path);
int vfs_read(int fd, void* buf, size_t count);
int vfs_close(int fd);
int vfs_list(const char* path);
int vfs_mount(const char* path, struct fs_driver* driver);
int vfs_umount(const char* path);
static struct fs_driver* vfs_find_driver(const char* path, const char** rel);
void list_mounts();
int vfs_create(const char* path, const uint8_t* data, size_t size);

#endif
