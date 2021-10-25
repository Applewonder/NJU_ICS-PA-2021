#include "cpu/instr.h"
/*
Put the implementations of `inc' instructions here.
*/
static void instr_execute_1op() 
{
	operand_read(&opr_dest);
	opr_dest.val = alu_add(1, opr_dest.val, data_size);
	operand_write(&opr_dest);
}

make_instr_impl_1op(inc, rm, v)