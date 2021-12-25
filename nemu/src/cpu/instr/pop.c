#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/

static void instr_execute_1op()
{
    
    // uint32_t opcode = instr_fetch(cpu.eip, 1);
    // opcode -= 0x58;
    
    // OPERAND esp;
    // esp.type = OPR_MEM;
    // esp.addr = cpu.esp;
    // esp.data_size = 32;
    // operand_read(&esp);
    
    // OPERAND opr;
    // opr.type = OPR_REG;
    // opr.addr = opcode;
    // opr.data_size = 32;
    // opr.val = esp.val;
    
    // operand_write(&opr);
    // cpu.esp += 4;
    opr.dest.addr = cpu.esp;
    opr.dest.type = OPR.MEM;
    operand_read(&opr_dest);
    opr_src.val = opr_dest.val;
    operand_write(&opr_src);
    cpu.esp += data_size/8;
    
    
}

make_instr_impl_1op(pop, r, v);