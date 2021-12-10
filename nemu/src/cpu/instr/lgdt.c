#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt)
{
    uint32_t addr = vaddr_read(eip + 3, 1, 8);
    OPERAND opr;
    opr.type = OPR_MEM;
    opr.addr = addr;
    opr.sreg = SREG_DS;
    opr.data_size = 16;
    operand_read(&opr);
    cpu.gdtr.limit = opr.val;
    opr.addr += 2;
    opr.data_size = 32;
    operand_read(&opr);
    cpu.gdtr.base = opr.val;
    return 7;
}