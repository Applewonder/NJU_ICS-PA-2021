#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/
int ret_near(uint32_t eip, uint8_t opcode)
{
    OPERAND opr;
    opr.type = OPR_MEM;
    opr.addr = cpu.esp;
    opr.data_size = data_size;
    operand_read(&opr);
    cpu.esp += 4;
    cpu.eip = opr.val;
    return 0;
}

int ret_near_imm16(uint32_t eip, uint8_t opcode)
{
    OPERAND imm;
    imm.type = OPR_IMM;
    imm.addr = cpu.eip + 1;
    imm.data_size = 16;
    operand_read(&imm);
    
    OPERAND opr;
    opr.type = OPR_MEM;
    opr.addr = cpu.esp;
    opr.data_size = (imm.val * 4);
    operand_read(&opr);
    cpu.esp += imm.val;
    
    cpu.eip = opr.val;
    return 0;
}