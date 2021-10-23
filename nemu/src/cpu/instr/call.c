#include "cpu/instr.h"
/*
Put the implementations of `call' instructions here.
*/
static void call_near(uint32_t eip, uint8_t opcode) 
{
    operand_read(&opr_src);
    cpu.eip += opr_src.val;
}