#include <am.h>
#include <x86.h>

#define RTC_PORT 0x48   // Note that this is not standard
static unsigned long boot_time;

void _ioe_init() {
  boot_time = inl(RTC_PORT);
}

unsigned long _uptime() {
  unsigned long t = inl(RTC_PORT) - boot_time;
  return t;
}

uint32_t* const fb = (uint32_t *)0x40000;

_Screen _screen = {
  .width  = 400,
  .height = 300,
};

extern void* memcpy(void *, const void *, int);

void _draw_rect(const uint32_t *pixels, int x, int y, int w, int h) {
	int boarder_x = (w < _screen.width - x)? w : _screen.width - x;
	int c = sizeof(uint32_t) *boarder_x;
	int boarder_y = (h < _screen.height - y)? h : _screen.height - y;
	int i = 0;
	for (i = 0; i < boarder_y; i ++) {
		memcpy(&fb[(y + i) * _screen.width + x], pixels, c);
		pixels += w;
	}
}

void _draw_sync() {
}

int _read_key() {
  if (inb(0x64))
	return inl(0x60);
  else
  	return _KEY_NONE;
}
