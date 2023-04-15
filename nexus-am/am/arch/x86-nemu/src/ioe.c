#include <am.h>
#include <x86.h>

#define RTC_PORT 0x48 // Note that this is not standard
static unsigned long boot_time;

void _ioe_init()
{
  boot_time = inl(RTC_PORT);
}

unsigned long _uptime()
{
  unsigned long t = inl(RTC_PORT) - boot_time;
  return t;
}

uint32_t *const fb = (uint32_t *)0x40000;

_Screen _screen = {
    .width = 400,
    .height = 300,
};

extern void *memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h)
{
  // int i;
  // for (i = 0; i < _screen.width * _screen.height; i++)
  // {
  //   fb[i] = i;
  // }
  // idndex = _screen.width * y + x
  int temp = (w > _screen.width - x) ? _screen.width - x : w;
  int cp_bytes = sizeof(u_int32_t) * temp;
  for (int j = 0; j < h && y + j < _screen.height; j++)
  {
    // y+j乘屏幕宽度+x列得到内存中y+j行的首地址
    memcpy(&fb[(y + j) * _screen.width + x], pixels, cp_bytes); // pixels是像素数组首地址
    pixels += w;
  }
}

void _draw_sync()
{
}

int _read_key()
{
  if (inb(0x64))
  {
    return inl(0x60);
  }
  else
    return _KEY_NONE;
}
