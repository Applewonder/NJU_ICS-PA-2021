#ifndef __INSTR_RET_H__
#define __INSTR_RET_H__
/*
Put the declarations of `ret' instructions here.
*/
int ret_near(uint32_t eip, uint8_t opcode);
int ret_near_imm16(uint32_t eip, uint8_t opcode);
#endif
