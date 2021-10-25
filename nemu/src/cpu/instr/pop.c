#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op()
{
    int opcode = instr_fetch(cpu.eip + 1, 1);
    opcode -= 0x58;
    
    opr_dest.type = OPR_MEM;
    opr_dest.addr = cpu.esp;
    opr_dest.data_size = data_size;
    operand_read(&opr_dest);
    
    OPERAND opr;
    opr.type = OPR_REG;
    opr.addr = opcode;
    opr.data_size = data_size;
    opr.val = opr_dest.val;
    
    operand_write(&opr);
    cpu.esp += 4;
}
make_instr_impl_1op(pop, r, v)