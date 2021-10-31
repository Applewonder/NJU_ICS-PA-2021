#include "cpu/instr.h"
/*
Put the implementations of `test' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_and(sign_ext(opr_src.val, opr_src.data_size), sign_ext(opr_dest.val, opr_dest.data_size), data_size);
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	operand_write(&opr_dest);
}
make_instr_impl_2op(test, r, rm, v)
make_instr_impl_2op(test, r, rm, b)
make_instr_impl_2op(test, i, rm, v)
make_instr_impl_2op(test, i, a, b)
make_instr_impl_2op(test, i, a, v)
make_instr_impl_2op(test, i, rm, b)
