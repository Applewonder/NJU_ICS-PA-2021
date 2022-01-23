#include "cpu/instr.h"
/*
Put the implementations of `iret' instructions here.
*/
static uint32_t pop_reg()
{
    OPERAND opr;
    opr.addr = cpu.esp;
    opr.type = OPR_MEM;
    opr.data_size = 32;
    opr.sreg = SREG_DS;
    operand_read(&opr);
    cpu.esp += 4;
    return opr.val;
}

make_instr_func(iret){
    int len = 0;    
    cpu.eip = pop_reg();
    cpu.segReg[1].val = pop_reg();
    //load_sreg(1);
    cpu.eflags.val = pop_reg();
    return len;
}
