#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt)
{
    int len = 2;
    OPERAND opr;
    printf("%x", eip);
    opr.type = OPR_MEM;
    //opr.addr = 0x0003004c;
    len += modrm_rm(eip + 2, &opr);
    //assert(opr.addr == 0x0003004c);
    opr.sreg = SREG_DS;
    opr.data_size = 16;
    //assert(0);
    operand_read(&opr);
    
    //assert(0);
    cpu.gdtr.limit = opr.val;
    opr.addr += 2;
    opr.data_size = 32;
    operand_read(&opr);
    cpu.gdtr.base = opr.val;
    return len;
}