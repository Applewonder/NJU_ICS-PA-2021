#include "cpu/cpu.h"

void set_CF_add(uint32_t result, uint32_t src, size_t data_size) {
    result = sign_ext(result & (0xFFFFFFFF >> (32 -data_size)), data_size);
    src= sign_ext(src& (0xFFFFFFFF >> (32 -data_size)), data_size);
    cpu.eflags.CF = result < src; 
}

void set_ZF(uint32_t result, size_t data_size) {
    result = result & (0xFFFFFFFF >> (32 -data_size));
    cpu.eflags.ZF= (result == 0);
}

void set_SF(uint32_t result, size_t data_size) {
    result = sign_ext(result & (0xFFFFFFFF >> (32 -data_size)), data_size);
    cpu.eflags.SF= sign(result);
}

void set_PF(uint32_t result){
    int count = 0;
    for(int i = 0; i < 8; i++) {
        if (result % 2 == 1) {
            count++;
        }
        result /= 2;
    }
    cpu.eflags.PF = (count % 2) ? 0 : 1;
}

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
    switch(data_size) {
        case 8:
        result = sign_ext(result & 0xFF, 8);
        src= sign_ext(src& 0xFF, 8);
        dest= sign_ext(dest& 0xFF, 8);
    break;
        case 16:
        result = sign_ext(result & 0xFFFF, 16);
        src= sign_ext(src& 0xFFFF, 16);
        dest= sign_ext(dest& 0xFFFF, 16);
    break;
    default: break;// do nothing
    }
    if(sign(src) == sign(dest)) {
        if(sign(src) != sign(result))
            cpu.eflags.OF= 1;
        else
            cpu.eflags.OF= 0;
    } 
    else {
        cpu.eflags.OF= 0;
        
    }
}

void set_CF_adc(uint32_t result, uint32_t src, size_t data_size) {
    if (cpu.eflags.CF == 0) {
	    set_CF_add(result, src, data_size);
	} else {
	    result = sign_ext(result & (0xFFFFFFFF >> (32 -data_size)), data_size);
        src= sign_ext(src& (0xFFFFFFFF >> (32 -data_size)), data_size);
        if(src == result) {
            cpu.eflags.CF = 1;
            return;
        }
        cpu.eflags.CF = result < src; 
	}
}

void set_CF_sub(uint32_t result, uint32_t dest, size_t data_size) {
    result = sign_ext(result & (0xFFFFFFFF >> (32 -data_size)), data_size);
    dest = sign_ext(dest & (0xFFFFFFFF >> (32 -data_size)), data_size);
    cpu.eflags.CF = result > dest;
}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
    switch(data_size) {
        case 8:
        result = sign_ext(result & 0xFF, 8);
        src= sign_ext(src& 0xFF, 8);
        dest= sign_ext(dest& 0xFF, 8);
    break;
        case 16:
        result = sign_ext(result & 0xFFFF, 16);
        src= sign_ext(src& 0xFFFF, 16);
        dest= sign_ext(dest& 0xFFFF, 16);
    break;
    default: break;// do nothing
    }
    if(sign(src) != sign(dest)) {
        if(sign(dest) != sign(result))
            cpu.eflags.OF= 1;
        else
            cpu.eflags.OF= 0;
    } 
    else {
        cpu.eflags.OF= 0;
        
    }
}

void set_CF_sbb(uint32_t result, uint32_t dest, size_t data_size) {
    if (cpu.eflags.CF == 0) {
	    set_CF_sub(result, dest, data_size);
	} else {
	    result = sign_ext(result & (0xFFFFFFFF >> (32 -data_size)), data_size);
        dest = sign_ext(dest & (0xFFFFFFFF >> (32 -data_size)), data_size);
        if(dest == result) {
            cpu.eflags.CF = 1;
            return;
        }
        cpu.eflags.CF = result > dest; 
	}
}

void set_shl_CF(uint32_t src, uint32_t dest, size_t data_size) {
    while (src != 0) {
        uint32_t tmp = sign_ext(dest, data_size);
        cpu.eflags.CF = sign(tmp);
        dest *= 2;
        src--;
    }
}

void set_shr_CF(uint32_t src, uint32_t dest, size_t data_size) {
    while (src != 0) {
        cpu.eflags.CF = (dest % 2);
        dest /= 2;
        src--;
    }
}

uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = 0;
	res = src + dest;
	
	set_CF_add(res, src, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	set_OF_add(res, src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{

	uint32_t res = 0;
	res = src + dest + cpu.eflags.CF;
	
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_CF_adc(res, src, data_size);
	set_OF_add(res, src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
    uint32_t res = 0;
	res = dest - src;
	
	set_PF(res);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_CF_sub(res, dest, data_size);
	set_OF_sub(res, src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
	
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
	res = dest - (src + cpu.eflags.CF);
	
	set_CF_sbb(res, dest, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	set_OF_sub(res, src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest & src;
	
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest ^ src;
	
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest | src;
	
	cpu.eflags.CF = 0;
	cpu.eflags.OF = 0;
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
	uint32_t res = 0;
	res = dest << src;
	
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	set_shl_CF(src, dest, data_size);
	
	return res & (0xFFFFFFFF >> (32 -data_size));
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	uint32_t res = 0;
	for(int i = 0; i < src; i++) {
	    dest /= 2;
	}
	res = dest;
	
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_PF(res);
	set_shr_CF(src, dest, data_size);
	
	
	return res & (0xFFFFFFFF >> (32 -data_size));
#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	printf("\e[0;31mPlease implement me at alu.c\e[0m\n");
	fflush(stdout);
	assert(0);
	return 0;
#endif
}
