#include "cpu/instr.h"

make_instr_func(jmp_near)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.sreg = SREG_CS;
        rel.data_size = data_size;
        rel.addr = eip + 1;

        operand_read(&rel);

        int offset = sign_ext(rel.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("jmp", "", 1 + data_size / 8, &rel);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(jmp_short)
{
        OPERAND rel;
        rel.type = OPR_IMM;
        rel.data_size = 8;
        rel.addr = cpu.eip + 1;

        operand_read(&rel);
        cpu.eip += sign_ext(rel.val, 8) + 2;

        return 0;
}

make_instr_func(jmp_near_indirect)
{
        int len = 1;
        OPERAND reg;
        reg.data_size = 32;
        len += modrm_rm(eip + 1, &reg);
        
        operand_read(&reg);
        cpu.eip = reg.val;
        
        return 0;
}

make_instr_func(jmp_far_imm)
{
        cpu.cs.val = (1 << 3);
        OPERAND reg;
        reg.data_size = 32;
        reg.type = OPR_IMM;
        reg.addr = eip;
        printf("%x", eip);
        reg.sreg = SREG_CS;
        operand_read(&reg);
        cpu.eip = reg.val;
        return 0;
}
