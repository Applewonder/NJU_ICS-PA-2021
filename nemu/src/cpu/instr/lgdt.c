#include "cpu/instr.h"
/*
Put the implementations of `lgdt' instructions here.
*/
make_instr_func(lgdt)
{
    int len = 1;
    OPERAND opr;
    //printf("%x\n", eip);
    opr.type = OPR_MEM;
    //opr.addr = 0x0003004c;
    len += modrm_rm(eip + 1, &opr);
    //assert(opr.addr == 0x0003004c);
    opr.sreg = SREG_DS;
    opr.data_size = 32;
    //assert(0);
    operand_read(&opr);
    
    //assert(0);
    cpu.gdtr.limit = opr.val;
    printf("base = %x\n", cpu.gdtr.limit);
    opr.addr += 4;
    opr.data_size = 32;
    
    operand_read(&opr);
    printf("limit = %x\n", opr.val);
    //assert(0);
    cpu.gdtr.base = opr.val;
    return len;
}