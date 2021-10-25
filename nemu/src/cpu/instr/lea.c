#include "cpu/instr.h"
/*
Put the implementations of `lea' instructions here.
*/

int lea(uint32_t eip, uint8_t opcode)
{
    int len = 1;
    decode_data_size_v;
    decode_operand_rm2r;
    opr_dest.val = opr_src.addr;
	operand_write(&opr_dest);
	return len;
}