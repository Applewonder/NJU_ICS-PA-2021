#include "cpu/instr.h"
#include "device/port_io.h"
/*
Put the implementations of `out' instructions here.
*/
make_instr_func(out_b)
{
	int len = 1;
	opr_src.data_size = 8;
	opr_src.type = OPR_REG;
	opr_src.sreg = SREG_CS;
	opr_src.addr = 0;
	
	operand_read(&opr_src);
	
	uint32_t port = cpu.gpr[2]._16;
	pio_write(port, 1, opr_src.val);
	
	return len;
}


make_instr_func(out_v)
{
	//port
	int len = 1;
	opr_src.data_size = 16;
	opr_src.type = OPR_REG;
	opr_src.sreg = SREG_CS;
	opr_src.addr = 2;
	
	operand_read(&opr_src);
	
	//data
	opr_dest.addr = 0;
	opr_dest.type = OPR_REG;
	opr_dest.data_size = data_size;
	opr_dest.sreg = SREG_CS;
	
	operand_read(&opr_dest);
	
	pio_write(opr_src.val, data_size / 8, opr_dest.val);
	
	return len;
}
