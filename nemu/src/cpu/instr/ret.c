#include "cpu/instr.h"
/*
Put the implementations of `ret' instructions here.
*/
int ret_near(uint32_t eip, uint8_t opcode)
{
    /*opr_dest.type = OPR_MEM;
    opr_dest.addr = cpu.esp;
    opr_dest.data_size = data_size;
    operand_read(&opr_dest);
    cpu.esp += 4;
    if (data_size == 16) {
        cpu.eip = opr_dest.val;
        cpu.eip = cpu.eip & 0x0000ffff;
    } else if(data_size == 16) {
        cpu.eip = opr_dest.val;
    }
    return 0;
}
