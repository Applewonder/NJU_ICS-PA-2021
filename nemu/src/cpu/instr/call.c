#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
int call_near(uint32_t eip, uint8_t opcode) 
{
     opr_src.type = OPR_IMM;
     opr_src.addr = cpu.eip + 1;
     opr_src.data_size = 32;
     operand_read(&opr_src);
     
     opr_dest.data_size = 32;
     cpu.esp -= 4;
     opr_dest.type = OPR_MEM;
     opr_dest.addr = cpu.esp;
     opr_dest.val = (cpu.eip + 1 + data_size / 8);
     operand_write(&opr_dest);

     cpu.eip += (opr_src.val + 1 + data_size / 8);
     return 0;
}

int call_near_indirect(uint32_t eip, uint8_t opcode) 
{
     int len = 1;
     opr_src.data_size = 32;
     len += modrm_rm(eip + 1, &opr_src);
     operand_read(&opr_src);
     
     opr_dest.data_size = 32;
     cpu.esp -= 4;
     opr_dest.type = OPR_MEM;
     opr_dest.addr = cpu.esp;
     opr_dest.val = (cpu.eip + 1 + data_size / 8);
     operand_write(&opr_dest);
     
     cpu.eip += opr_src.val;
     return 0;
}