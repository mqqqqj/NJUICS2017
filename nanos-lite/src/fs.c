#include "fs.h"

extern void ramdisk_read(void *buf, off_t offset, size_t len);
extern void ramdisk_write(const void *buf, off_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  off_t disk_offset;
  off_t open_offset;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB, FD_EVENTS, FD_DISPINFO, FD_NORMAL};

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  {"stdin (note that this is not the actual stdin)", 0, 0},
  {"stdout (note that this is not the actual stdout)", 0, 0},
  {"stderr (note that this is not the actual stderr)", 0, 0},
  [FD_FB] = {"/dev/fb", 0, 0},
  [FD_EVENTS] = {"/dev/events", 0, 0},
  [FD_DISPINFO] = {"/proc/dispinfo", 128, 0},
#include "files.h"
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

void init_fs() {
  // TODO: initialize the size of /dev/fb
}

void check_fd_valid(int fd) {
  assert(fd >=0 && fd < NR_FILES);
}

char* get_name(int fd) {
  check_fd_valid(fd);
  return file_table[fd].name;
}

size_t get_size(int fd) {
  check_fd_valid(fd);
  return file_table[fd].size;
}

off_t get_disk_offset(int fd) {
  check_fd_valid(fd);
  return file_table[fd].disk_offset;
}

off_t get_open_offset(int fd) {
  check_fd_valid(fd);
  return file_table[fd].open_offset;
}

void set_open_offset(int fd, off_t offset) {
  check_fd_valid(fd);
  // assert(offset >=0 && offset <= file_table[fd].size);
  if(offset > file_table[fd].size)
    offset = file_table[fd].size;
  file_table[fd].open_offset = offset;
}

int fs_open(const char *pathname, int flags, int mode) {
  for(int i = 0; i < NR_FILES; i ++) {
    if(strcmp(pathname, get_name(i)) == 0)
      return i;
  }
  panic("File does't exist.");
  return -1;
}

ssize_t fs_read(int fd, void *buf, size_t len) {
  check_fd_valid(fd);
  if(fd < 3) {
    Log("Can't read placeholder entry.");
    return 0;
  }
  int read_upper_bound = get_size(fd) - get_open_offset(fd);
  if(read_upper_bound < len)
    len = read_upper_bound;
  ramdisk_read(buf, get_disk_offset(fd) + get_open_offset(fd), len);
  set_open_offset(fd, get_open_offset(fd) + len);
  return len;
}

ssize_t fs_write(int fd, const void *buf, size_t len) {
  check_fd_valid(fd);
  if(fd < 3) {
    Log("Can't write placeholder entry.");
    return 0;
  }
  int write_upper_bound = get_size(fd) - get_open_offset(fd);
  if(write_upper_bound < len)
    len = write_upper_bound;
  ramdisk_write(buf, get_disk_offset(fd) + get_open_offset(fd), len);
  set_open_offset(fd, get_open_offset(fd) + len);
  return len;
}

off_t fs_lseek(int fd, off_t offset, int whence) {
  check_fd_valid(fd);
  switch(whence) {
    case SEEK_SET:
      set_open_offset(fd, offset);
      return get_open_offset(fd);
    case SEEK_CUR:
      set_open_offset(fd, get_open_offset(fd) + offset);
      return get_open_offset(fd);
    case SEEK_END:
      set_open_offset(fd, get_size(fd) + offset);
      return get_open_offset(fd);
    default:
      panic("Unhandled whence ID = %d", whence);
      return -1;
  }
  return 0;
}

int fs_close(int fd) {
  check_fd_valid(fd);
  return 0;
}