#include "cpu/exec.h"

make_EHelper(mov)
{
  operand_write(id_dest, &id_src->val);
  print_asm_template2(mov);
}

make_EHelper(push)
{
  // TODO();
  rtl_sext(&t0, &id_dest->val, id_dest->width);
  rtl_push(&t0);
  print_asm_template1(push);
}

make_EHelper(pop)
{
  // TODO();
  rtl_pop(&t2);
  operand_write(id_dest, &t2);
  print_asm_template1(pop);
}

make_EHelper(pusha)
{
  // TODO();
  t0 = cpu.esp;
  rtl_push(&cpu.eax);
  rtl_push(&cpu.ecx);
  rtl_push(&cpu.edx);
  rtl_push(&cpu.ebx);
  rtl_push(&t0);
  rtl_push(&cpu.ebp);
  rtl_push(&cpu.esi);
  rtl_push(&cpu.edi);
  print_asm("pusha");
}

make_EHelper(popa)
{
  // TODO();
  rtl_pop(&cpu.edi);
  rtl_pop(&cpu.esi);
  rtl_pop(&cpu.ebp);
  rtl_pop(&t0);
  rtl_pop(&cpu.ebx);
  rtl_pop(&cpu.edx);
  rtl_pop(&cpu.ecx);
  rtl_pop(&cpu.eax);

  print_asm("popa");
}
/* movl %ebp %esp
 * popl %ebp
 */
make_EHelper(leave)
{
  // TODO();
  rtl_mv(&cpu.esp, &cpu.ebp);
  rtl_pop(&cpu.ebp);
  print_asm("leave");
}
/* 作用是把 eax 的 32 位整数扩展为 64 位，高 32 位用 eax 的符号位填充保存到edx;
 * 或 ax 的 16 位整数扩展为 32 位，高 16 位用 ax 的符号位填充保存到 dx */
make_EHelper(cltd)
{
  // TODO();
  if (decoding.is_operand_size_16)
  {
    rtl_lr_w(&t0, R_AX);
    rtl_sext(&t0, &t0, 2);
    rtl_sari(&t0, &t0, 31); //>16似乎都可以
    rtl_sr_w(R_DX, &t0);
  }
  else
  {
    rtl_sari(&cpu.edx, &cpu.eax, 31);
  }

  print_asm(decoding.is_operand_size_16 ? "cwtl" : "cltd");
}

make_EHelper(cwtl)
{
  // TODO();
  if (decoding.is_operand_size_16)
  {
    rtl_msb(&t2,&cpu.eax,1);
    if(t2 == 1) {
      cpu.eax |= 0xff00;
    } else {
      cpu.eax &= 0xffff00ff;
    }
    //  rtl_lr_w(&t0, R_AX);
    // rtl_sext(&t0, &t0, 2);
    // rtl_sari(&t0, &t0, 16);
    // rtl_sr_w(R_DX, &t0);
  }
  else
  {
    // panic("Nemu does not allow an operand to have a size of a single byte.");
    rtl_msb(&t2,&cpu.eax,2);
    if(t2 == 1) {
      cpu.eax |= 0xffff0000;
    } else {
      cpu.eax &= 0x0000ffff;
    }
  }
  print_asm(decoding.is_operand_size_16 ? "cbtw" : "cwtl");
}

make_EHelper(movsx)
{
  // TODO();
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  rtl_sext(&t2, &id_src->val, id_src->width);
  operand_write(id_dest, &t2);
  print_asm_template2(movsx);
}

make_EHelper(movzx)
{
  // TODO();
  id_dest->width = decoding.is_operand_size_16 ? 2 : 4;
  operand_write(id_dest, &id_src->val);
  print_asm_template2(movzx);
}

make_EHelper(lea)
{
  // TODO();
  rtl_li(&t2, id_src->addr);
  operand_write(id_dest, &t2);
  print_asm_template2(lea);
}
