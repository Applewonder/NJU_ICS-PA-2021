#include "cpu/intr.h"
#include "cpu/instr.h"
#include "memory/memory.h"

static void push_reg(uint32_t reg)
{
	cpu.esp -= 4;
	OPERAND opr_dest;
	opr_dest.type = OPR_MEM;
	opr_dest.addr = cpu.esp;
	opr_dest.data_size  = data_size;
	opr_dest.sreg = SREG_SS;
	
	opr_dest.val = reg;
	operand_write(&opr_dest);
}


void raise_intr(uint8_t intr_no)
{
#ifdef IA32_INTR
	push_reg(cpu.eflags.val);
	push_reg(cpu.segReg[1].val);
	push_reg(cpu.eip);
	uint32_t a = (uint32_t)hw_mem + (cpu.idtr.base - 0xc0000000) + (intr_no*8);
	
	GateDesc* b = (GateDesc*) a;
	cpu.segReg[1].val = b->selector;
	
	cpu.eip = (b->offset_31_16 << 16) + (b->offset_15_0);
	if(b->type == 0xE) cpu.eflags.IF = 0;
	load_sreg(1);
#endif
}

void raise_sw_intr(uint8_t intr_no)
{
	// return address is the next instruction
	cpu.eip += 2;
	raise_intr(intr_no);
}
