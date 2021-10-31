#include "cpu/instr.h"
/*
Put the implementations of `adc' instructions here.
*/
void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_adc(opr_src.val, opr_dest.val, data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(adc, r, rm, v)
make_instr_impl_2op(adc, i, rm, b)
make_instr_impl_2op(adc, i, rm, v)
make_instr_impl_2op(adc, i, rm, bv)
make_instr_impl_2op(adc, r, rm, b)
make_instr_impl_2op(adc, rm, r, b)
make_instr_impl_2op(adc, rm, r, v)
make_instr_impl_2op(adc, i, a, b)
make_instr_impl_2op(adc, i, a, v)