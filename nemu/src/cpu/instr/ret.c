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
