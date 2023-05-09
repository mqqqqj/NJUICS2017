#include "cpu/exec.h"
// Data Movement Instructions
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(leave);
make_EHelper(cwtl); // Convert Word to Long
make_EHelper(cltd); // Convert Signed Word to Signed Double Word
make_EHelper(movsx);
make_EHelper(movzx);

// Binary Arithmetic Instructions
make_EHelper(add);
make_EHelper(inc);
make_EHelper(sub);
make_EHelper(dec);
make_EHelper(cmp);
make_EHelper(neg);
make_EHelper(adc);
make_EHelper(sbb);
make_EHelper(mul);
make_EHelper(imul1);
make_EHelper(imul2);
make_EHelper(imul3);
make_EHelper(div);
make_EHelper(idiv);

// Logical Instructions
make_EHelper(not );
make_EHelper(and);
make_EHelper(or);
make_EHelper(xor);
make_EHelper(sal); // shift arithmetic left
make_EHelper(shl); // shift left logical
make_EHelper(shr);
make_EHelper(sar);
make_EHelper(setcc);
make_EHelper(test);
make_EHelper(rol);

// Control Transfer Instructions
make_EHelper(jmp);
make_EHelper(jmp_rm);
make_EHelper(jcc);
make_EHelper(call);
make_EHelper(call_rm);
make_EHelper(ret);

// Miscellaneous Instructions
make_EHelper(lea);
make_EHelper(nop);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

make_EHelper(in);
make_EHelper(out);

// System Instructions
make_EHelper(lidt);
