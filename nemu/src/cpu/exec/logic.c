#include "cpu/exec.h"

make_EHelper(test)
{
  rtl_and(&t2, &id_dest->val, &id_src->val);
  rtl_update_ZFSF(&t2, id_dest->width);
  rtl_set_CF(&tzero);
  rtl_set_OF(&tzero);
  print_asm_template2(test);
}

make_EHelper(and)
{
  rtl_and(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);
  rtl_update_ZFSF(&t2, id_dest->width);
  cpu.eflags.CF = 0;
  cpu.eflags.OF = 0;
  print_asm_template2(and);
}

make_EHelper(xor)
{
  rtl_xor(&t2, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t2);
  // modify eflags
  // ZF,SF
  rtl_update_ZFSF(&t2, id_dest->width);
  // CF,OF
  rtl_set_OF(&tzero);
  rtl_set_CF(&tzero);
  print_asm_template2(xor);
}

make_EHelper(or)
{
  rtl_or(&t3, &id_dest->val, &id_src->val);
  // modify eflags
  operand_write(id_dest, &t3);
  rtl_update_ZFSF(&t3, id_dest->width);
  rtl_set_CF(&tzero);
  rtl_set_OF(&tzero);
  print_asm_template2(or);
}

make_EHelper(sal)
{
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&t3, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t3);
  rtl_update_ZFSF(&t3, id_dest->width);
  print_asm_template2(sal);
}

make_EHelper(sar)
{
  // unnecessary to update CF and OF in NEMU
  rtl_sext(&t3, &id_dest->val, id_dest->width);
  rtl_sar(&t3, &t3, &id_src->val);
  operand_write(id_dest, &t3);
  rtl_update_ZFSF(&t3, id_dest->width);
  print_asm_template2(sar);
}

make_EHelper(shl)
{
  // unnecessary to update CF and OF in NEMU
  rtl_shl(&t3, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t3);
  rtl_update_ZFSF(&t3, id_dest->width);
  print_asm_template2(shl);
}

make_EHelper(shr)
{
  // unnecessary to update CF and OF in NEMU
  rtl_shr(&t3, &id_dest->val, &id_src->val);
  operand_write(id_dest, &t3);
  rtl_update_ZFSF(&t3, id_dest->width);
  print_asm_template2(shr);
}

make_EHelper(setcc)
{
  uint8_t subcode = decoding.opcode & 0xf;
  rtl_setcc(&t2, subcode);
  operand_write(id_dest, &t2);

  print_asm("set%s %s", get_cc_name(subcode), id_dest->str);
}

make_EHelper(not )
{
  TODO();

  print_asm_template1(not );
}
