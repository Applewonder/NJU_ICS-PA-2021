#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt)
{
    int len = 1;
    OPERAND opr;
    opr.type = OPR_MEM;
    len += modrm_rm(eip + 1, &opr);
    opr.sreg = SREG_DS;
    opr.data_size = 16;
    operand_read(&opr);
    
    cpu.gdtr.limit = opr.val;
    opr.addr += 2;
    opr.data_size = 32;
    
    operand_read(&opr);
    cpu.gdtr.base = opr.val;
    return len;
}