#ifndef __FS_H__
#define __FS_H__

#include "common.h"

enum {SEEK_SET, SEEK_CUR, SEEK_END};

void init_fs();
void check_fd_valid(int fd);
char* get_name(int fd);
size_t get_size(int fd);
off_t get_disk_offset(int fd);
off_t get_open_offset(int fd);
void set_open_offset(int fd, off_t offset);
int fs_open(const char *pathname, int flags, int mode);
ssize_t fs_read(int fd, void *buf, size_t len);
ssize_t fs_write(int fd, const void *buf, size_t len);
off_t fs_lseek(int fd, off_t offset, int whence);
int fs_close(int fd);


#endif
