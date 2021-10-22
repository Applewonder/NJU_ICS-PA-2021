#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val, sign_ext(opr.src, opr_dest.data_size));
}
make_instr_impl_2op(cmp, i, rm, bv)