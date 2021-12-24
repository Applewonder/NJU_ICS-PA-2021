#include "cpu/cpu.h"
#include "memory/memory.h"

// translate from linear address to physical address
paddr_t page_translate(laddr_t laddr)
{
#ifndef TLB_ENABLED
	uint32_t dir = laddr >> 22;
	uint32_t page = (laddr >> 12) % 1024;
	uint32_t offset = laddr % 4096;
	uint32_t m = (cpu.cr3.pdbr << 12) + 4*dir;
	PDE t;
	printf("laddr = -------->%x\n", laddr);
	fflush(stdout);
	printf("page = -------->%x\n", page);
	fflush(stdout);
	
	memcpy(&t.val, hw_mem + m, 4);
	
	assert(t.present == 1);
	PTE l;
	
	
//	m = (t.page_frame << 12) + 4*page;
	
	printf("m = -------->%x\n", m);
	fflush(stdout);
	printf("t.page_frame-------->%x\n", t.page_frame);
	fflush(stdout);
	
	memcpy(&l.val, hw_mem + m, 4);

	//assert(0);
//	printf("-------->%d", l.present);
// 	assert(l.present == 1);
 //	assert(0);
	paddr_t result = (l.page_frame << 12) + offset;
// 	assert(0);
	return result;
#else
	return tlb_read(laddr) | (laddr & PAGE_MASK);
#endif
}
