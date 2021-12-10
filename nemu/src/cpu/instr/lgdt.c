#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt)
{
    printf("---->x");
    uint32_t addr = vaddr_read(eip + 3, 1, 4);
    printf("%x", addr);
    OPERAND opr;
    
    opr.val = vaddr_read(addr, 1, 2);
    assert(0);
    cpu.gdtr.limit = opr.val;
    opr.addr += 2;
    opr.data_size = 32;
    operand_read(&opr);
    cpu.gdtr.base = opr.val;
    return 7;
}