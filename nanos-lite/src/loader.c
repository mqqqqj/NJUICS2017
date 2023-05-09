#include "common.h"
#include "fs.h"
#define DEFAULT_ENTRY ((void *)0x4000000)

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;
extern void ramdisk_read(void *buf, off_t offset, size_t len);

uintptr_t loader(_Protect *as, const char *filename) {
  // TODO();
  // ramdisk_read(DEFAULT_ENTRY, 0, (&ramdisk_end) - (&ramdisk_start));
  int fd = fs_open(filename, 0, 0);
  Log("Load img from file %s",filename);
  fs_read(fd, DEFAULT_ENTRY, get_size(fd));
  fs_close(fd);
  return (uintptr_t)DEFAULT_ENTRY;
}
