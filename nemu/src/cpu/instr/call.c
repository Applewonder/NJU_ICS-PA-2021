#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
void call_near(uint32_t eip, uint8_t opcode) 
{
    operand_read(&opr_src);
    
    cpu.esp -= 4;
    opr_dest.type = OPR_MEM;
    opr_dest.addr = cpu.esp;
    opr_dest.data_size = data_size;
    opr_dest.val = cpu.eip;
    operand_write(&opr_dest);
}
    cpu.eip += opr_src.val;
}