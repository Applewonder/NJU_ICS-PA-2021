#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/
static void instr_execute_1op()
{
    cpu.esp -= 4;
    opr_dest.type = OPR_MEM;
    opr_dest.addr = cpu.esp;
    opr_dest.data_size = data_size;
    
    operand_read(&opr_src);
    opr_dest.val = sign_ext(opr_src.val, data_size);
    operand_write(&opr_dest);
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
