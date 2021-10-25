#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op()
{
    int opcode = instr_fetch(cpu.eip + 1, 1);
    opcode -= 0x58;
    
    OPERAND opr;
    opr.type = OPR_REG;
    opr.addr = opcode;
    opr.data_size = data_size;
    opr.val = esp.val;
    
    operand_write(&opr);
    cpu.esp += 4;
}
make_instr_impl_1op(pop, r, v)