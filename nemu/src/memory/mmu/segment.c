#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	uint32_t res = segReg[sreg].base + offset;
	return res;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
	 segReg[sreg].base = (cpu.gdtr.base[segReg[sreg].index].base_31_24 << 24) + (cpu.gdtr.base[segReg[sreg].index].base_23_16 << 16) + cpu.gdtr.base[segReg[sreg].index].base_15_0;
	 segReg[sreg].limit = (cpu.gdtr.base[segReg[sreg].index].limit_19_16 << 16) + cpu.gdtr.base[segReg[sreg].index].limit_15_0;
	 segReg[sreg].type = cpu.gdtr.base[segReg[sreg].index].type;
	 segReg[sreg].privilege_level = cpu.gdtr.base[segReg[sreg].index].privilege_level;
	 segReg[sreg].soft_use = cpu.gdtr.base[segReg[sreg].index].soft_use;
}
