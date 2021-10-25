#include "cpu/instr.h"
/*
Put the implementations of `inc' instructions here.
*/
static void instr_execute_1op() 
{
	operand_read(&opr_src);
	opr_src.val = alu_add(1, opr_src.val, data_size);
	operand_write(&opr_src);
}

make_instr_impl_1op(inc, rm, v)