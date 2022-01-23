#include "cpu/instr.h"
/*
Put the implementations of `push' instructions here.
*/
static void instr_execute_1op()
{
	//operand_read(&opr_src);
    
    
    operand_read(&opr_src);
    cpu.esp -= 4;
    opr_dest.type = OPR_MEM;
    opr_dest.addr = cpu.esp;
    opr_dest.data_size = data_size;
    
    
    
    //opr_dest.val = sign_ext(opr_src.val, data_size);
    opr_dest.val = opr_src.val;
    opr_dest.sreg = SREG_DS;
    operand_write(&opr_dest);
    
    /*
    OPERAND opr;
    opr.type = OPR_MEM;
    opr.data_size = data_size;
    opr.addr = cpu.esp;
    
    opr.val = opr_src.val;
    opr.sreg = SREG_DS;
    operand_write(&opr);
    * */
}
make_instr_impl_1op(push, r, v)
make_instr_impl_1op(push, rm, v)
make_instr_impl_1op(push, i, b)
make_instr_impl_1op(push, i, v)

static void push_reg(uint32_t reg)
{
    OPERAND opr_dest;
    cpu.esp -= 4;
    opr_dest.type = OPR_MEM;
    opr_dest.addr = cpu.esp;
    opr_dest.data_size = data_size;
    
    opr_dest.val = reg;
    operand_write(&opr_dest);
}

make_instr_func(pusha){
    int len = 1;    
    push_reg(cpu.eax);
    push_reg(cpu.ecx);
    push_reg(cpu.edx);
    push_reg(cpu.ebx);
    push_reg(cpu.esp);
    push_reg(cpu.ebp);
    push_reg(cpu.esi);
    push_reg(cpu.edi);
    return len;
}


