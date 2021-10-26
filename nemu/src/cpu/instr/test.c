#include "cpu/instr.h"
/*
Put the implementations of `test' instructions here.
*/
static void instr_execute_2op() 
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	opr_dest.val = alu_and(opr_src.val, opr_dest.val, data_size);
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	operand_write(&opr_dest);
}
make_instr_impl_2op(test, r, rm, v)
make_instr_impl_2op(test, r, rm, b)

make_instr_impl_2op(test, i, a, b)
make_instr_impl_2op(test, i, a, v)

int test_i2rm_v(uint32_t eip, uint8_t opcode)
{
    int len = 1;
    decode_data_size_v;
    decode_operand_i2rm;
    if (data_size == 16) {
        operand_read(&opr_src);
	    operand_read(&opr_dest);
	    opr_dest.val = alu_and(opr_src.val, opr_dest.val, data_size);
	    cpu.eflags.CF = 0;
	    cpu.eflags.OF = 0;
	    operand_write(&opr_dest);
	    return 4;
    }else if(data_size == 32) {
        operand_read(&opr_src);
	    operand_read(&opr_dest);
	    opr_dest.val = alu_and(opr_src.val, opr_dest.val, data_size);
	    cpu.eflags.CF = 0;
	    cpu.eflags.OF = 0;
	    operand_write(&opr_dest);
	    return 6;
    }
}}