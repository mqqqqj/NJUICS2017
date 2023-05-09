#include "common.h"
#include "syscall.h"

int sys_none() {
  return 1;
}

void sys_exit(int a) {
  Log("call sys_exit.");
  _halt(a);
}

int sys_write(int fd, void* buf, size_t len) {
  Log("call sys_write.");
  if(fd == 1 || fd == 2) {
    char c;
    for(int i = 0; i < len; i ++) {
      memcpy(&c, buf + i, 1);
      _putc(c);
    }
    return len;
  } else if( fd == 3) {
    // return fs_write(fd, buf, len);
    return 0;
  } else {
    panic("Unhandled fd = %d in sys_write.", fd);
  }
  return -1;
}

_RegSet* do_syscall(_RegSet *r) {
  uintptr_t a[4];
  a[0] = SYSCALL_ARG1(r);
  a[1] = SYSCALL_ARG2(r);
  a[2] = SYSCALL_ARG3(r);
  a[3] = SYSCALL_ARG4(r);

  switch (a[0]) {
    case SYS_none:
      SYSCALL_ARG1(r) = sys_none();// we definee sys_none as return 1
      break;
    case SYS_exit:
      sys_exit(a[1]);
      break;
    case SYS_write:
      SYSCALL_ARG1(r) = sys_write(a[1], (void*)a[2], a[3]);
      break;
    default: panic("Unhandled syscall ID = %d", a[0]);
  }

  return NULL;
}
