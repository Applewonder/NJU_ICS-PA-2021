#include "cpu/instr.h"
/*
Put the implementations of `sbb' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_sbb(sign_ext(opr_src.val, opr_src.data_size), opr_dest.val, data_size);
	operand_write(&opr_dest);
}
make_instr_impl_2op(sbb, r, rm, b)
make_instr_impl_2op(sbb, rm, r, v)