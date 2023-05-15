#include "cpu/exec.h"
#include "memory/mmu.h"

void raise_intr(uint8_t NO, vaddr_t ret_addr) {
  /* TODO: Trigger an interrupt/exception with ``NO''.
   * That is, use ``NO'' to index the IDT.
   */

  // TODO();
  memcpy(&t1, &cpu.eflags, sizeof(cpu.eflags));
  rtl_li(&t0, &t1);
  rtl_push(&t0);//push eflags
  rtl_push(&cpu.cs);//push cs
  rtl_li(&t0,ret_addr);
  rtl_push(&t0);//push eip
  vaddr_t gd_addr = cpu.idtr.base + NO*sizeof(GateDesc);
  assert(gd_addr <= cpu.idtr.base + cpu.idtr.limit);
  uint32_t offset_low = vaddr_read(gd_addr, 2);
  uint32_t offset_high = vaddr_read(gd_addr + sizeof(GateDesc) - 2, 2); 
  uint32_t target_addr = (offset_high << 16) + offset_low;
#ifdef DEBUG
  Log("Chosen intr addr is 0x%x", target_addr);
#endif
  decoding.is_jmp = 1;
  decoding.jmp_eip = target_addr;
}

void dev_raise_intr() {
}
