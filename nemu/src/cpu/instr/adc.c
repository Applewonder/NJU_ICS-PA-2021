#include "cpu/instr.h"
/*
Put the implementations of `adc' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val += opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(add, r, rm, b)