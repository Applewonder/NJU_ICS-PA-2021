#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
int call_near(uint32_t eip, uint8_t opcode) 
{
     int len = 5;
     opr_src.data_size = opr_dest.data_size = 32;
     data_size = 32;
     cpu.esp -= 4;
     opr_dest.type = OPR_MEM;
     opr_dest.addr = cpu.esp;
     opr_dest.data_size = data_size;
     opr_dest.val = cpu.eip;
     operand_write(&opr_dest);

     cpu.eip += opr_src.val;
     return len;
}