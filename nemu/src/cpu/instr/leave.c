#include "cpu/instr.h"
/*
Put the implementations of `leave' instructions here.
*/
make_instr_func(leave)
{
    int len = 1;
    cpu.esp = cpu.ebp;
    
    OPERAND opr;
    opr.type = OPR_MEM;
    opr.addr = cpu.esp;
    opr.data_size = data_size;
    operand_read(&opr);
    cpu.ebp = opr.val;
    cpu.esp += 4;
    
    return len;
}