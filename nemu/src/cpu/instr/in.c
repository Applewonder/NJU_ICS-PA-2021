#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `in' instructions here.
*/
make_instr_func(in_b)
{
	int len = 1;
	opr_src.data_size = 16;
	opr_src.type = OPR_REG;
	opr_src.sreg = SREG_CS;
	opr_src.addr = 2;
	
	operand_read(&opr_src);
	
	uint32_t val = pio_read(opr_src.val, 1);
	
	cpu.gpr[0]._8[0] = val;
	
	return len;
}

make_instr_func(in_v)
{
	int len = 1;
	opr_src.data_size = 16;
	opr_src.type = OPR_REG;
	opr_src.sreg = SREG_CS;
	opr_src.addr = 2;
	
	operand_read(&opr_src);
	
	uint32_t val = pio_read(opr_src.val, data_size/8);
	
	opr_dest.addr = 0;
	opr_dest.type = OPR_REG;
	opr_dest.data_size = data_size;
	opr_dest.sreg = SREG_CS;
	opr_dest.val = val;
	
	operand_write(&opr_dest);
	
	return len;
}
