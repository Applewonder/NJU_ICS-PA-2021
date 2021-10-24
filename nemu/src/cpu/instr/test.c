#include "cpu/instr.h"
/*
Put the implementations of `test' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_and(opr_src.val, opr_dest.val, data_size);
	cpu.CF = 0;
	cpu.OF = 0;
	operand_write(&opr_dest);
}
make_instr_impl_2op(test, i, rm, v)