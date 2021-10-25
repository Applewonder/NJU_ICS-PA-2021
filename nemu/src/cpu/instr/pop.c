#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op()
{
    OPERAND opr;
    opr.type = OPR_MEM;
    opr.addr = cpu.esp;
    opr.data_size = data_size;
    operand_read(&opr);
    
    operand_read(&opr_dest);
    opr_dest.val = opr.val;
    operand_write(&opr_dest);
    cpu.esp += 4;
}
make_instr_impl_1op(pop, r, v)