#include "cpu/instr.h"
/*
Put the implementations of `pop' instructions here.
*/
static void instr_execute_1op()
{
    opr_src.type = OPR_MEM;
    opr_src.addr = cpu.esp;
    opr_src.data_size = data_size;
    
    operand_read(&opr_src);
    opr_dest.val = sign_ext(opr_src.val, data_size);
    operand_write(&opr_dest);
    cpu.esp += 4;
}
make_instr_impl_1op(pop, r, v)