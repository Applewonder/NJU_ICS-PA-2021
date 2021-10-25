#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

make_instr_func(pop_r_v)
{
    int len = 1;
    
    // opcode = instr_fetch(cpu.eip, 1);
    // opcode -= 0x58;
    
    // OPERAND esp;
    // esp.type = OPR_MEM;
    // esp.addr = cpu.esp;
    // esp.data_size = 32;
    // operand_read(&esp);
    
    // OPERAND opr;
    // opr.type = OPR_REG;
    // opr.addr = opcode;
    // opr.data_size = data_size;
    // opr.val = esp.val;
    
    // operand_write(&opr);
    // cpu.esp += 4;
    
    return len;
}