#include "cpu/intr.h"
#include "cpu/instr.h"

/*
Put the implementations of `int' instructions here.

Special note for `int': please use the instruction name `int_' instead of `int'.
*/
make_instr_func(int_)
{
	OPERAND opr;
	opr.addr = cpu.eip + 1;
	opr.type = OPR_IMM;
	opr.data_size = 32;
	operand_read(&opr);
	
	raise_sw_intr(opr.val);
	return 0;
}
