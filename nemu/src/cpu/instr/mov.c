#include "cpu/instr.h"

static void instr_execute_2op() 
{
	operand_read(&opr_src);
	opr_dest.val = opr_src.val;
	operand_write(&opr_dest);
}

make_instr_impl_2op(mov, r, rm, b)
make_instr_impl_2op(mov, r, rm, v)
make_instr_impl_2op(mov, rm, r, b)
make_instr_impl_2op(mov, rm, r, v)
make_instr_impl_2op(mov, i, rm, b)
make_instr_impl_2op(mov, i, rm, v)
make_instr_impl_2op(mov, i, r, b)
make_instr_impl_2op(mov, i, r, v)
make_instr_impl_2op(mov, a, o, b)
make_instr_impl_2op(mov, a, o, v)
make_instr_impl_2op(mov, o, a, b)
make_instr_impl_2op(mov, o, a, v)


make_instr_func(mov_zrm82r_v) {
	int len = 1;
	OPERAND r, rm;
	r.data_size = data_size;
	rm.data_size = 8;
	len += modrm_r_rm(eip + 1, &r, &rm);
	
	operand_read(&rm);
	r.val = rm.val;
	operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
	return len;
}

make_instr_func(mov_zrm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);

        operand_read(&rm);
        r.val = rm.val;
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm82r_v) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = data_size;
        rm.data_size = 8;
        len += modrm_r_rm(eip + 1, &r, &rm);
        
	operand_read(&rm);
        r.val = sign_ext(rm.val, 8);
        operand_write(&r);
	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_srm162r_l) {
        int len = 1;
        OPERAND r, rm;
        r.data_size = 32;
        rm.data_size = 16;
        len += modrm_r_rm(eip + 1, &r, &rm);
        operand_read(&rm);
        r.val = sign_ext(rm.val, 16);
        operand_write(&r);

	print_asm_2("mov", "", len, &rm, &r);
        return len;
}

make_instr_func(mov_rm2s_w) {
        int len = 1;
        OPERAND rm;
        rm.data_size = 16;
        len += modrm_rm(eip+1, &rm);
        operand_read(&rm);
        //printf("------>%d\n", rm.val);
        uint32_t m = instr_fetch(eip + 1, 1);
        int tag = (m >> 3) % 8;
        //assert(0);
        //printf("------>%d\n", tag);
        cpu.segReg[tag].val = rm.val;
        load_sreg(tag);
        
        return len;
}

make_instr_func(mov_r2c_l) {
        int len = 1;
        OPERAND r, c;
        r.data_size = 32;
        c.data_size = 32;
        c.type = OPR_CREG;
        len += modrm_r_rm(eip + 1,  &c, &r);
        operand_read(&r);
        c.val = r.val;
        operand_write(&c);
        
        return len;
}

make_instr_func(mov_c2r_l) {
        int len = 1;
        OPERAND r, c;
        r.data_size = 32;
        c.data_size = 32;
        len += modrm_r_rm(eip + 1, &r, &c);
        c.type = OPR_CREG;
        operand_read(&c);
        r.val = c.val;
        operand_write(&r);
        
        return len;
}

