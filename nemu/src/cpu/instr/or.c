#include "cpu/instr.h"
/*
Put the implementations of `or' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	
	opr_dest.val = alu_or(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}
make_instr_impl_2op(or, r, rm, v)