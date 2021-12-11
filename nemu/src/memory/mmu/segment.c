#include "cpu/cpu.h"
#include "memory/memory.h"

// return the linear address from the virtual address and segment selector
uint32_t segment_translate(uint32_t offset, uint8_t sreg)
{
	/* TODO: perform segment translation from virtual address to linear address
	 * by reading the invisible part of the segment register 'sreg'
	 */
	uint32_t res = cpu.segReg[sreg].base + offset;
	return res;
}

// load the invisible part of a segment register
void load_sreg(uint8_t sreg)
{
	/* TODO: load the invisibile part of the segment register 'sreg' by reading the GDT.
	 * The visible part of 'sreg' should be assigned by mov or ljmp already.
	 */
// 	 cpu.segReg[sreg].base = ((cpu.gdtr.base + cpu.segReg[sreg].index * 8).base_31_24 << 24) + (cpu.gdtr.base[cpu.segReg[sreg].index].base_23_16 << 16) + cpu.gdtr.base[cpu.segReg[sreg].index].base_15_0;
// 	 cpu.segReg[sreg].limit = (cpu.gdtr.base[cpu.segReg[sreg].index].limit_19_16 << 16) + cpu.gdtr.base[cpu.segReg[sreg].index].limit_15_0;
// 	 cpu.segReg[sreg].type = cpu.gdtr.base[cpu.segReg[sreg].index].type;
// 	 cpu.segReg[sreg].privilege_level = cpu.gdtr.base[cpu.segReg[sreg].index].privilege_level;
// 	 cpu.segReg[sreg].soft_use = cpu.gdtr.base[cpu.segReg[sreg].index].soft_use;
    SegDesc seg;
    seg.val[0] = vaddr_read(cpu.gdtr.base + cpu.segReg[sreg].index * 8, sreg, 4);
    seg.val[1] = vaddr_read(cpu.gdtr.base + cpu.segReg[sreg].index * 8 + 4, sreg, 4);
    printf("index = %x\n", cpu.segReg[sreg].index);
    printf("granularity = %x, reg.base = %x, %x, %x, val[1] = %x, val[2]%x\n", seg.granularity, seg.base_31_24, seg.base_23_16, seg.base_15_0, seg.val[0], seg.val[1]);
    printf("limit_19_16 = %x, limit_15_0 = %x", seg.limit_19_16, seg.limit_15_0)
    assert(seg.granularity == 1);
    cpu.segReg[sreg].base = (seg.base_31_24 << 24) + (seg.base_23_16 << 16) + seg.base_15_0;
    cpu.segReg[sreg].limit = (seg.limit_19_16 << 16) + seg.limit_15_0;
    cpu.segReg[sreg].type = seg.type;
    cpu.segReg[sreg].privilege_level = seg.privilege_level;
    cpu.segReg[sreg].soft_use = seg.soft_use;
    assert(cpu.segReg[sreg].base == 0);
    assert(cpu.segReg[sreg].limit == 0xffffffff);
 }
