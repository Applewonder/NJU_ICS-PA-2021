#include "cpu/instr.h"
/*
Put the implementations of `cmp' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(sign_ext(opr_src.val, opr_src.data_size), opr_dest.val, data_size);
}
make_instr_impl_2op(cmp, i, rm, bv)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, i, rm, v)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, i, a, v)
make_instr_impl_2op(cmp, i, rm, b)
make_instr_func(cmp, i, a, b)
{
    int len = 1;
    decode_data_size_b;
    decode_operand_a2b;
    print_asm_2(#inst_name, opr_dest.data_size == 8 ? "b" : (opr_dest.data_size == 16 ? "w" : "l"), len, &opr_src, &opr_dest);
    operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val, opr_dest.val, data_size);
	return len;
}