#include "cpu/exec.h"
// Data Movement Instructions
make_EHelper(mov);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(leave);
make_EHelper(cwtl); // Convert Word to Long
make_EHelper(cwtd); // Convert Signed Word to Signed Double Word
make_EHelper(movsx);
make_EHelper(movzx);

// Binary Arithmetic Instructions
make_EHelper(add);
make_EHelper(inc);
make_EHelper(sub);
make_EHelper(dec);
make_EHelper(cmp);
make_EHelper(neg);

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

// Control Transfer Instructions
make_EHelper(jcc);
make_EHelper(call);
make_EHelper(ret);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);
